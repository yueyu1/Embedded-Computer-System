/*******************************************************************************
 System Tasks File

  File Name:
    system_tasks.c

  Summary:
    This file contains source code necessary to maintain system's polled state
    machines.

  Description:
    This file contains source code necessary to maintain system's polled state
    machines.  It implements the "SYS_Tasks" function that calls the individual
    "Tasks" functions for all the MPLAB Harmony modules in the system.

  Remarks:
    This file requires access to the systemObjects global data structure that
    contains the object handles to all MPLAB Harmony module objects executing
    polled in the system.  These handles are passed into the individual module
    "Tasks" functions to identify the instance of the module to maintain.
 *******************************************************************************/

// DOM-IGNORE-BEGIN
/*******************************************************************************
Copyright (c) 2013-2015 released Microchip Technology Inc.  All rights reserved.

Microchip licenses to you the right to use, modify, copy and distribute
Software only when embedded on a Microchip microcontroller or digital signal
controller that is integrated into your product or third party product
(pursuant to the sublicense terms in the accompanying license agreement).

You should refer to the license agreement accompanying this Software for
additional information regarding your rights and obligations.

SOFTWARE AND DOCUMENTATION ARE PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF
MERCHANTABILITY, TITLE, NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR PURPOSE.
IN NO EVENT SHALL MICROCHIP OR ITS LICENSORS BE LIABLE OR OBLIGATED UNDER
CONTRACT, NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION, BREACH OF WARRANTY, OR
OTHER LEGAL EQUITABLE THEORY ANY DIRECT OR INDIRECT DAMAGES OR EXPENSES
INCLUDING BUT NOT LIMITED TO ANY INCIDENTAL, SPECIAL, INDIRECT, PUNITIVE OR
CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF PROCUREMENT OF
SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY CLAIMS BY THIRD PARTIES
(INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF), OR OTHER SIMILAR COSTS.
 *******************************************************************************/
// DOM-IGNORE-END


// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include "system_config.h"
#include "system_definitions.h"
#include "app.h"
#include "motorcontrol.h"
#include "communication.h"
#include "pathmovement.h"
#include "tapesensor.h"
#include "controller.h"
#include "colorsensor.h"


// *****************************************************************************
// *****************************************************************************
// Section: Local Prototypes
// *****************************************************************************
// *****************************************************************************


 
static void _SYS_Tasks ( void );
static void _APP_Tasks(void);
static void _MOTORCONTROL_Tasks(void);
static void _COMMUNICATION_Tasks(void);
static void _PATHMOVEMENT_Tasks(void);
static void _TAPESENSOR_Tasks(void);
static void _CONTROLLER_Tasks(void);
static void _COLORSENSOR_Tasks(void);


// *****************************************************************************
// *****************************************************************************
// Section: System "Tasks" Routine
// *****************************************************************************
// *****************************************************************************

/*******************************************************************************
  Function:
    void SYS_Tasks ( void )

  Remarks:
    See prototype in system/common/sys_module.h.
*/

void SYS_Tasks ( void )
{
    /* Create OS Thread for Sys Tasks. */
    xTaskCreate((TaskFunction_t) _SYS_Tasks,
                "Sys Tasks",
                1024, NULL, 0, NULL);

    /* Create OS Thread for APP Tasks. */
    xTaskCreate((TaskFunction_t) _APP_Tasks,
                "APP Tasks",
                1024, NULL, 1, NULL);

    /* Create OS Thread for MOTORCONTROL Tasks. */
    xTaskCreate((TaskFunction_t) _MOTORCONTROL_Tasks,
                "MOTORCONTROL Tasks",
                1024, NULL, 1, NULL);

    /* Create OS Thread for COMMUNICATION Tasks. */
    xTaskCreate((TaskFunction_t) _COMMUNICATION_Tasks,
                "COMMUNICATION Tasks",
                1024, NULL, 1, NULL);

    /* Create OS Thread for PATHMOVEMENT Tasks. */
    xTaskCreate((TaskFunction_t) _PATHMOVEMENT_Tasks,
                "PATHMOVEMENT Tasks",
                1024, NULL, 1, NULL);

    /* Create OS Thread for TAPESENSOR Tasks. */
    xTaskCreate((TaskFunction_t) _TAPESENSOR_Tasks,
                "TAPESENSOR Tasks",
                1024, NULL, 1, NULL);

    /* Create OS Thread for CONTROLLER Tasks. */
    xTaskCreate((TaskFunction_t) _CONTROLLER_Tasks,
                "CONTROLLER Tasks",
                1024, NULL, 1, NULL);

    /* Create OS Thread for COLORSENSOR Tasks. */
    xTaskCreate((TaskFunction_t) _COLORSENSOR_Tasks,
                "COLORSENSOR Tasks",
                1024, NULL, 1, NULL);

    /**************
     * Start RTOS * 
     **************/
    vTaskStartScheduler(); /* This function never returns. */
}


/*******************************************************************************
  Function:
    void _SYS_Tasks ( void )

  Summary:
    Maintains state machines of system modules.
*/
static void _SYS_Tasks ( void)
{
    while(1)
    {
        /* Maintain system services */
        SYS_DEVCON_Tasks(sysObj.sysDevcon);

        /* Maintain Device Drivers */

        /* Maintain Middleware */

        /* Task Delay */
    }
}


/*******************************************************************************
  Function:
    void _APP_Tasks ( void )

  Summary:
    Maintains state machine of APP.
*/

static void _APP_Tasks(void)
{
    while(1)
    {
        APP_Tasks();
    }
}


/*******************************************************************************
  Function:
    void _MOTORCONTROL_Tasks ( void )

  Summary:
    Maintains state machine of MOTORCONTROL.
*/

static void _MOTORCONTROL_Tasks(void)
{
    while(1)
    {
        MOTORCONTROL_Tasks();
    }
}


/*******************************************************************************
  Function:
    void _COMMUNICATION_Tasks ( void )

  Summary:
    Maintains state machine of COMMUNICATION.
*/

static void _COMMUNICATION_Tasks(void)
{
    while(1)
    {
        COMMUNICATION_Tasks();
    }
}


/*******************************************************************************
  Function:
    void _PATHMOVEMENT_Tasks ( void )

  Summary:
    Maintains state machine of PATHMOVEMENT.
*/

static void _PATHMOVEMENT_Tasks(void)
{
    while(1)
    {
        PATHMOVEMENT_Tasks();
    }
}


/*******************************************************************************
  Function:
    void _TAPESENSOR_Tasks ( void )

  Summary:
    Maintains state machine of TAPESENSOR.
*/

static void _TAPESENSOR_Tasks(void)
{
    while(1)
    {
        TAPESENSOR_Tasks();
    }
}


/*******************************************************************************
  Function:
    void _CONTROLLER_Tasks ( void )

  Summary:
    Maintains state machine of CONTROLLER.
*/

static void _CONTROLLER_Tasks(void)
{
    while(1)
    {
        CONTROLLER_Tasks();
    }
}


/*******************************************************************************
  Function:
    void _COLORSENSOR_Tasks ( void )

  Summary:
    Maintains state machine of COLORSENSOR.
*/

static void _COLORSENSOR_Tasks(void)
{
    while(1)
    {
        COLORSENSOR_Tasks();
    }
}


/*******************************************************************************
 End of File
 */

