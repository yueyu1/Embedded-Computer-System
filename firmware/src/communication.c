#include "communication.h"
#include "communication_public.h"
#include "motorControl_public.h"
#include "utility.h"

static COMMUNICATION_DATA appData;

void initVariable() {
    startPosition.sx = 0;
    startPosition.sy = 0;
    goalPosition.gx = 5;
    goalPosition.gy = 5;
    flag.x = 100;
    flag.y = 100;
    
    MIN_X = 0;
    MIN_Y = 0;
    MAX_X = 5;
    MAX_Y = 5;
}

void COMMUNICATION_Initialize ( void )
{   
    // Initialize global variables
    resetAStartGlobalVariables();
    resetMapDataGlobalVariables();
    resetCurMapDataGlobalVariables();
    initVariable();
    explorePath(startPosition.sx, startPosition.sy, goalPosition.gx, goalPosition.gy);
    resetAStartGlobalVariables();
    
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
//    sendTimerValtoPathMovement(MOVE_FORWARD);
    //sendTimerValtoPathMovement(MOVE_FORWARD);
    
//    initVariable();
//    resetPathMovementGlobalVariables();
//    message msg;
//    strcpy(msg.to, "SENSOR");
//    strcpy(msg.from, "FLAG");
//    msg.sequence = 50;
//    strcpy(msg.payload, "Path");
//    explorePath(startPosition.sx, startPosition.sy, goalPosition.gx, goalPosition.gy);
//    constructPathJson(appData.sendToUARTString, msg);
//    //makeMove();
//    appData.sendReceive = false;
    

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
        else if (strcmp(exMsg.payload, "Map") == 0) {
            updateMap();
            transformNewMapData();
//            transformMapData();
            strcpy(exMsg.payload, "RespondPath");
        }
        else if (strcmp(exMsg.payload, "Boundary") == 0) {
            transformBoundaryData();
            strcpy(exMsg.payload, "RespondBoundary");
        }
        else if(strcmp(exMsg.payload, "Flag") == 0) {
            strcpy(exMsg.payload, "RespondFlag");
            sendTapeSensorQ(FLAG_ZONE);
        }
        else if(strcmp(exMsg.payload, "STARTGAME") == 0) {
            strcpy(exMsg.payload, "Flag rover started");
            sendColorSensorQ(START_GAME);
        }
//        transformMapData();
        explorePath(startPosition.sx, startPosition.sy, goalPosition.gx, goalPosition.gy);
//        resetPathMovementGlobalVariables();
        constructPathJson(appData.sendToUARTString, exMsg);
        sendTapeSensorQ(MAP_DATA);
        appData.sendReceive = false;
    }
    else {
        emptyCharArray(incomingJson, JSON_MSG_MAX_SIZE);
        clear(appData.sendToUARTString);
    }
}

void sendResponse(char* response) {
    strcpy(appData.sendToUARTString, response);
    appData.sendReceive = false;
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

void sendTapeSensorData(int direction) {
//    if(direction ==0){
//        dbgSendMsgServer("Tape indicated from left");
//    }
//    if (direction == 1){
//        dbgSendMsgServer("Tape indicated from right");
//    }
//    if (direction == 2){
//        dbgSendMsgServer("Tape indicated from both direction");
//    }
}

void sendColorSensorData(int appearVal) {
    if (appearVal == 0) {
        dbgSendMsgServer("1 Detected!");
    }
    if (appearVal == 1) {
        dbgSendMsgServer("2 Detected!");
    }
    if (appearVal == 2) {
        dbgSendMsgServer("Both Detected!");
    }
}


/*******************************************************************************
 End of File
 */
