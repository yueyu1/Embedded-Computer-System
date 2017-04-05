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

APP_DATA appData;

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
char* strndup_custom (const char *json, jsmntok_t *tok)
{
    char dest[12];
    int i;
    for (i = 0; i < 12; i++) {
        dest[i] = ' ';
    }
    int len = tok->end - tok->start;
    strncpy(dest, json + tok->start, len);
	return dest;
}

char *jsonStr = "{\"To\": \"toField\", \"From\": \"fromField\", \"SeqNum\": \"seqNum\", \"Payload\": \"payload\"}";

int jsoneq(const char *json, jsmntok_t *tok, const char *s) {
    int expected_len = tok->end - tok->start;
    char* expected_str = json + tok->start;
	if (tok->type == JSMN_STRING && (int) strlen(s) == expected_len &&
			strncmp(expected_str, s, expected_len) == 0) {
		return 0;
	}
	return -1;
}

char* parse_to(char *JSON_STRING) {
    int i;
	int r;
	jsmn_parser p;
	jsmntok_t t[128]; /* We expect no more than 128 tokens */
    
    char *token;
    
    jsmn_init(&p);
	r = jsmn_parse(&p, JSON_STRING, strlen(JSON_STRING), t, sizeof(t)/sizeof(t[0]));
    
    for (i = 1; i < r; i++) {
		if (jsoneq(JSON_STRING, &t[i], "To") == 0) {
            //appData.jsonToSend.to = strndup_custom(JSON_STRING, &t[i+1]);
            token = strndup_custom(JSON_STRING, &t[i+1]);
            i++;
		}
    }
    return token;
}

char* parse_from(char *JSON_STRING) {
    int i;
	int r;
	jsmn_parser p;
	jsmntok_t t[128]; /* We expect no more than 128 tokens */
    
    char *token;
    
    jsmn_init(&p);
	r = jsmn_parse(&p, JSON_STRING, strlen(JSON_STRING), t, sizeof(t)/sizeof(t[0]));
    
    for (i = 1; i < r; i++) {
		if (jsoneq(JSON_STRING, &t[i], "From") == 0) {
            //appData.jsonToSend.to = strndup_custom(JSON_STRING, &t[i+1]);
            token = strndup_custom(JSON_STRING, &t[i+1]);
            i++;
		}
    }
    return token;
}

char* parse_seq(char *JSON_STRING) {
    int i;
	int r;
	jsmn_parser p;
	jsmntok_t t[128]; /* We expect no more than 128 tokens */
    
    char *token;
    
    jsmn_init(&p);
	r = jsmn_parse(&p, JSON_STRING, strlen(JSON_STRING), t, sizeof(t)/sizeof(t[0]));
    
    for (i = 1; i < r; i++) {
		if (jsoneq(JSON_STRING, &t[i], "SeqNum") == 0) {
            //appData.jsonToSend.to = strndup_custom(JSON_STRING, &t[i+1]);
            token = strndup_custom(JSON_STRING, &t[i+1]);
            i++;
		}
    }
    return token;
}

char* parse_payload(char *JSON_STRING) {
    int i;
	int r;
	jsmn_parser p;
	jsmntok_t t[128]; /* We expect no more than 128 tokens */
    
    char *token;
    
    jsmn_init(&p);
	r = jsmn_parse(&p, JSON_STRING, strlen(JSON_STRING), t, sizeof(t)/sizeof(t[0]));
    
    for (i = 1; i < r; i++) {
		if (jsoneq(JSON_STRING, &t[i], "Payload") == 0) {
            //appData.jsonToSend.to = strndup_custom(JSON_STRING, &t[i+1]);
            token = strndup_custom(JSON_STRING, &t[i+1]);
            i++;
		}
    }
    return token;
}

char* parse_json(char *JSON_STRING){
    int i;
	int r;
	jsmn_parser p;
	jsmntok_t t[128]; /* We expect no more than 128 tokens */
    
    char *token;
    
    jsmn_init(&p);
	r = jsmn_parse(&p, JSON_STRING, strlen(JSON_STRING), t, sizeof(t)/sizeof(t[0]));
    
    for (i = 1; i < r; i++) {
		if (jsoneq(JSON_STRING, &t[i], "To") == 0) {
            //appData.jsonToSend.to = strndup_custom(JSON_STRING, &t[i+1]);
            token = strndup_custom(JSON_STRING, &t[i+1]);
            i++;
		}
        
        else if (jsoneq(JSON_STRING, &t[i], "From") == 0) {
            //appData.jsonToSend.from = strndup_custom(JSON_STRING, &t[i+1]);
            token = strndup_custom(JSON_STRING, &t[i+1]);
            i++;
		}
        
        /*else if (jsoneq(JSON_STRING, &t[i], "SeqNum") == 0) {
            token = strndup_custom(JSON_STRING, &t[i+1]);
            //appData.jsonToSend.sequence = atoi(seqStr);
            i++;
        }*/
        
        /*else if (jsoneq(JSON_STRING, &t[i], "Payload") == 0) {
            //appData.jsonToSend.payload = strndup_custom(JSON_STRING, &t[i+1]);
            token = strndup_custom(JSON_STRING, &t[i+1]);
            i++;
        }*/
        
        else {
            // Keys other than the four specified above
        }
    }
    return token;
}

void APP_Initialize ( void )
{
    /*Debug: Output location*/
    //dbgOutputLoc(DLOC_INIT_BEGIN);
    
    // initialize "Team 6" message
    appData.teamString = "Team 6";
    appData.sendToUARTString = "{\"To\": \"toField\", \"From\": \"fromField\", \"SeqNum\": \"seqNum\", \"Payload\": \"payload\"}";
    
    // initialize json struct
    appData.jsonToSend.to = " ";
    appData.jsonToSend.from = " ";
    appData.jsonToSend.sequence = 0;
    appData.jsonToSend.payload = " ";
    
    // initialize time vars to be 0
    appData.elapsedTime = 0;
    appData.timeMsg = 0;
    
    // initialize display char index to be 0
    appData.dispCharIndx = 0;
    
    appData.sendReceive = false;
    
    // setup message queue
    // message queue that can hold 5 unsigned integers
    appData.timeMsgQ = xQueueCreate(5, sizeof(unsigned int));
    if(appData.timeMsgQ == NULL) {
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
    //dbgOutputLoc(DLOC_TASK_ENTER);
    
    // initialize interrupts for Timer 2
    PLIB_INT_SourceEnable(INT_ID_0, INT_SOURCE_TIMER_2);
    
    // start Timer2
    DRV_TMR0_Start();            

    // initial dbgOutputVal call - assert all pins
    dbgOutputVal(0xFF);

    // initial dbgUARTVal call - transmit an exclamation point
   // dbgUARTVal('!');

    // set display char to be the first char in string
    appData.displayChar = appData.teamString[0];
    
    // Debug: Output Location
    //dbgOutputLoc(DLOC_PRE_WHILE_LOOP);
    
    receiveBytes = false;
    appData.delimitCounter = 0;
    ex_msg.to = TAGGERROVER;
    ex_msg.from = SENSORROVER;
    ex_msg.sequence = 2;
    ex_msg.payload = "hello";
    
    while(1){

        if(!appData.sendReceive){
                appData.sendReceive = true;
                dbgSendMsgServer(appData.sendToUARTString);
                
            }
        
        if(appData.timeMsgQ != 0) {
            // receive message on the q.  blocks indefinitely.

            /*Debug: Output location*/
            //dbgOutputLoc(DLOC_STATE_MSG_Q_WAIT_BEFORE_RECEIVE_MSG);
            if(xQueueReceive(appData.timeMsgQ, &(appData.timeMsg), portMAX_DELAY)) {
                // timeMsg now points to a variable holding the front of q
                appData.elapsedTime += appData.timeMsg;
            }
            else {
                dbgSysHalt(DLOC_MSG_Q_RX_FAIL);
            }

            /*Debug: Output location*/
            //dbgOutputLoc(DLOC_STATE_MSG_Q_WAIT_AFTER_RECEIVE_MSG);
        }
        // check to make sure 100 ms have passed
        if(appData.timeMsg == 100) {
            // set appData.displayChar to correct character
            appData.displayChar = appData.teamString[appData.dispCharIndx];

            // write appData.displayChar to debug ports
       //     dbgOutputVal(appData.displayChar);
            
            // increment and check the display character index
            appData.dispCharIndx += 1;
            if(appData.dispCharIndx >= TEAM_SIX_STRING_LENGTH) {
                appData.dispCharIndx = 0;
            }
        }
    }
}

void doneReceiving(char rx){
    //dbgOutputVal(rx);
    //dbgOutputLoc(rx);
    if(rx == SERVER_MSG_DELIMITER){
        appData.delimitCounter++;
    }
    if(appData.delimitCounter == 3){
        appData.delimitCounter = 0;
        //sendMessage(TAGGERROVER, SENSORROVER, 3, "Wifly #1");
        
        
        // Extract To
        char *to = parse_to(jsonStr);
        appData.sendToUARTString = to;
        appData.sendReceive = false;
        
        /*
        // Extract From
        char *from = parse_from(jsonStr);
        appData.sendToUARTString = from;
        appData.sendReceive = false;
        
        // Extract SeqNum
        char *seq = parse_seq(jsonStr);
        appData.sendToUARTString = seq;
        appData.sendReceive = false;
         
        
        // Extract Payload
        char *payload = parse_payload(jsonStr);
        appData.sendToUARTString = payload;
        appData.sendReceive = false; */
        
    //    processMessage(msg);
        
       // processMessage(msg);
    }
}

void sendMessage(char *to, char *from, int sequence, char* payload){
    message example;
    example.to = to;
    example.from = from;
    example.sequence = sequence;
    example.payload = payload;
    processMessage(example);
}

void processMessage(message exMsg){

    if(exMsg.to == TAGGERROVER){

        appData.sendReceive = false;
        exMsg.to = exMsg.from;
        exMsg.from = TAGGERROVER;
        exMsg.sequence = exMsg.sequence + 1;

    }
    
    appData.sendToUARTString = constructJson(exMsg);
    appData.sendReceive = false;
}

char * constructJson(message exMsg){
    int len = strlen(exMsg.to) + strlen(exMsg.from) + strlen(exMsg.payload);
    char str[len+100];
    char seqStr[5];
    sprintf(seqStr, "%d", exMsg.sequence);
    strcpy(str,"{\"");
    strcat(str,"to");
    strcat(str,"\":\"");
    strcat(str,exMsg.to);
    strcat(str,"\",\"");
    strcat(str,"from");
    strcat(str,"\":\"");
    strcat(str,exMsg.from);
    strcat(str,"\",\"");
    strcat(str,"sequence");
    strcat(str,"\":\"");
    strcat(str, seqStr);
    strcat(str,"\",\"");
    strcat(str,"payload");
    strcat(str,"\":\"");
    strcat(str,exMsg.payload);
    strcat(str,"\"}");
    return str;

}

int appSendTimerValToQ(unsigned int msecElapsed) {
    BaseType_t xHigherPriorityTaskWoken;
    
    xHigherPriorityTaskWoken = pdFALSE;
    
    /*Debug: Output location*/
    //dbgOutputLoc(DLOC_ISR_BEFORE_SEND_MSG);
    
    if(xQueueSendFromISR(appData.timeMsgQ, &msecElapsed, &xHigherPriorityTaskWoken) == pdTRUE) {
        // message was sent
        /*Debug: Output location*/
        //dbgOutputLoc(DLOC_ISR_AFTER_SEND_MSG);
        return 0;
    }
    else {
        // queue was full
        /*Debug: Output location*/
        //dbgOutputLoc(DLOC_ISR_AFTER_SEND_MSG);
        return 1;
    }
}

 

/*******************************************************************************
 End of File
 */
