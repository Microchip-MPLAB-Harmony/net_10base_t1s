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

#define DELAY_LED               (10)

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

extern SYSTICK_OBJECT systick; /* Instanced in plib_systick.c */

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
    memset(&appData, 0, sizeof(appData));

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
            SYS_CONSOLE_PRINT("\r========================================\r\n");
            SYS_CONSOLE_PRINT("    10BASE-T1S MACPHY DEMO (V1.0.0)\r\n");
            SYS_CONSOLE_PRINT("========================================\r\n");

            SYSTICK_TimerStart();
            PwmDrv_Init();

            appData.state = APP_STATE_SERVICE_TASKS;
            break;
        }

        case APP_STATE_SERVICE_TASKS:
        {
            uint32_t now = systick.tickCounter;
            if (now >= appData.nextLed) {
                uint8_t ledVal0;
                uint8_t ledVal1;
                appData.nextLed = now + DELAY_LED;
                if (appData.ledState) {
                    ledVal0 = appData.ledVal--;
                    if (0x00 == appData.ledVal) {
                        appData.ledState = false;
                    }
                } else {
                    ledVal0 = appData.ledVal++;
                    if (0xFF == appData.ledVal) {
                        appData.ledState = true;
                    }
                }
                ledVal1 = 0xFF - ledVal0;
                PwmDrv_SetLevel(0, ledVal0);
                PwmDrv_SetLevel(1, ledVal1);
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
