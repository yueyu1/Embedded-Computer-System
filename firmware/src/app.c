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

// DOM-IGNORE-BEGIN
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
// DOM-IGNORE-END


// *****************************************************************************
// *****************************************************************************
// Section: Included Files 
// *****************************************************************************
// *****************************************************************************

#include "app.h"
#include "motorControl_public.h"
#include "communication_public.h"
#include "pathMovement.h"

// *****************************************************************************
// *****************************************************************************
// Section: Global Data Definitions
// *****************************************************************************
// *****************************************************************************

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

static APP_DATA appData;

// *****************************************************************************
// *****************************************************************************
// Section: Application Callback Functions
// *****************************************************************************
// *****************************************************************************

/* TODO:  Add any necessary callback functions.
*/

// *****************************************************************************
// *****************************************************************************
// Section: Application Local Functions
// *****************************************************************************
// *****************************************************************************


/* TODO:  Add any necessary local functions.
*/


// *****************************************************************************
// *****************************************************************************
// Section: Application Initialization and State Machine Functions
// *****************************************************************************
// *****************************************************************************

/*******************************************************************************
  Function:
    void APP_Initialize ( void )

  Remarks:
    See prototype in app.h.
 */

void APP_Initialize ( void )
{
    /*Debug: Output location*/
//    dbgOutputLoc(DLOC_INIT_BEGIN);   
    // setup message queue
    // message queue that can hold 5 unsigned integers
    appData.encMsgQ = xQueueCreate(5, sizeof(unsigned int));
    if(appData.encMsgQ == NULL) {
        dbgSysHalt(DLOC_MSG_Q_SETUP_FAIL);
    }
}


/******************************************************************************
  Function:
    void APP_Tasks ( void )

  Remarks:
    See prototype in app.h.
 */

void APP_Tasks ( void )
{  
    /*Debug: Output location*/
//    dbgOutputLoc(DLOC_TASK_ENTER);
    
    unsigned int recvVal = 0;
    
    unsigned int encVal = 0;
    
    unsigned int motorState = MOTOR_CONTROL_HALT;
    
    unsigned int periodsElapsed = 0;
    
    int timeReached;
    
    // initialize interrupts for Timer 2
    PLIB_INT_SourceEnable(INT_ID_0, INT_SOURCE_TIMER_2);
    
    // start Timer2
    DRV_TMR0_Start();    
    
    // start Timer 4 (enc counter)
    
    
    // Debug: Output Location
    dbgOutputLoc(0x00);
    
    int cmdctr=0;

    while(1) {
//        if(cmdctr==0)sendTimerValtoPathMovement(MOVE_FORWARD);
//        else if(cmdctr==1)sendTimerValtoPathMovement(MOVE_FORWARD);
//        //    sendTimerValtoPathMovement(MOVE_RIGHT);
//        //    sendTimerValtoPathMovement(MOVE_LEFT);
//        else if(cmdctr==2)sendServoArmQ(BACKWARD);
//        else if(cmdctr==3)sendTimerValtoPathMovement(MOVE_FORWARD);
//        cmdctr++;
        if(appData.encMsgQ != 0) {
            // receive message on the q.  blocks indefinitely.

            /*Debug: Output location*/
//            dbgOutputLoc(DLOC_STATE_MSG_Q_WAIT_BEFORE_RECEIVE_MSG);
            if(xQueueReceive(appData.encMsgQ, &(recvVal), portMAX_DELAY)) {
//                DRV_TMR1_CounterClear();
            }
            else {
                dbgSysHalt(DLOC_MSG_Q_RX_FAIL);
            }

            /*Debug: Output location*/
//            dbgOutputLoc(DLOC_STATE_MSG_Q_WAIT_AFTER_RECEIVE_MSG);
        }
        
//        if(recvVal == MOTOR_REQUEST_ENCVAL) {
//            motorControlSendValToMsgQ((encVal & 0xFF));
//            continue;
//        }
//        else {
//            encVal = recvVal;
//        }
        
        // write the value to dbgLoc
//        dbgOutputLoc( encoderVal & 0xFF);
        
        timeReached = 0;
        ++periodsElapsed;
        if (periodsElapsed >= 100) {
            periodsElapsed = 0;
            timeReached = 1;
        }
        
//        if(periodsElapsed >= 10) {
//            periodsElapsed = 0;
//            switch(motorState) {
//                case MOTOR_CONTROL_HALT:
//                    motorControlSendValToMsgQ(MOTOR_CONTROL_HALT);
//                    //motorState = MOTOR_CONTROL_FORWARD;
//                    break;
//                case MOTOR_CONTROL_FORWARD:
//                    motorControlSendValToMsgQ(MOTOR_CONTROL_FORWARD);
//                    motorState = MOTOR_CONTROL_HALT;
//                    break;
//                case MOTOR_CONTROL_REVERSE:
//                    motorControlSendValToMsgQ(MOTOR_CONTROL_REVERSE);
//                    motorState = MOTOR_CONTROL_LEFT;
//                    break;
//                case MOTOR_CONTROL_LEFT:
//                    motorControlSendValToMsgQ(MOTOR_CONTROL_LEFT);
//                    motorState = MOTOR_CONTROL_RIGHT;
//                    break;
//                case MOTOR_CONTROL_RIGHT:
//                    motorControlSendValToMsgQ(MOTOR_CONTROL_RIGHT);
//                    motorState = MOTOR_CONTROL_HALT;
//                    break;
//                default: motorState = MOTOR_CONTROL_HALT;
//            }
//        }
        
//        switch(motorState) {
//            case MOTOR_CONTROL_HALT:
//                motorControlSendValToMsgQ(MOTOR_CONTROL_HALT);
//                //motorState = MOTOR_CONTROL_FORWARD;
//                break;
//            case MOTOR_CONTROL_FORWARD:
//                motorControlSendValToMsgQ(MOTOR_CONTROL_FORWARD);
//                if (timeReached == 1) {
//                    motorState = MOTOR_CONTROL_HALT;
//                    FINISHED = 1;
//                }
//                break;
//            case MOTOR_CONTROL_REVERSE:
//                motorControlSendValToMsgQ(MOTOR_CONTROL_REVERSE);
//                if (timeReached == 1) {
//                    motorState = MOTOR_CONTROL_HALT;
//                    FINISHED = 1;
//                }
//                break;
//            case MOTOR_CONTROL_LEFT:
//                motorControlSendValToMsgQ(MOTOR_CONTROL_LEFT);
//                if (timeReached == 1) {
//                    motorState = MOTOR_CONTROL_HALT;
//                    FINISHED = 1;
//                }
//                break;
//            case MOTOR_CONTROL_RIGHT:
//                motorControlSendValToMsgQ(MOTOR_CONTROL_RIGHT);
//                if (timeReached == 1) {
//                    motorState = MOTOR_CONTROL_HALT;
//                    FINISHED = 1;
//                }
//                break;
//            default: motorState = MOTOR_CONTROL_HALT;
//        }
    }
}

/******************************************************************************
  Function:
    int appSendTimerValToQ ( unsigned int )

  Remarks:
    See prototype in app_public.h.
 */
void appSendEncValToQ(unsigned int encVal) {
    BaseType_t xHigherPriorityTaskWoken;
    
    xHigherPriorityTaskWoken = pdFALSE;
    
    if (xQueueSendFromISR(appData.encMsgQ, &encVal, &xHigherPriorityTaskWoken)!=pdTRUE) {
        dbgSysHalt(DLOC_MSG_Q_FULL);
    }      
    
    portEND_SWITCHING_ISR(xHigherPriorityTaskWoken);
}

 

/*******************************************************************************
 End of File
 */
