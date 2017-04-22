/*******************************************************************************
  MPLAB Harmony Application Source File
  
  Company:
    Microchip Technology Inc.
  
  File Name:
    tapesensor.c

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

#include "tapesensor.h"

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

static TAPESENSOR_DATA tapesensorData;

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
    void TAPESENSOR_Initialize ( void )

  Remarks:
    See prototype in tapesensor.h.
 */

void TAPESENSOR_Initialize ( void )
{
    /* Place the App state machine in its initial state. */
    tapesensorData.state = TAPESENSOR_STATE_INIT;

    tapesensorData.tapesensorQ = xQueueCreate(5, sizeof(unsigned int));
    if(tapesensorData.tapesensorQ == NULL) {
        dbgSysHalt(DLOC_MSG_Q_SETUP_FAIL);
    }

    
    /* TODO: Initialize your application's state machine and other
     * parameters.
     */
}


/******************************************************************************
  Function:
    void TAPESENSOR_Tasks ( void )

  Remarks:
    See prototype in tapesensor.h.
 */

void TAPESENSOR_Tasks ( void )
{

    unsigned int recvTapeVal = 0;
    int secondCtr = 0;
    unsigned char sendFromTapeSensor[100] = "Sent from Tape Sensor";
    /* Check the application's current state. */
    while (1){
        
        if(tapesensorData.tapesensorQ != 0) {
            // receive message on the q.  blocks indefinitely.

            /*Debug: Output location*/
//            dbgOutputLoc(DLOC_STATE_MSG_Q_WAIT_BEFORE_RECEIVE_MSG);
            if(xQueueReceive(tapesensorData.tapesensorQ, &(recvTapeVal), portMAX_DELAY)) {
                secondCtr++;
                if (recvTapeVal == MAP_DATA) {
                    tapesensorData.state = TAPESENSOR_STATE_FOLLOW_PATH;
                }
//                makeMove();
//                resetMapDataGlobalVariables();
//                resetAStartGlobalVariables();
            }
            else {
                dbgSysHalt(DLOC_MSG_Q_RX_FAIL);
            }
            /*Debug: Output location*/
//            dbgOutputLoc(DLOC_STATE_MSG_Q_WAIT_AFTER_RECEIVE_MSG);
        }
        
        switch (tapesensorData.state) {
        /* Application's initial state. */
            case TAPESENSOR_STATE_INIT:
            {                        
                bool appInitialized = true;

                if (appInitialized) {
                    tapesensorData.state = TAPESENSOR_STATE_SERVICE_TASKS;
                }
                break;
            }

            case TAPESENSOR_STATE_SERVICE_TASKS:
            {
//                makeMove();
//                resetMapDataGlobalVariables();
//                resetAStartGlobalVariables();
                break;
            }
            
            case TAPESENSOR_STATE_FOLLOW_PATH:
            {
                makeMove();
                resetMapDataGlobalVariables();
                resetAStartGlobalVariables();
                break;
            }
            
            default:
            {
                tapesensorData.state = TAPESENSOR_STATE_SERVICE_TASKS;
                break;
            }
            
        }
    }

}


void sendTapeSensorQ(unsigned int tapeVal) {
    
    BaseType_t xHigherPriorityTaskWoken;
    
    xHigherPriorityTaskWoken = pdFALSE;
    
    if (xQueueSendFromISR(tapesensorData.tapesensorQ, &tapeVal, &xHigherPriorityTaskWoken)!=pdTRUE) {
        dbgSysHalt(DLOC_MSG_Q_FULL);
    }      
    
    portEND_SWITCHING_ISR(xHigherPriorityTaskWoken);
}

 

/*******************************************************************************
 End of File
 */
