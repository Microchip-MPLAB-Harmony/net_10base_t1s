/*******************************************************************************
  MPLAB Harmony Application Source File

  Company:
    Microchip Technology Inc.

  File Name:
    app.c

  Summary:
    This file contains the source code for the MPLAB Harmony application.

  Description:
    This file contains the source code for the MPLAB Harmony application.  It
    implements the logic of the application's state machine and it may call
    API routines of other MPLAB Harmony modules in the system, such as drivers,
    system services, and middleware.  However, it does not call any of the
    system interfaces (such as the "Initialize" and "Tasks" functions) of any of
    the modules in the system or make any assumptions about when those functions
    are called.  That is the responsibility of the configuration-specific system
    files.
 *******************************************************************************/

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include "app.h"
#include "tcpip/tcpip.h"
#include "system/console/sys_console.h"
#include "driver/ethphy/src/dynamic/drv_extphy_lan867x.h"
#include "definitions.h"

/******************************************************************************
*  Global Data Definitions
******************************************************************************/

// *****************************************************************************
/* Application Data

  Summary:
    Holds application data

  Description:
    This structure holds the application's data.

  Remarks:
    This structure should be initialized by the APP_Initialize function.

    Application strings and buffers are be defined outside this structure.
 */

APP_DATA appData;

static uint16_t                  data;                // Register operation data.
static SYS_MODULE_INDEX          miimObjIx = 0;       // MIIM object index
static LAN867X_REG_OBJ           clientObj = {0};
static DRV_MIIM_OPERATION_HANDLE opHandle;

/******************************************************************************
*  Private function declaration
******************************************************************************/
static DRV_MIIM_RESULT local_miim_init(void);
static void local_miim_close(void);
#if 0
static DRV_MIIM_RESULT Write_Phy_Register (LAN867X_REG_OBJ *clientObj, int phyAddress, const uint32_t regAddr, uint16_t wData);
static DRV_MIIM_RESULT Write_Bit_Phy_Register (LAN867X_REG_OBJ *clientObj, int phyAddress, const uint32_t regAddr, uint16_t mask, uint16_t wData);
#endif
static DRV_MIIM_RESULT Read_Phy_Register (LAN867X_REG_OBJ *clientObj, int phyAddress, const uint32_t regAddr, uint16_t *rData);

/******************************************************************************
*  Function Definitions
******************************************************************************/

/*******************************************************************************
  Function:
    void APP_Initialize ( void )

  Remarks:
    See prototype in app.h.
 */
void APP_Initialize(void)
{
	/* Place the App state machine in its initial state. */
	appData.state = APP_WAIT_STACK_INIT;
}

/******************************************************************************
  Function:
    void APP_Tasks ( void )

  Remarks:
    See prototype in app.h.
 */
void APP_Tasks(void)
{
	DRV_MIIM_RESULT opRes = DRV_MIIM_RES_OK;
	/* Check the application's current state. */
	switch (appData.state)
    {
        /* Wait till TCP stack is initialized. */
        case APP_WAIT_STACK_INIT:
        {
            if (TCPIP_STACK_Status(sysObj.tcpip) == SYS_STATUS_READY)
            {
                appData.state = APP_MIIM_INIT;
            }
            break;
        }

        /* Initialize the MIIM instance. */
        case APP_MIIM_INIT:
        {
            /*  Setup the MIIM driver instance. */
            if (local_miim_init() < 0)
            {
                SYS_CONSOLE_PRINT("App: miim setup failed !\r\n");
            }

            appData.state = APP_READ_OPERATION_MODE;
            break;
        }

        /* Read in which mode of 10BaseT1s is working, CSMA/CD or PLCA. */
        case APP_READ_OPERATION_MODE:
        {
            // Example for Read register.
            opRes = Read_Phy_Register(&clientObj, 0, PHY_PLCA_CONTROL_0, &data);

            if (opRes < 0)
            {
                /* In case of an error, report and close miim instance. */
                SYS_CONSOLE_PRINT("Error occured:%d\r\n", opRes);
                appData.state = APP_MIIM_CLOSE;
            }
            else if (opRes == DRV_MIIM_RES_OK) /* Check operation is completed. */
            {
                if(1 == R2F(data, PHY_PLCA_CTRL0_EN))
                {
                    SYS_CONSOLE_PRINT(" Lan867x is in PLCA mode. \r\n", data);
                    appData.state = APP_READ_PLCA_CONFIGURATION;
                }
                else
                {
                    SYS_CONSOLE_PRINT(" Lan867x is in CSMA mode. \r\n", data);
                    appData.state = APP_MIIM_CLOSE;
                }
            }
            break;
        }

#if 0
        /* Write register example: Set the PLCA configuration, node ID and node count. */
        case APP_WRITE_PLCA_CONFIGURATION:
        {
            /* Set the Node id as 0 and Node count as 5*/
            data = F2R_(0, PHY_PLCA_CTRL1_ID0) | F2R_(5, PHY_PLCA_CTRL1_NCNT);
            opRes = Write_Phy_Register(&clientObj, 0, PHY_PLCA_CONTROL_1, data);

            if (opRes < 0)
            {
                /* In case of an error, report and close miim instance. */
                SYS_CONSOLE_PRINT("Error occured:%d\r\n", opRes);
                appData.state = APP_MIIM_CLOSE;
            }
            else if (opRes == DRV_MIIM_RES_OK) /* Check operation is completed. */
            {
                SYS_CONSOLE_PRINT(" Register set, Node Id: %d, Node count: %d. \r\n", R2F(data, PHY_PLCA_CTRL1_ID0), R2F(data, PHY_PLCA_CTRL1_NCNT));
                appData.state = APP_READ_PLCA_CONFIGURATION;
            }
            break;
        }
#endif

        /* Read the PLCA configuration. */
        case APP_READ_PLCA_CONFIGURATION:
        {
            opRes = Read_Phy_Register(&clientObj, 0, PHY_PLCA_CONTROL_1, &data);
            if (opRes < 0)
            {
                /* In case of an error, report and close miim instance. */
                SYS_CONSOLE_PRINT("Error occured:%d\r\n", opRes);
                appData.state = APP_MIIM_CLOSE;
            }
            else if (opRes == DRV_MIIM_RES_OK) /* Check operation is completed. */
            {
                SYS_CONSOLE_PRINT(" Node Id: %d, Node count: %d. \r\n", R2F(data, PHY_PLCA_CTRL1_ID0), R2F(data, PHY_PLCA_CTRL1_NCNT));
                appData.state = APP_MIIM_CLOSE;
            }
            break;
        }

        /* Close miim driver instance. */
        case APP_MIIM_CLOSE:
        {
            /* Close and release the handle(instance) to miim, as I do not need access to miim register anymore. */
            local_miim_close();

            appData.state = APP_STATE_SERVICE_TASKS;
            break;
        }

        case APP_STATE_SERVICE_TASKS:
        {
            /* Any operation to be done. */
            break;
        }

        /* The default state should never be executed. */
        default:
        {
            /* TODO: Handle error in application's state machine. */
            break;
        }
	}
}


// *****************************************************************************
// Section: Application Initialization
// *****************************************************************************

static DRV_MIIM_RESULT local_miim_init(void)
{
	DRV_MIIM_SETUP miimSetup;
	DRV_MIIM_RESULT res;

	opHandle                    = 0;
	clientObj.miimOpHandle      = &opHandle;
	clientObj.miimBase          = &DRV_MIIM_OBJECT_BASE_Default;
	miimObjIx                   = DRV_MIIM_DRIVER_INDEX;

	/*  Open the MIIM driver and get an instance to it. */
	clientObj.miimHandle = clientObj.miimBase->DRV_MIIM_Open(miimObjIx, DRV_IO_INTENT_SHARED);
	if ( (clientObj.miimHandle == DRV_HANDLE_INVALID) || (clientObj.miimHandle == 0))
	{
		SYS_CONSOLE_PRINT("> Local miim open: failed!\r\n");
		clientObj.miimHandle = 0;
		res = DRV_MIIM_RES_OP_INTERNAL_ERR;
	}
	else
	{

		miimSetup.hostClockFreq = (uint32_t) TCPIP_INTMAC_PERIPHERAL_CLK;
		miimSetup.maxBusFreq = 2000000;
		miimSetup.setupFlags = 0;

		/*  Setup the miim driver instance. */
		res = clientObj.miimBase->DRV_MIIM_Setup(clientObj.miimHandle, &miimSetup);
		if (res < 0)
		{
			SYS_CONSOLE_PRINT("> Local miim setup: failed!\r\n");
		}
		else
		{
			SYS_CONSOLE_PRINT("> Miim Successfully opened. \r\n");
		}
	}

	return res;
}

static void local_miim_close(void)
{
	clientObj.miimBase->DRV_MIIM_Close(clientObj.miimHandle);
	clientObj.miimHandle = 0;
	SYS_CONSOLE_PRINT("> Miim closed. \r\n");
}

#if 0
static DRV_MIIM_RESULT Write_Phy_Register (LAN867X_REG_OBJ *clientObj, int phyAddress, const uint32_t regAddr, uint16_t wData)
{
	clientObj->phyAddress = phyAddress;
	return Lan867x_Write_Register(clientObj, regAddr, wData);
}

static DRV_MIIM_RESULT Write_Bit_Phy_Register (LAN867X_REG_OBJ *clientObj, int phyAddress, const uint32_t regAddr, uint16_t mask, uint16_t wData)
{
	clientObj->phyAddress = phyAddress;
	return Lan867x_Write_Bit_Register(clientObj, regAddr, mask, wData);
}
#endif

static DRV_MIIM_RESULT Read_Phy_Register (LAN867X_REG_OBJ *clientObj, int phyAddress, const uint32_t regAddr, uint16_t *rData)
{
	clientObj->phyAddress = phyAddress;
	return Lan867x_Read_Register(clientObj, regAddr, rData);
}

/*********************************** End of File *******************************/
