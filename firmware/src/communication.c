#include "communication.h"
#include "communication_public.h"
#include "motorControl_public.h"
#include "utility.h"

static COMMUNICATION_DATA appData;

void COMMUNICATION_Initialize ( void )
{   
    // Initialize global variables
    resetAStartGlobalVariables();
    resetMapDataGlobalVariables();
    
    // initialize "Team 6" message
    appData.teamString = "Team 6";

    //strcpy(appData.sendToUARTString, "{\"To\": \"SERVER\", \"From\": \"FLAG\", \"Sequence\": \"420\", \"Payload\": \"Pathfinding\"}";
    
    // initialize time vars to be 0
    appData.elapsedTime = 0;
    appData.timeMsg = 0;
    
    emptyCharArray(incomingJson, JSON_MSG_MAX_SIZE);
    jsonIndex = 0;
    ORIENTATION = 0;
    FINISHED = 0;
    
    // initialize display char index to be 0
    appData.dispCharIndx = 0;
    appData.sendReceive = true;
    appData.isReceiving = false;
    
    // setup message queue
    appData.commQ = xQueueCreate(JSON_MSG_MAX_SIZE, sizeof(char));
    if(appData.commQ == NULL) {
        dbgSysHalt(DLOC_MSG_Q_SETUP_FAIL);
    }
}

/******************************************************************************
  Function:
    void APP_Tasks ( void )
  Remarks:
    See prototype in app.h.
 */

void COMMUNICATION_Tasks ( void )
{  
    // Set display char to be the first char in string
    appData.displayChar = appData.teamString[0];
    receiveBytes = false;
    appData.delimitCounter = 0;
    
    // The motor should stop initially
    sendTimerValtoPathMovement(STOP);

    char rxChar;
    
    while(1){

        if(!appData.sendReceive){
                appData.sendReceive = true;
                emptyCharArray(incomingJson, JSON_MSG_MAX_SIZE);
                dbgSendMsgServer(appData.sendToUARTString);
                
            }
        
        if(appData.commQ != 0) {
            // receive message on the q.  blocks indefinitely.

            /*Debug: Output location*/
            //dbgOutputLoc(DLOC_STATE_MSG_Q_WAIT_BEFORE_RECEIVE_MSG);
            if(xQueueReceive(appData.commQ, &(rxChar), portMAX_DELAY)==pdFALSE) {
                dbgSysHalt(DLOC_MSG_Q_RX_FAIL);
            }
            doneReceiving(rxChar);
        }
        
    }
}

void doneReceiving(char rx){
    dbgOutputVal(rx);
    dbgOutputLoc(rx);
    if(rx == SERVER_MSG_DELIMITER){
        appData.delimitCounter++; 
    }
    else if(rx == '{' && appData.isReceiving) {
        emptyCharArray(incomingJson, JSON_MSG_MAX_SIZE);
        jsonIndex = 0;
        incomingJson[jsonIndex] = rx;
        ++jsonIndex;
    }
    else if(rx == '{') {
        appData.isReceiving = true;
        incomingJson[jsonIndex] = rx;
        jsonIndex++;
    }
    else
    {
        incomingJson[jsonIndex] = rx;
        jsonIndex++;
    }
    
    if(appData.delimitCounter == 3){
        appData.delimitCounter = 0;
        appData.isReceiving = false;
        jsonIndex = 0;
        //int receivedChecksum = getReceivedChecksum(incomingJson);
        //int receivedChecksum = 0;
        //int actualChecksum = countReceivedChecksum(incomingJson);
//        if (compareChecksum(receivedChecksum, actualChecksum)) {
//            message newMsg = parseJsonMsg(incomingJson);
//            //appData.sendToUARTString = newMsg.payload;
//            //appData.sendReceive = false;
//            processMessage(newMsg);
//        }
        message newMsg = parseJsonMsg(incomingJson);
        transformMapData();
        processMessage(newMsg);
    }
}

void processMessage(message exMsg){
    char reqVal = '\0';
    char valStr[5];
    
    if(strcmp(exMsg.to, ROVERSIGN_THIS) == 0) {
        clear(exMsg.to);
        strcpy(exMsg.to, exMsg.from);
        clear(exMsg.from);
        strcpy(exMsg.from, ROVERSIGN_THIS);
        exMsg.sequence = exMsg.sequence + 1;
        
        // Request for encoder data
        if(strcmp(exMsg.payload, "M_REQ_ENCVAL")==0) {
            motorControlSendValToMsgQ(MOTOR_REQUEST_ENCVAL);
            if(xQueueReceive(appData.commQ, &(reqVal), portMAX_DELAY)==pdFALSE) {
                dbgSysHalt(DLOC_MSG_Q_RX_FAIL);
            }
            sprintf(valStr, "%d", reqVal);
            strcpy(exMsg.payload, valStr);
        }
        // Other request
        else {
            strcpy(exMsg.payload, "RespondPath");
        }
        
        explorePath(startPosition.sx, startPosition.sy, goalPosition.gx, goalPosition.gy);
        resetPathMovementGlobalVariables();
        constructPathJson(appData.sendToUARTString, exMsg);
        //constructNextMovementJson(appData.sendToUARTString, exMsg);
        makeMove();
        resetMapDataGlobalVariables();
        resetAStartGlobalVariables();
        appData.sendReceive = false;
    }
    else {
        emptyCharArray(incomingJson, JSON_MSG_MAX_SIZE);
        clear(appData.sendToUARTString);
    }
}

void communicationSendToMsgQ(char data) {
    if(xQueueSend(appData.commQ, (void *) &data, portMAX_DELAY) != pdTRUE) {
        dbgSysHalt(DLOC_MSG_Q_FULL);
    }
}

void communicationSendToMsgQFromISR(char rxChar) {
    BaseType_t xHigherPriorityTaskWoken;
    
    xHigherPriorityTaskWoken = pdFALSE;
    
    if (xQueueSendFromISR(appData.commQ, &rxChar, &xHigherPriorityTaskWoken)!=pdTRUE) {
        dbgSysHalt(DLOC_MSG_Q_FULL);
    }      
    
    portEND_SWITCHING_ISR(xHigherPriorityTaskWoken);
}

/*******************************************************************************
 End of File
 */
