/*******************************************************************************
  MPLAB Harmony Application Source File
  
  Company:
    Microchip Technology Inc.
  
  File Name:
    colorsensor.c

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

#include "colorsensor.h"

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

COLORSENSOR_DATA colorsensorData;

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
    void COLORSENSOR_Initialize ( void )

  Remarks:
    See prototype in colorsensor.h.
 */

static uint16_t INTEGRATION_TIME = 0xF681;
static uint16_t GAIN = 0x008F;
static uint16_t SET_PON = 0x0180;
static uint16_t SET_AEN = 0x0380;

static uint8_t RED = 0x96;
static uint8_t GREEN = 0x98;
static uint8_t BLUE = 0x9A;

static bool blueAppears1;
static uint16_t buffer_red1;
static uint16_t buffer_green1;
static uint16_t buffer_blue1;

static bool blueAppears2;
static uint16_t buffer_red2;
static uint16_t buffer_green2;
static uint16_t buffer_blue2;

static bool redAppears3;
static uint16_t buffer_red3;
static uint16_t buffer_green3;
static uint16_t buffer_blue3;

static unsigned int counter;
static bool finishedCounting;
static unsigned int bothHitCount;
static bool finishedOrientation;

void RGB_Init1() {
    DRV_I2C0_Transmit(ADDRESS_WRITE, &INTEGRATION_TIME, 2, NULL);
    DRV_I2C0_Transmit(ADDRESS_WRITE, &GAIN, 2, NULL);
    DRV_I2C0_Transmit(ADDRESS_WRITE, &SET_PON, 2, NULL);
    DRV_I2C0_Transmit(ADDRESS_WRITE, &SET_AEN, 2, NULL); 
}

void RGB_Init2() {
    DRV_I2C1_Transmit(ADDRESS_WRITE, &INTEGRATION_TIME, 2, NULL);
    DRV_I2C1_Transmit(ADDRESS_WRITE, &GAIN, 2, NULL);
    DRV_I2C1_Transmit(ADDRESS_WRITE, &SET_PON, 2, NULL);
    DRV_I2C1_Transmit(ADDRESS_WRITE, &SET_AEN, 2, NULL); 
}

void RGB_Init3() {
    DRV_I2C2_Transmit(ADDRESS_WRITE, &INTEGRATION_TIME, 2, NULL);
    DRV_I2C2_Transmit(ADDRESS_WRITE, &GAIN, 2, NULL);
    DRV_I2C2_Transmit(ADDRESS_WRITE, &SET_PON, 2, NULL);
    DRV_I2C2_Transmit(ADDRESS_WRITE, &SET_AEN, 2, NULL); 
}

void Read_RED1(void * buffer) {
    DRV_I2C0_Transmit(ADDRESS_WRITE, &RED, 2, NULL);
    DRV_I2C0_Receive(ADDRESS_READ, buffer, 2, NULL);
}

void Read_GREEN1(void * buffer) {
    DRV_I2C0_Transmit(ADDRESS_WRITE, &GREEN, 2, NULL);
    DRV_I2C0_Receive(ADDRESS_READ, buffer, 2, NULL);
}

void Read_BLUE1(void * buffer) {
    DRV_I2C0_Transmit(ADDRESS_WRITE, &BLUE, 2, NULL);
    DRV_I2C0_Receive(ADDRESS_READ, buffer, 2, NULL);
}

void Read_RED2(void * buffer) {
    DRV_I2C1_Transmit(ADDRESS_WRITE, &RED, 2, NULL);
    DRV_I2C1_Receive(ADDRESS_READ, buffer, 2, NULL);
}

void Read_GREEN2(void * buffer) {
    DRV_I2C1_Transmit(ADDRESS_WRITE, &GREEN, 2, NULL);
    DRV_I2C1_Receive(ADDRESS_READ, buffer, 2, NULL);
}

void Read_BLUE2(void * buffer) {
    DRV_I2C1_Transmit(ADDRESS_WRITE, &BLUE, 2, NULL);
    DRV_I2C1_Receive(ADDRESS_READ, buffer, 2, NULL);
}

void Read_RED3(void * buffer) {
    DRV_I2C2_Transmit(ADDRESS_WRITE, &RED, 2, NULL);
    DRV_I2C2_Receive(ADDRESS_READ, buffer, 2, NULL);
}

void Read_GREEN3(void * buffer) {
    DRV_I2C2_Transmit(ADDRESS_WRITE, &GREEN, 2, NULL);
    DRV_I2C2_Receive(ADDRESS_READ, buffer, 2, NULL);
}

void Read_BLUE3(void * buffer) {
    DRV_I2C2_Transmit(ADDRESS_WRITE, &BLUE, 2, NULL);
    DRV_I2C2_Receive(ADDRESS_READ, buffer, 2, NULL);
}

bool rgbRead1(){
    Read_RED1(&buffer_red1);
    Read_GREEN1(&buffer_green1);
    Read_BLUE1(&buffer_blue1);
	if((double)buffer_blue1 / (buffer_red1 + buffer_green1) > 0.7){
		return true;
	}
	else {
		return false;
	}
}

bool rgbRead2(){
    Read_RED2(&buffer_red2);
    Read_GREEN2(&buffer_green2);
    Read_BLUE2(&buffer_blue2);
	if((double)buffer_blue2 / (buffer_red2 + buffer_green2) > 0.7){
		return true;
	}
	else {
		return false;
	}
}

bool rgbRead3(){
    Read_RED3(&buffer_red3);
    Read_GREEN3(&buffer_green3);
    Read_BLUE3(&buffer_blue3);
	if((double)buffer_red3 / (buffer_blue3 + buffer_green3) > 0.7){
		return true;
	}
	else {
		return false;
	}
}

void sendSensorStatus() {
    char result[20];
    char b[5], r[5], g[5];
    sprintf(b, "%d", buffer_blue3);
    sprintf(r, "%d", buffer_red3);
    sprintf(g, "%d", buffer_green3);
    strcat(result, "blue:");
    strcat(result, b);
    strcat(result, ", ");
    strcat(result, "red:");
    strcat(result, r);
    strcat(result, ", ");
    strcat(result, "green:");
    strcat(result, g);
    dbgSendMsgServer(result);
    char ratio[50];
    double ra;
    ra = (double)buffer_red3 / (buffer_blue3 + buffer_green3);
    snprintf(ratio, 10, "%f", r);
    dbgSendMsgServer(ratio);
}

void readData() {
    // Read data and determine if blue appears
    blueAppears1 = rgbRead1();
    blueAppears2 = rgbRead2();
    redAppears3 = rgbRead3();
    // Make data ready
    colorsensorData.dataReady = true;
}

//void readData() {
//    Read_BLUE1(&buffer_blue);
//    Read_RED1(&buffer_red);
//    Read_GREEN1(&buffer_green);
//    colorsensorData.dataReady = true;
//}


void COLORSENSOR_Initialize ( void )
{
    /* Place the App state machine in its initial state. */
    colorsensorData.state = COLORSENSOR_STATE_INIT;

    /* TODO: Initialize your application's state machine and other
     * parameters.
     */
    colorsensorData.dataReady = false;
    colorsensorData.colorsensorQ = xQueueCreate(5, sizeof(unsigned int));
    counter = 0;
    bothHitCount = 0;
    finishedCounting = false;
    finishedOrientation = false;
}


/******************************************************************************
  Function:
    void COLORSENSOR_Tasks ( void )

  Remarks:
    See prototype in colorsensor.h.
 */

void COLORSENSOR_Tasks ( void )
{
    RGB_Init1();
    RGB_Init2();
    RGB_Init3();
    unsigned int recvVal;
    unsigned int secondCtr = 0;
    bool gameStarted = false;
    controlLED(0);
    
    while (1) {
        if (colorsensorData.colorsensorQ != 0) {
            // receive message on the q.  blocks indefinitely.
            if (xQueueReceive(colorsensorData.colorsensorQ, &(recvVal), portMAX_DELAY)) {
                if(recvVal == START_GAME) {
                    gameStarted = true;
                }
                else {
                    secondCtr++;
                }
            } 
            else {
                dbgSysHalt(DLOC_MSG_Q_RX_FAIL);
            }
        }
        
        if (secondCtr > 0) {
            secondCtr = 0;
            readData();
        }
        
        
        if (bothHitCount >= 10) {
            motorControlSendValToMsgQ(MOTOR_CONTROL_HALT);
            finishedOrientation = true;
            sendTapeSensorQ(FINISHED_ORIENTATION);
        }
        
        /* Check the application's current state. */
        switch (colorsensorData.state) {
        /* Application's initial state. */
            case COLORSENSOR_STATE_INIT:
            {
                motorControlSendValToMsgQ(MOTOR_CONTROL_HALT);
                        
                bool appInitialized = true;

                if (appInitialized) {

                    colorsensorData.state = COLORSENSOR_STATE_SERVICE_TASKS;
                }
                break;
            }

            case COLORSENSOR_STATE_SERVICE_TASKS:
            {
                if (colorsensorData.dataReady) {
                    colorsensorData.dataReady = false;
                    
                    //sendSensorStatus();

                    if (counter > 2) {
                        finishedCounting = true;
                        
                        if (!finishedOrientation && gameStarted) {
                            
                            if (!blueAppears1 && !blueAppears2) {
                                colorsensorData.state = COLORSENSOR_STATE_BOTH_NOT_HIT;
                            }

                            if (blueAppears1 && !blueAppears2) {
                                colorsensorData.state = COLORSENSOR_STATE_LEFT_HIT;
                            }

                            if (blueAppears2 && !blueAppears1) {
                                colorsensorData.state = COLORSENSOR_STATE_RIGHT_HIT;
                            }

                            if (blueAppears1 && blueAppears2) {
                                colorsensorData.state = COLORSENSOR_STATE_BOTH_HIT;
                            }
                        }
                    }

                    if (!finishedCounting) {
                        counter++;
                    }
                    
                    if(finishedOrientation) {
                        if(redAppears3) {
                            controlLED(1);
//                            sendSensorStatus();
                        }
                        else {
                            controlLED(0);
                        }
                    }
                }
                break;
            }
            
            case COLORSENSOR_STATE_LEFT_HIT:
            {
                motorControlSendValToMsgQ(MOTOR_CONTROL_LEFT);
                colorsensorData.state = COLORSENSOR_STATE_SERVICE_TASKS;
                break;
            }
            
            case COLORSENSOR_STATE_RIGHT_HIT:
            {
                motorControlSendValToMsgQ(MOTOR_CONTROL_RIGHT);
                colorsensorData.state = COLORSENSOR_STATE_SERVICE_TASKS;
                break;
            }
            
            case COLORSENSOR_STATE_BOTH_HIT:
            {
                bothHitCount++;
                motorControlSendValToMsgQ(MOTOR_CONTROL_REVERSE);              
                colorsensorData.state = COLORSENSOR_STATE_SERVICE_TASKS;
                break;
            }
            
            case COLORSENSOR_STATE_BOTH_NOT_HIT:
            {
                motorControlSendValToMsgQ(MOTOR_CONTROL_FORWARD);
                colorsensorData.state = COLORSENSOR_STATE_SERVICE_TASKS;
                break;
            }
        }
    }
}
   

void sendColorSensorQ(unsigned int val) {
    
    BaseType_t xHigherPriorityTaskWoken;
    
    xHigherPriorityTaskWoken = pdFALSE;
    
    if (xQueueSendFromISR(colorsensorData.colorsensorQ, &val, &xHigherPriorityTaskWoken)!=pdTRUE) {
        dbgSysHalt(DLOC_MSG_Q_FULL);
    }      
    
    portEND_SWITCHING_ISR(xHigherPriorityTaskWoken);
}

 

/*******************************************************************************
 End of File
 */