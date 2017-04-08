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

bool rgbRead1(){
    Read_RED1(&buffer_red1);
    Read_GREEN1(&buffer_red1);
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
    Read_GREEN2(&buffer_red2);
    Read_BLUE2(&buffer_blue2);
	if((double)buffer_blue2 / (buffer_red2 + buffer_green2) > 0.7){
		return true;
	}
	else {
		return false;
	}
}

void sendSensorStatus() {
    if (blueAppears1 && !blueAppears2) {
        sendColorSensorData(0);
    }
    
    if (blueAppears2 && !blueAppears1) {
        sendColorSensorData(1);
    }
    
    if (blueAppears1 && blueAppears2) {
        sendColorSensorData(2);
    }
}

//void sendSensorStatus() {
////    char result[20];
////    char b[5], r[5], g[5];
////    sprintf(b, "%d", buffer_blue);
////    sprintf(r, "%d", buffer_red);
////    sprintf(g, "%d", buffer_green);
////    strcat(result, "blue:");
////    strcat(result, b);
////    strcat(result, ", ");
////    strcat(result, "red:");
////    strcat(result, r);
////    strcat(result, ", ");
////    strcat(result, "green:");
////    strcat(result, g);
////    dbgSendMsgServer(result);
////    char ratio[50];
////    double r;
////    r = (double)buffer_blue / (buffer_red + buffer_green);
////    snprintf(ratio, 10, "%f", r);
////    dbgSendMsgServer(ratio);
//}

void readData() {
    // Read data and determine if blue appears
    blueAppears1 = rgbRead1();
    blueAppears2 = rgbRead2();
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
    unsigned int recvVal;
    unsigned int secondCtr = 0;
    
    while (1) {
        if (colorsensorData.colorsensorQ != 0) {
            // receive message on the q.  blocks indefinitely.
            if (xQueueReceive(colorsensorData.colorsensorQ, &(recvVal), portMAX_DELAY)) {
                secondCtr++;        
            } 
            else {
                dbgSysHalt(DLOC_MSG_Q_RX_FAIL);
            }
        }
        
        if (secondCtr >= 1) {
            secondCtr = 0;
            readData();
        }

        if (colorsensorData.dataReady) {
            sendSensorStatus();
            colorsensorData.dataReady = false;
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
