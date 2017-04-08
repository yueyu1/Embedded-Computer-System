/*******************************************************************************
  MPLAB Harmony Application Source File
  
  Company:
    Microchip Technology Inc.
  
  File Name:
    motorcontrol.c

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

#include "motorControl.h"


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

static MOTORCONTROL_DATA motorcontrolData;

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
    void MOTORCONTROL_Initialize ( void )

  Remarks:
    See prototype in motorcontrol.h.
 */

void MOTORCONTROL_Initialize ( void )
{ 
    // setup message queue
    // message queue that can hold 5 unsigned integers
    motorcontrolData.motorQ = xQueueCreate(5, sizeof(char));
    if(motorcontrolData.motorQ == NULL) {
        dbgSysHalt(DLOC_MSG_Q_SETUP_FAIL);
    }
}


/******************************************************************************
  Function:
    void MOTORCONTROL_Tasks ( void )

  Remarks:
    See prototype in motorcontrol.h.
 */

void MOTORCONTROL_Tasks ( void )
{
    char requestState;
    char state;
    char encVal;
    while(1){
        if(motorcontrolData.motorQ != 0) {
            // receive message on the q.  blocks indefinitely.

            /*Debug: Output location*/
//            dbgOutputLoc(DLOC_STATE_MSG_Q_WAIT_BEFORE_RECEIVE_MSG);
            if(!xQueueReceive(motorcontrolData.motorQ, &(requestState), portMAX_DELAY)) {
                // timeMsg now points to a variable holding the front of q
                dbgSysHalt(DLOC_MSG_Q_RX_FAIL);
            }
            switch(requestState) {
                case MOTOR_CONTROL_HALT:
                    state = MOTOR_CONTROL_HALT;
                    // TODO: Send an acknowledge message back
                    break;
                case MOTOR_CONTROL_LEFT:
                    state = MOTOR_CONTROL_LEFT;
                    // TODO: Send and acknowledge back
                    break;
                case MOTOR_CONTROL_RIGHT:
                    state = MOTOR_CONTROL_RIGHT;
                    // TODO: Send and acknowledge back
                    break;
                case MOTOR_CONTROL_FORWARD:
                    state = MOTOR_CONTROL_FORWARD;
                    // TODO: Send and acknowledge back
                    break;
                case MOTOR_CONTROL_REVERSE:
                    state = MOTOR_CONTROL_REVERSE;
                    // TODO: Send and acknowledge back
                    break;
                case MOTOR_REQUEST_ENCVAL:
                    dbgOutputLoc('1');
                    encVal = getEncVal();
                    communicationSendToMsgQ(encVal);
                    break;
                default: state = state;
            }
            switch(state) {
                case MOTOR_CONTROL_HALT:
                    stop();
                    break;
                case MOTOR_CONTROL_LEFT:
                    enableLeft();
                    break;
                case MOTOR_CONTROL_RIGHT:
                    enableRight();
                    break;
                case MOTOR_CONTROL_FORWARD:
                    enableReverse();
                    break;
                case MOTOR_CONTROL_REVERSE:
                    enableForward();
                    break;
                default: state = state;
            }
        }
    }
}

char getEncVal() {
    char encVal;
    appSendEncValToQ(MOTOR_REQUEST_ENCVAL);
    if(!xQueueReceive(motorcontrolData.motorQ, &(encVal), portMAX_DELAY)){
        dbgSysHalt(DLOC_MSG_Q_RX_FAIL);
    }
    dbgOutputLoc(encVal);
    return encVal;
}

void enableForward() {
    writeMotorPins(true, false, false);
}

void enableReverse() {
    writeMotorPins(true, true, true);
}
void enableLeft() {
    writeMotorPins(true, false, true);
}
void enableRight() {
    writeMotorPins(true, true, false);
}
void stop() {
    writeMotorPins(false, false, false);
}

void writeMotorPins(bool enable, bool leftTread, bool rightTread) {
    SYS_PORTS_PinWrite (PORTS_ID_0, PORT_CHANNEL_D, PORTS_BIT_POS_0, enable);
    SYS_PORTS_PinWrite (PORTS_ID_0, PORT_CHANNEL_D, PORTS_BIT_POS_5, rightTread);
    SYS_PORTS_PinWrite (PORTS_ID_0, PORT_CHANNEL_F, PORTS_BIT_POS_1, leftTread);
}

void motorControlSendValToMsgQ(char motorMacro) {
    if(xQueueSend(motorcontrolData.motorQ, (void *) &motorMacro, portMAX_DELAY) != pdTRUE) {
        dbgSysHalt(DLOC_MSG_Q_FULL);
    }
}

void motorControlSendValToMsgQFromISR(char macro) {
    BaseType_t xHigherPriorityTaskWoken;
    
    xHigherPriorityTaskWoken = pdFALSE;
    
    if (xQueueSendFromISR(motorcontrolData.motorQ, &macro, &xHigherPriorityTaskWoken)!=pdTRUE) {
        dbgSysHalt(DLOC_MSG_Q_FULL);
    }      
    
    portEND_SWITCHING_ISR(xHigherPriorityTaskWoken);
}
 

/*******************************************************************************
 End of File
 */
