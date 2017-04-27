/*******************************************************************************
  MPLAB Harmony Application Header File

  Company:
    Microchip Technology Inc.

  File Name:
    pathmovement.h

  Summary:
    This header file provides prototypes and definitions for the application.

  Description:
    This header file provides function prototypes and data type definitions for
    the application.  Some of these are required by the system (such as the
    "APP_Initialize" and "APP_Tasks" prototypes) and some of them are only used
    internally by the application (such as the "APP_STATES" definition).  Both
    are defined here for convenience.
*******************************************************************************/

//DOM-IGNORE-BEGIN
/*******************************************************************************
Copyright (c) 2013-2014 released Microchip Technology Inc.  All rights reserved.

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
//DOM-IGNORE-END

#ifndef _PATHMOVEMENT_H
#define _PATHMOVEMENT_H

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "FreeRTOS.h"
#include "queue.h"
#include "system_config.h"
#include "system_definitions.h"
#include "motorControl_public.h"
#include "debug.h"
#include "system/ports/sys_ports.h"

// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility

extern "C" {

#endif
// DOM-IGNORE-END 

// *****************************************************************************
// *****************************************************************************
// Section: Type Definitions
// *****************************************************************************
// *****************************************************************************
#define TIMER_VAL 0
#define MOVE_FORWARD 1
#define MOVE_REVERSE 2
#define MOVE_LEFT 3
#define MOVE_RIGHT 4

#define STOP 5
#define COMPLETE_STOP 6
#define ARM_FORWARD 7
#define ARM_REVERSE 8
#define INITIAL_ORIENT 9
#define ORIENT_DONE 10
#define WAIT 11
    
#define ARM_DOWN 296
#define ARM_UP 283
#define ARM_NEUTRAL 285
// *****************************************************************************
/* Application states

  Summary:
    Application states enumeration

  Description:
    This enumeration defines the valid application states.  These states
    determine the behavior of the application at various times.
*/

typedef enum
{
	/* Application's state machine's initial state. */
	PATHMOVEMENT_STATE_INIT = 0,
        PATHMOVEMENT_STATE_RIGHT,
        PATHMOVEMENT_STATE_LEFT,
        PATHMOVEMENT_STATE_FORWARD,
        PATHMOVEMENT_STATE_REVERSE,
        PATHMOVEMENT_STATE_STOP,
        PATHMOVEMENT_STATE_COMPLETE_STOP,
        PATHMOVEMENT_STATE_ARM_FORWARD,
        PATHMOVEMENT_STATE_ARM_REVERSE,
        PATHMOVEMENT_STATE_ORIENTATION,
        PATHMOVEMENT_STATE_WAIT,

	/* TODO: Define states used by the application state machine. */

} PATHMOVEMENT_STATES;


// *****************************************************************************
/* Application Data

  Summary:
    Holds application data

  Description:
    This structure holds the application's data.

  Remarks:
    Application strings and buffers are be defined outside this structure.
 */

typedef struct
{
    /* The application's current state */
    PATHMOVEMENT_STATES state;
    QueueHandle_t moveQ;
    /* TODO: Define any additional data used by the application. */

} PATHMOVEMENT_DATA;


// *****************************************************************************
// *****************************************************************************
// Section: Application Callback Routines
// *****************************************************************************
// *****************************************************************************
/* These routines are called by drivers when certain events occur.
*/
	
// *****************************************************************************
// *****************************************************************************
// Section: Application Initialization and State Machine Functions
// *****************************************************************************
// *****************************************************************************

/*******************************************************************************
  Function:
    void PATHMOVEMENT_Initialize ( void )

  Summary:
     MPLAB Harmony application initialization routine.

  Description:
    This function initializes the Harmony application.  It places the 
    application in its initial state and prepares it to run so that its 
    APP_Tasks function can be called.

  Precondition:
    All other system initialization routines should be called before calling
    this routine (in "SYS_Initialize").

  Parameters:
    None.

  Returns:
    None.

  Example:
    <code>
    PATHMOVEMENT_Initialize();
    </code>

  Remarks:
    This routine must be called from the SYS_Initialize function.
*/

void PATHMOVEMENT_Initialize ( void );


/*******************************************************************************
  Function:
    void PATHMOVEMENT_Tasks ( void )

  Summary:
    MPLAB Harmony Demo application tasks function

  Description:
    This routine is the Harmony Demo application's tasks function.  It
    defines the application's state machine and core logic.

  Precondition:
    The system and application initialization ("SYS_Initialize") should be
    called before calling this.

  Parameters:
    None.

  Returns:
    None.

  Example:
    <code>
    PATHMOVEMENT_Tasks();
    </code>

  Remarks:
    This routine must be called from SYS_Tasks() routine.
 */

void PATHMOVEMENT_Tasks( void );
void sendTimerValtoPathMovement(char timerVal);
void armControl(int dir);

#endif /* _PATHMOVEMENT_H */

//DOM-IGNORE-BEGIN
#ifdef __cplusplus
}
#endif
//DOM-IGNORE-END

/*******************************************************************************
 End of File
 */

