/*******************************************************************************
  MPLAB Harmony Application Source File
  
  Company:
    Microchip Technology Inc.
  
  File Name:
    servoarm.c

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

#include "servoarm.h"

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

static SERVOARM_DATA servoarmData;

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
    void SERVOARM_Initialize ( void )

  Remarks:
    See prototype in servoarm.h.
 */

void SERVOARM_Initialize ( void )
{
    /* Place the App state machine in its initial state. */
    servoarmData.state = SERVOARM_STATE_INIT;
    servoarmData.servoarmQ = xQueueCreate(500, sizeof(unsigned int));
    if(servoarmData.servoarmQ == NULL) {
        dbgSysHalt(DLOC_MSG_Q_SETUP_FAIL);
    }
    servoarmData.moveIndex = 0;
    servoarmData.curMove = 0;
    
    servoarmData.pulseCtr = 0;
    servoarmData.stateCtr = 0;
     
    /* TODO: Initialize your application's state machine and other
     * parameters.
     */
}


/******************************************************************************
  Function:
    void SERVOARM_Tasks ( void )

  Remarks:
    See prototype in servoarm.h.
 */

void SERVOARM_Tasks ( void )
{

    PLIB_INT_SourceEnable(INT_ID_0, INT_SOURCE_TIMER_4);
    DRV_TMR1_Start();

    
    int recvServoVal;
    int ctr = 0;
    moveArm(0);
    
    while (1) {

        
        if(servoarmData.servoarmQ != 0) {
            // receive message on the q.  blocks indefinitely.

            /*Debug: Output location*/
//            dbgOutputLoc(DLOC_STATE_MSG_Q_WAIT_BEFORE_RECEIVE_MSG);
            if(xQueueReceive(servoarmData.servoarmQ, &(recvServoVal), portMAX_DELAY)) {
                if(recvServoVal == FORWARD){
//                    moveArm(0);
                    servoarmData.state = SERVOARM_STATE_FORWARD;
                }
                else if(recvServoVal == BACKWARD){
//                    moveArm(0);
                    servoarmData.state = SERVOARM_STATE_REVERSE;
                }
                else {
                    servoarmData.pulseCtr++;
                }
                
            }
            else {
                dbgSysHalt(DLOC_MSG_Q_RX_FAIL);
            }
            /*Debug: Output location*/
//            dbgOutputLoc(DLOC_STATE_MSG_Q_WAIT_AFTER_RECEIVE_MSG);
        }
//        if(pulseCtr>=190){
//            moveArm(1);
//        }
//        else {
//            moveArm(0);
//        }

        switch (servoarmData.state) {
                /* Application's initial state. */
            case SERVOARM_STATE_INIT:
            {
                bool appInitialized = true;
                if (appInitialized) {
                    servoarmData.state = SERVOARM_STATE_HALT;
                }
                
                break;
            }
            case SERVOARM_STATE_WAIT:
            {
                moveArm(0);
                break;
            }
            case SERVOARM_STATE_FORWARD:
            {

                if (servoarmData.pulseCtr == 299) { // 1 ms
                    moveArm(1);

                }
                if (servoarmData.pulseCtr >= 300) { //20 ms
                    servoarmData.pulseCtr = 0;
                    moveArm(0);
                    servoarmData.stateCtr++;
                }

                if (servoarmData.stateCtr == 100) {
                    servoarmData.stateCtr = 0;
                    servoarmData.state = SERVOARM_STATE_HALT;
                }
                break;
            }
            case SERVOARM_STATE_REVERSE:
            {

                if (servoarmData.pulseCtr == 275) { // 1 ms
                    moveArm(1);

                }
                if (servoarmData.pulseCtr >= 300) { //20 ms
                    servoarmData.pulseCtr = 0;
                    moveArm(0);
                    servoarmData.stateCtr++;
                }

                if (servoarmData.stateCtr == 100) {
                    servoarmData.stateCtr = 0;
                    servoarmData.state = SERVOARM_STATE_HALT;
                }
                break;
            }
            
            case SERVOARM_STATE_HALT:
            {

                if (servoarmData.pulseCtr == 285) { //1.5 ms
                    moveArm(1);
//                    servoarmData.stateCtr++;
                    
                }
                if (servoarmData.pulseCtr >= 300) { //20 ms
                    servoarmData.pulseCtr = 0;
                    moveArm(0);
//                    servoarmData.stateCtr=0;
//                    if(servoarmData.moveIndex > servoarmData.curMove){
//                        if(servoarmData.nextMoves[servoarmData.curMove] == FORWARD){
//                            servoarmData.state = SERVOARM_STATE_FORWARD;
//                        }
//                        if(servoarmData.nextMoves[servoarmData.curMove] == BACKWARD){
//                            servoarmData.state = SERVOARM_STATE_REVERSE;
//                        }
//                        servoarmData.curMove++;
//                    }
//                    servoarmData.stateCtr++;
                }
                
//                if(servoarmData.stateCtr == 100){
//                    servoarmData.stateCtr = 0;
//                    servoarmData.state = SERVOARM_STATE_FORWARD;
//                }
                break;
            }

            default:
            {
                /* TODO: Handle error in application's state machine. */
                break;
            }
        }
        
    }
    /* Check the application's current state. */
    
}
int toggleArm = 0;
void moveArm(int dir){
    
    if(dir==0){
        SYS_PORTS_PinWrite (PORTS_ID_0, PORT_CHANNEL_E, PORTS_BIT_POS_9, 0);
    }
    else {
        SYS_PORTS_PinWrite (PORTS_ID_0, PORT_CHANNEL_E, PORTS_BIT_POS_9, 1);
    }
    
//    if (toggleArm == 1) {
//        toggleArm = 0;
//    } else {
//        toggleArm = 1;
//    }
    
}

void sendServoArmQ(unsigned int servoVal) {
    
    BaseType_t xHigherPriorityTaskWoken;
    
    xHigherPriorityTaskWoken = pdFALSE;
    
    if (xQueueSendFromISR(servoarmData.servoarmQ, &servoVal, &xHigherPriorityTaskWoken)!=pdTRUE) {
        dbgSysHalt(DLOC_MSG_Q_FULL);
    }      
    
    portEND_SWITCHING_ISR(xHigherPriorityTaskWoken);
}

void turnUp(){
//    servoarmData.pulseCtr++;
    sendServoArmQ(1);
}

//void moveArmForward(){
////    servoarmData.state = SERVOARM_STATE_FORWARD;
//    servoarmData.nextMoves[servoarmData.moveIndex] = FORWARD;
//    servoarmData.moveIndex++;
//}
//
//void moveArmBackward(){
//    servoarmData.state = SERVOARM_STATE_REVERSE;
//}
 

/*******************************************************************************
 End of File
 */
