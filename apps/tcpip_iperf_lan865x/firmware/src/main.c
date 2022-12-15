/*******************************************************************************
  Main Source File

  Company:
    Microchip Technology Inc.

  File Name:
    main.c

  Summary:
    This file contains the "main" function for a project.

  Description:
    This file contains the "main" function for a project.  The
    "main" function calls the "SYS_Initialize" function to initialize the state
    machines of all modules in the system
 *******************************************************************************/

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include <stddef.h>                     // Defines NULL
#include <stdbool.h>                    // Defines true
#include <stdlib.h>                     // Defines EXIT_FAILURE
#include "definitions.h"                // SYS function prototypes


// *****************************************************************************
// *****************************************************************************
// Section: Main Entry Point
// *****************************************************************************
// *****************************************************************************

int main ( void )
{
    /* Initialize all modules */
    SYS_Initialize ( NULL );

    SYS_CONSOLE_PRINT("\033[2J");      /* Clear Terminal */
    SYS_CONSOLE_PRINT("\033[1;1H");    /* Set Cursor to x=1 y=1 */
    SYS_CONSOLE_PRINT("========================================\r\n");
#ifdef INC_FREERTOS_H
    SYS_CONSOLE_PRINT("   10BASE-T1S MACPHY FreeRTOS (V1.0.0)\r\n");
#else
    SYS_CONSOLE_PRINT("   10BASE-T1S MACPHY mainLoop (V1.0.0)\r\n");
#endif
    SYS_CONSOLE_PRINT("    [ BUILD " __DATE__ "  " __TIME__ " ]\r\n");
    SYS_CONSOLE_PRINT("========================================\r\n");

    while ( true )
    {
        /* Maintain state machines of all polled MPLAB Harmony modules. */
        SYS_Tasks ( );
    }

    /* Execution should not come here during normal operation */

    return ( EXIT_FAILURE );
}


/*******************************************************************************
 End of File
*/

