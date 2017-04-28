/*******************************************************************************
  MPLAB Harmony Application Source File
  
  Company:
    Microchip Technology Inc.
  
  File Name:
    pathmovement.c

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

#include "pathmovement.h"

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

static PATHMOVEMENT_DATA pathmovementData;

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
    void PATHMOVEMENT_Initialize ( void )

  Remarks:
    See prototype in pathmovement.h.
 */

void PATHMOVEMENT_Initialize ( void )
{
    /* Place the App state machine in its initial state. */
    pathmovementData.moveQ = xQueueCreate(10000, sizeof(char));
    if(pathmovementData.moveQ == NULL) {
        dbgSysHalt(DLOC_MSG_Q_SETUP_FAIL);
    }

    pathmovementData.state = PATHMOVEMENT_STATE_INIT;
    
    PLIB_INT_SourceEnable(INT_ID_0, INT_SOURCE_TIMER_4);
    DRV_TMR1_Start();
    PLIB_INT_SourceEnable(INT_ID_0, INT_SOURCE_TIMER_3);
    DRV_TMR2_Start();
    
    
    /* TODO: Initialize your application's state machine and other
     * parameters.
     */
}


/******************************************************************************
  Function:
    void PATHMOVEMENT_Tasks ( void )

  Remarks:
    See prototype in pathmovement.h.
 */

void PATHMOVEMENT_Tasks ( void )
{
    char move;
    char time;
    int turnPeriods=0;
    int moves [100];
    int moveIndex = 0;
    int currentMove = 0;
    int done = 0;
    int pulseCtr = 0;
    int armValue = ARM_UP;

    while(1){        
        if(pathmovementData.moveQ != 0) {
            // receive message on the q.  blocks indefinitely.
            if(xQueueReceive(pathmovementData.moveQ, &(move), portMAX_DELAY)) {
                if(move == TIMER_VAL){
                    turnPeriods++;
                    pulseCtr++;
                    
                    
                }
                else if(move == MOVE_RIGHT){
                    moves[moveIndex] = MOVE_RIGHT;
                    moveIndex++;
                }
                else if(move == MOVE_LEFT){
                    moves[moveIndex] = MOVE_LEFT;
                    moveIndex++;
                }
                else if(move == MOVE_FORWARD){
                    moves[moveIndex] = MOVE_FORWARD;
                    moveIndex++;
                }
                else if(move == MOVE_REVERSE){
                    moves[moveIndex] = MOVE_REVERSE;
                    moveIndex++;
                }
                else if(move == ARM_FORWARD){
//                    armValue = ARM_DOWN;
                    moves[moveIndex] = ARM_FORWARD;
                    moveIndex++;
                }
                else if(move == ARM_REVERSE){
//                    armValue = ARM_UP;
                    moves[moveIndex] = ARM_REVERSE;
                    moveIndex++;
                }
                else if(move == COMPLETE_STOP){
                    pathmovementData.state = PATHMOVEMENT_STATE_COMPLETE_STOP;
                }
                else if(move == ORIENT_DONE) {
                    pathmovementData.state = PATHMOVEMENT_STATE_STOP;
                }
                else if(move == WAIT) {
                    moves[moveIndex] = WAIT;
                    moveIndex++;
                }
                
                
            }
            else {
                dbgSysHalt(DLOC_MSG_Q_RX_FAIL);
            }
        }
        
        
        switch (pathmovementData.state) {
                /* Application's initial state. */
            case PATHMOVEMENT_STATE_INIT:
            {
                bool appInitialized = true;
                if (appInitialized) {
                    pathmovementData.state = PATHMOVEMENT_STATE_ORIENTATION;
                }
                break;
            }

            case PATHMOVEMENT_STATE_STOP:
            {
//                turnPeriods = 0;
                motorControlSendValToMsgQ(MOTOR_CONTROL_HALT);
                if (turnPeriods >= 500) {
                    turnPeriods = 0;
                    if (moveIndex > currentMove) {

                        if (moves[currentMove] == MOVE_LEFT) {
                            pathmovementData.state = PATHMOVEMENT_STATE_LEFT;
                        } 
                        else if (moves[currentMove] == MOVE_RIGHT) {
                            pathmovementData.state = PATHMOVEMENT_STATE_RIGHT;
                        } 
                        else if (moves[currentMove] == MOVE_FORWARD) {
                            pathmovementData.state = PATHMOVEMENT_STATE_FORWARD;
                        } 
                        else if (moves[currentMove] == MOVE_REVERSE) {
                            pathmovementData.state = PATHMOVEMENT_STATE_REVERSE;
                        }
                        else if (moves[currentMove] == ARM_FORWARD) {
                            pathmovementData.state = PATHMOVEMENT_STATE_ARM_FORWARD;
                        }
                        else if (moves[currentMove] == ARM_REVERSE) {
                            pathmovementData.state = PATHMOVEMENT_STATE_ARM_REVERSE;
                        }
                        else if (moves[currentMove] == WAIT) {
                            pathmovementData.state = PATHMOVEMENT_STATE_WAIT;
                        }
                        currentMove++;
                    }
                }
                break;
            }
            
            case PATHMOVEMENT_STATE_ORIENTATION:
            {
                // wait here until orientation is done
                break;
            }
            
            case PATHMOVEMENT_STATE_RIGHT:
            {
                if(turnPeriods >= 14000){
                    turnPeriods = 0;
                    pathmovementData.state = PATHMOVEMENT_STATE_STOP;
                }
                motorControlSendValToMsgQ(MOTOR_CONTROL_RIGHT);
                break;
            }
            case PATHMOVEMENT_STATE_LEFT:
            {
                if(turnPeriods >= 13800) {
                    turnPeriods = 0;
                    pathmovementData.state = PATHMOVEMENT_STATE_STOP;
                }
                motorControlSendValToMsgQ(MOTOR_CONTROL_LEFT);
                break;
            }
            case PATHMOVEMENT_STATE_FORWARD:
            {
                if(turnPeriods >= 7300) {
                    turnPeriods = 0;
                    pathmovementData.state = PATHMOVEMENT_STATE_STOP;
                }
                motorControlSendValToMsgQ(MOTOR_CONTROL_FORWARD);
                break;
            }
            
            case PATHMOVEMENT_STATE_REVERSE:
            {
                if(turnPeriods >= 6800) {
                    turnPeriods = 0;
                    pathmovementData.state = PATHMOVEMENT_STATE_STOP;
                }
                motorControlSendValToMsgQ(MOTOR_CONTROL_REVERSE);
                break;
            }
            
             case PATHMOVEMENT_STATE_COMPLETE_STOP:
            {
                
                motorControlSendValToMsgQ(MOTOR_CONTROL_HALT);
                turnPeriods = 0;
                moveIndex = 0;
                currentMove = 0;
                pathmovementData.state = PATHMOVEMENT_STATE_STOP;
                break;
            }
             
            case PATHMOVEMENT_STATE_ARM_FORWARD:
            {
                
                armValue = ARM_DOWN;
                pathmovementData.state = PATHMOVEMENT_STATE_STOP;
//                if(turnPeriods >= 30000) {
//                    turnPeriods = 0;
//                    pathmovementData.state = PATHMOVEMENT_STATE_STOP;
////                    sendTapeSensorQ(ARM_DONE);
//                }
                
                break;
            }
            
            case PATHMOVEMENT_STATE_ARM_REVERSE:
            {
                armValue = ARM_UP;
                pathmovementData.state = PATHMOVEMENT_STATE_STOP;
//                if(turnPeriods >= 20000) {
//                    turnPeriods = 0;
//                    pathmovementData.state = PATHMOVEMENT_STATE_STOP;
//                }
                
                break;
            }
            
            case PATHMOVEMENT_STATE_WAIT:
            {
                if(turnPeriods >= 20000) {
                    turnPeriods = 0;
                    pathmovementData.state = PATHMOVEMENT_STATE_STOP;
                }
                
                break;
            }


            default:
            {
                turnPeriods = 0;
                pathmovementData.state = PATHMOVEMENT_STATE_STOP;
                break;
            }
        }
    }
    /* Check the application's current state. */
//    
}

void sendTimerValtoPathMovement(char timerVal) {

    BaseType_t xHigherPriorityTaskWoken;
    
    xHigherPriorityTaskWoken = pdFALSE;
    
    if (xQueueSendFromISR(pathmovementData.moveQ, &timerVal, &xHigherPriorityTaskWoken)!=pdTRUE) {
        dbgSysHalt(DLOC_MSG_Q_FULL);
    }      
    
    portEND_SWITCHING_ISR(xHigherPriorityTaskWoken);
}

 

/*******************************************************************************
 End of File
 */
