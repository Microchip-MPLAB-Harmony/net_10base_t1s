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

#include "definitions.h"
#include "app.h"
#include "tcpip/tcpip.h"
#include "system/console/sys_console.h"
#include "pwm_driver/pwm_driver.h"

/******************************************************************************
*  Global Data Definitions
******************************************************************************/

#define DELAY_LED               (10u)

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
    (void)memset(&appData, 0, sizeof(appData));

	/* Place the App state machine in its initial state. */
	appData.state = APP_WAIT_STACK_INIT;
}


/******************************************************************************
  Function:
    static void OnIdRead(void *reserved1, bool success, uint32_t addr, uint32_t value, void *pTag, void *reserved2)

  Remarks:
    Callback on ID register read
 */
static void OnIdRead(void *reserved1, bool success, uint32_t addr, uint32_t value, void *pTag, void *reserved2)
{
    (void)reserved1;
    (void)addr;
    (void)pTag;
    (void)reserved2;
    if (success) {
        uint32_t major = (value >> 4) & 0x3Fu;
        uint32_t minor = value & 0xFu;
        SYS_CONSOLE_PRINT("MAC-PHY ID=%d.%d\r\n", major, minor);
    } else {
        SYS_CONSOLE_PRINT("MAC-PHY ID register read failed\r\n");
    }
}

/******************************************************************************
  Function:
    void APP_Tasks ( void )

  Remarks:
    See prototype in app.h.
 */
void APP_Tasks(void)
{
	/* Check the application's current state. */
	switch (appData.state)
    {
        /* Wait till TCP stack is initialized. */
        case APP_WAIT_STACK_INIT:
        {
            if (TCPIP_STACK_Status(sysObj.tcpip) == SYS_STATUS_READY)
            {
                appData.state = APP_GENERAL_INIT;
            }
            break;
        }

        /* Initialize the MIIM instance. */
        case APP_GENERAL_INIT:
        {
            TCPIP_MAC_RES result = DRV_LAN865X_ReadRegister(0u /* first instance */, 0x00000000 /* ID-Register */, true /* protected */, OnIdRead, NULL);
            if (TCPIP_MAC_RES_OK == result) {
                PwmDrv_Init();

                appData.state = APP_STATE_SERVICE_TASKS;
            }
            break;
        }

        case APP_STATE_SERVICE_TASKS:
        {
            uint32_t now = SYS_TIME_CounterGet();
            if (now >= appData.nextLed) {
                uint8_t ledVal0;
                uint8_t ledVal1;
                appData.nextLed = now + DELAY_LED;
                if (appData.ledState) {
                    appData.ledVal--;
                    ledVal0 = appData.ledVal;
                    if (0x00u == appData.ledVal) {
                        appData.ledState = false;
                    }
                } else {
                    appData.ledVal++;
                    ledVal0 = appData.ledVal;
                    if (0xFFu == appData.ledVal) {
                        appData.ledState = true;
                    }
                }
                ledVal1 = 0xFFu - ledVal0;
                (void)PwmDrv_SetLevel(0, ledVal0);
                (void)PwmDrv_SetLevel(1, ledVal1);
            }
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
