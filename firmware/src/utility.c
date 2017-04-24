#include "utility.h"

/************** Utility functions*************/
int getStrLen(char* str) {
    int i = 0;
    while (str[i] != '\0') {
        i++;
    }
    return i;
}

void clear(char *str) {
    int i;
    for(i=0; i < strlen(str); ++i) {
        str[i] = '\0';
    }
}

char* strndup_custom (const char *json, jsmntok_t *tok, char *dest)
{
    int len = tok->end - tok->start;
    strncpy(dest, json + tok->start, len);
	return dest;
}

void emptyCharArray(char *arr, int size) {
    int i;
    for (i = 0; i < size; i++) {
        arr[i] = '\0';
    }
}

void emptyIntArray(int *arr, int size) {
    int i;
    for (i = 0; i < size; i++) {
        arr[i] = NULL;
    }
}

void copyNonNull(char *dest, char *source, int size_dest) {
    int size = getSendingChecksum(source);
    int i;
    for (i = 0; i < size; i++) {
        dest[i] = source[i];
    }
    if (size < size_dest) {
        i++;
        while (i < size_dest) {
            dest[i] = '\0';
        }
    }
}

void cleanUpTail(char *msg) {
    int i = 0;
    while (msg[i] != '\0') {
        i++;
    }
    while (i < JSON_VAL_MAX_SIZE) {
        msg[i] = '\0';
        i++;
    }
}

bool pointEqual(coordinate c1, coordinate c2) {
    return (c1.x == c2.x && c1.y == c2.y);
}
/************** end of utility functions*************/





/************** Checksum functions ***************/
int getSendingChecksum(char *arr) {
    int i = 0;
    while (arr[i] != '\0') {
        i++;
    }
    return i;
}

int getReceivedChecksum(char *arr) {
    int i = 0;
    int j =0;
    char result[5];
    while (arr[i] != '}') {
        i++;
    }
    i++;
    while (arr[i] != '\0') {
        result[j] = arr[i];
        i++;
        j++;
    }
    int checkSum = atoi(result);
    
    return checkSum;
}

int countReceivedChecksum(char *arr) {
    int i = 0;
    while (arr[i] != '}') {
        i++;
    }
    i++;
    return i;
}

bool compareChecksum(int check1, int check2) {
    return (check1 == check2);
}
/************** end of checksum functions*************/




/************** JSON functions ***************/
int jsoneq(char *json, jsmntok_t *tok, char *s) {
    int expected_len = tok->end - tok->start;
    char* expected_str = json + tok->start;
	if (tok->type == JSMN_STRING && (int) strlen(s) == expected_len &&
			strncmp(expected_str, s, expected_len) == 0) {
		return 0;
	}
	return -1;
}

message parseJsonMsg(char *jsonStr) {
    
        message newMsg;
        parse(jsonStr, "To", newMsg.to);
        cleanUpTail(newMsg.to);
        parse(jsonStr, "From", newMsg.from);
        cleanUpTail(newMsg.from);
        parse(jsonStr, "Payload", newMsg.payload);
        cleanUpTail(newMsg.payload);
        
        char temp[5];
        parse(jsonStr, "Sequence", temp);
        int seqNum = atoi(temp);
        newMsg.sequence = seqNum;
        
        int IsMapData = strcmp(newMsg.payload, "Map");
        
        if (IsMapData == 0) {
            parseArrayPayload(jsonStr, "Obstacle");
            parseArrayPayload(jsonStr, "Boundary");
            parseArrayPayload(jsonStr, "Start");
            parseArrayPayload(jsonStr, "Goal");
        }
        else if (strcmp(newMsg.payload, "Boundary") == 0){
            parseArrayPayload(jsonStr, "Obstacle");
            parseArrayPayload(jsonStr, "Boundary");
            parseArrayPayload(jsonStr, "Start");
            parseArrayPayload(jsonStr, "Goal");
        }
        return newMsg;
}

void parse(char *JSON_STRING, char *field, char *dest) {
    int i;
	int r;
	jsmn_parser p;
	jsmntok_t t[128]; /* We expect no more than 128 tokens */
    
    emptyCharArray(dest, 20);
    
    jsmn_init(&p);
	r = jsmn_parse(&p, JSON_STRING, strlen(JSON_STRING), t, sizeof(t)/sizeof(t[0]));
    
    for (i = 1; i < r; i++) {
		if (jsoneq(JSON_STRING, &t[i], field) == 0) {
            strndup_custom(JSON_STRING, &t[i+1], dest);
            i++;
        }
    }
}

void parseArrayPayload(char *JSON_STRING, char *field) {
    int i;
	int r;
	jsmn_parser p;
	jsmntok_t t[128]; /* We expect no more than 128 tokens */
    char num[2];
    
    jsmn_init(&p);
	r = jsmn_parse(&p, JSON_STRING, strlen(JSON_STRING), t, sizeof(t)/sizeof(t[0]));
    
    for (i = 1; i < r; i++) {
		if (jsoneq(JSON_STRING, &t[i], field) == 0) {
            if (t[i+1].type == JSMN_ARRAY) {
                int j;
                int toksize = t[i+1].size;
                for (j = 0; j < toksize; j++) {
                    jsmntok_t *g = &t[i+j+2];
                    int len = g->end - g->start;
                    const char* startChar = JSON_STRING + g->start;
                    strncpy(num, startChar, len);
                    int number = atoi(num);
                    storeMapData(field, number);
                    emptyCharArray(num, 2);
                }
                i += t[i+1].size + 1;
            }	
        }
    }
}

void constructPathJson(char *buf, message exMsg) {
    int len = strlen(exMsg.to) + strlen(exMsg.from) + strlen(exMsg.payload);
    char str[len+300];
    char seqStr[5];
    char pointStr[5];
    char traceStr[5];
    sprintf(seqStr, "%d", exMsg.sequence);

    strcpy(str,"{\"");
    strcat(str,"To");
    strcat(str,"\":\"");
    strcat(str,exMsg.to);
    strcat(str,"\", \"");
    strcat(str,"From");
    strcat(str,"\":\"");
    strcat(str,exMsg.from);
    strcat(str,"\", \"");
    strcat(str,"Sequence");
    strcat(str,"\":\"");
    strcat(str, seqStr);
    strcat(str,"\", \"");
    strcat(str,"Payload");
    strcat(str,"\":\"");
    strcat(str,exMsg.payload);
    
    strcat(str,"\", \"");
    strcat(str,"Start");
    strcat(str,"\":\"");
    sprintf(pointStr, "%d,%d", startPosition.sx, startPosition.sy);
    strcat(str,pointStr);
    emptyCharArray(pointStr, 5);
    
    strcat(str,"\", \"");
    strcat(str,"Goal");
    strcat(str,"\":\"");
    sprintf(pointStr, "%d,%d", goalPosition.gx, goalPosition.gy);
    strcat(str,pointStr);
    emptyCharArray(pointStr, 5);
    
    strcat(str,"\", \"");
    strcat(str,"Trace");
    strcat(str,"\":[\"");
    sprintf(traceStr, "%d,%d", startPosition.sx, startPosition.sy);
    strcat(str, traceStr);
    strcat(str,"\", \"");
    emptyCharArray(traceStr, 5);
    
    int ind = traceBackListLen - 1;
    while (ind >= 0) {
        int currx = traceback_list[ind]->x;
        int curry = traceback_list[ind]->y;
        sprintf(traceStr, "%d,%d", currx, curry);
        strcat(str,traceStr);
        emptyCharArray(traceStr, 5);
        if (ind != 0) {
            strcat(str,"\", \"");
        }
        ind--;
    }
    
    strcat(str,"\"]}");
    strcpy(buf, str);
    
    int checksum = getSendingChecksum(str);
    char checksumStr[5];
    sprintf(checksumStr, "%d", checksum);
    
    strcat(str, checksumStr);
    strcpy(buf, str);
}

void constructNextMovementJson(char *buf, message exMsg) {
    int len = strlen(exMsg.to) + strlen(exMsg.from) + strlen(exMsg.payload);
    char str[len+300];
    char seqStr[5];
    char pointStr[5];
    sprintf(seqStr, "%d", exMsg.sequence);

    strcpy(str,"{\"");
    strcat(str,"To");
    strcat(str,"\":\"");
    strcat(str,exMsg.to);
    strcat(str,"\", \"");
    strcat(str,"From");
    strcat(str,"\":\"");
    strcat(str,exMsg.from);
    strcat(str,"\", \"");
    strcat(str,"Sequence");
    strcat(str,"\":\"");
    strcat(str, seqStr);
    strcat(str,"\", \"");
    strcat(str,"Payload");
    strcat(str,"\":\"");
    strcat(str,exMsg.payload);
    
    strcat(str,"\", \"");
    strcat(str,"Start");
    strcat(str,"\":\"");
    sprintf(pointStr, "%d,%d", startPosition.sx, startPosition.sy);
    strcat(str,pointStr);
    emptyCharArray(pointStr, 5);
    
    strcat(str,"\", \"");
    strcat(str,"Goal");
    strcat(str,"\":\"");
    sprintf(pointStr, "%d,%d", goalPosition.gx, goalPosition.gy);
    strcat(str,pointStr);
    emptyCharArray(pointStr, 5);
    
    coordinate c = getNextPoint();
    coordinate start;
    start.x = startPosition.sx;
    start.y = startPosition.sy;
    getNextMovement(start, c);
    
    strcat(str,"\", \"");
    strcat(str,"NextMovement");
    strcat(str,"\":\"");
    strcat(str, movement);
    
    strcat(str,"\"}");
    strcpy(buf, str);
    
    int checksum = getSendingChecksum(str);
    char checksumStr[5];
    sprintf(checksumStr, "%d", checksum);
    
    strcat(str, checksumStr);
    strcpy(buf, str);
}

void constructJson(char *buf, message exMsg){
    int len = strlen(exMsg.to) + strlen(exMsg.from) + strlen(exMsg.payload);
    char str[len+100];
    char seqStr[5];
    sprintf(seqStr, "%d", exMsg.sequence);
    strcpy(str,"{\"");
    strcat(str,"To");
    strcat(str,"\":\"");
    strcat(str,exMsg.to);
    strcat(str,"\",\"");
    strcat(str,"From");
    strcat(str,"\":\"");
    strcat(str,exMsg.from);
    strcat(str,"\",\"");
    strcat(str,"Sequence");
    strcat(str,"\":\"");
    strcat(str, seqStr);
    strcat(str,"\",\"");
    strcat(str,"Payload");
    strcat(str,"\":\"");
    strcat(str,exMsg.payload);
    strcat(str,"\"}");
    strcpy(buf, str);
    
    int checksum = getSendingChecksum(str);
    char checksumStr[5];
    sprintf(checksumStr, "%d", checksum);
    
    strcat(str, checksumStr);
    strcpy(buf, str);
}
/************** end of JSON functions*************/






/************** Map functions ***************/
void storeMapData(char* field, int n) {
    if (strcmp(field, "Obstacle") == 0) {
        ex_map.obstacle[o_length] = n;
        o_length++;
    }
    if (strcmp(field, "Boundary") == 0) {
        ex_map.boundary[boundary_length] = n;
        boundary_length++;
    }
    if (strcmp(field, "Start") == 0) {
        ex_map.startpos[startPos_length] = n;
        startPos_length++;
    }
    
    if (strcmp(field, "Goal") == 0) {
        ex_map.flagpos[flag_length] = n;
        flag_length++;
    }
}

void transformMapData() {
    int i;
    for (i = 0; i < o_length; i++) {
        obsticle_list[obsticleListLen].x = ex_map.obstacle[i];
        i++;
        obsticle_list[obsticleListLen].y = ex_map.obstacle[i];
        obsticleListLen++;
    }
    
    i = 0;
    MIN_X = ex_map.boundary[i];
    i++;
    MIN_Y = ex_map.boundary[i];
    i++;
    MAX_X = ex_map.boundary[i];
    i++;
    MAX_Y = ex_map.boundary[i];
    
    i = 0;
    startPosition.sx = ex_map.startpos[i];
    i++;
    startPosition.sy = ex_map.startpos[i];
    
    i = 0;
    goalPosition.gx = ex_map.flagpos[i];
    i++;
    goalPosition.gy = ex_map.flagpos[i];
}
/************** end of Map functions*************/

void transformBoundaryData() {
    int i;
    i = 0;
    MIN_X = ex_map.boundary[i];
    i++;
    MIN_Y = ex_map.boundary[i];
    i++;
    MAX_X = ex_map.boundary[i];
    i++;
    MAX_Y = ex_map.boundary[i];
    
    i = 0;
    cur_map.boundary[i] = ex_map.boundary[i];
    i++;
    cur_map.boundary[i] = ex_map.boundary[i];
    i++;
    cur_map.boundary[i] = ex_map.boundary[i];
    i++;
    cur_map.boundary[i] = ex_map.boundary[i];
    
    cur_boundary_length+=4;
}





/************** Reset functions ***************/
void resetMapDataGlobalVariables() {
    emptyIntArray(ex_map.boundary, MAP_VAL_MAX_SIZE);
    emptyIntArray(ex_map.flagpos, MAP_VAL_MAX_SIZE);
    emptyIntArray(ex_map.obstacle, MAP_VAL_MAX_SIZE);
    emptyIntArray(ex_map.startpos, MAP_VAL_MAX_SIZE);
    
    o_length = 0;
    flag_length = 0;
    boundary_length = 0;
    startPos_length = 0;
}

void resetAStartGlobalVariables() {
    openListLen = 0;
    closeListLen = 0;
    traceBackListLen = 0;
    nodeListLen = 0;
    obsticleListLen = 0;
    boundaryListLen = 0;
}

void resetPathMovementGlobalVariables() {
    ORIENTATION = 0;
    CURRPOINT.x = startPosition.sx;
    CURRPOINT.y = startPosition.sy;
}
/************** end of Reset functions*************/






/************** Movement functions ***************/
coordinate getNextPoint() {
    coordinate co;
    int index;
    if(traceBackListLen >= 1) {
        index = traceBackListLen - 1;
        co.x = traceback_list[index]->x;
        co.y = traceback_list[index]->y;
        traceBackListLen--;
    }
    return co;
}

void getNextMovement(coordinate orig, coordinate next) {
    if (next.x > orig.x) {
        if (ORIENTATION == 0) {
            strcpy(movement, "FORWARD");
            CURRPOINT.x = next.x;
        }
        else {
            if (ORIENTATION == 1) {
                strcpy(movement, "LEFT");
            }
            if (ORIENTATION == 2) {
                strcpy(movement, "TURNAROUND");
            }
            if (ORIENTATION == 3) {
                strcpy(movement, "RIGHT");
            }
            ORIENTATION = 0;
        }       
    }
    
    if (next.x < orig.x) {
        if (ORIENTATION == 2) {
            strcpy(movement, "FORWARD");
            CURRPOINT.x = next.x;
        }
        else {
            if (ORIENTATION == 0) {
            strcpy(movement, "TURNAROUND");
            }
            if (ORIENTATION == 1) {
                strcpy(movement, "RIGHT");
            }
            if (ORIENTATION == 3) {
                strcpy(movement, "LEFT");
            }
            ORIENTATION = 2;
        }       
    }
    
    if (next.y > orig.y) {
        if (ORIENTATION == 3) {
            strcpy(movement, "FORWARD");
            CURRPOINT.y = next.y;
        }
        else {
            if (ORIENTATION == 0) {
                strcpy(movement, "LEFT");
            }
            if (ORIENTATION == 2) {
                strcpy(movement, "RIGHT");
            }
            if (ORIENTATION == 1) {
                strcpy(movement, "TURNAROUND");
            }
            ORIENTATION = 3;
        }
        
    }
    
    if (next.y < orig.y) {
        if (ORIENTATION == 1) {
            strcpy(movement, "FORWARD");
            CURRPOINT.y = next.y;
        }
        else {
            if (ORIENTATION == 0) {
           strcpy(movement, "RIGHT");
            }
            if (ORIENTATION == 3) {
                strcpy(movement, "TURNAROUND");
            }
            if (ORIENTATION == 2) {
                strcpy(movement, "LEFT");
            }
            ORIENTATION = 1;
        }
    }
    moveRover(movement);
    orig.x = next.x;
    orig.y = next.y;
}

void moveRover(char* nextMovement) {
    if (strcmp(nextMovement, "FORWARD") == 0) {
        sendTimerValtoPathMovement(MOVE_FORWARD);
    }
    
    if (strcmp(nextMovement, "RIGHT") == 0) {
       sendTimerValtoPathMovement(MOVE_RIGHT);
    }
    
    if (strcmp(nextMovement, "LEFT") == 0) {
        sendTimerValtoPathMovement(MOVE_LEFT);
    }
    
    if (strcmp(nextMovement, "TURNAROUND") == 0) {
        sendTimerValtoPathMovement(MOVE_RIGHT);
        sendTimerValtoPathMovement(MOVE_RIGHT);
    }
}

void makeMove() {
    coordinate g, next;
    g.x = goalPosition.gx;
    g.y = goalPosition.gy;
    next = getNextPoint();
    while (!pointEqual(CURRPOINT, g)) {
        getNextMovement(CURRPOINT, next);
//        moveRover(movement);
        if (strcmp(movement, "FORWARD") == 0) {
            next = getNextPoint();
            emptyCharArray(movement, 10);
        }
        emptyCharArray(movement, 10);
    }
}

//void makeMove() {
//    sendTimerValtoPathMovement(MOVE_RIGHT);
//    sendTimerValtoPathMovement(MOVE_LEFT);
//    sendTimerValtoPathMovement(MOVE_FORWARD);
//    sendTimerValtoPathMovement(MOVE_FORWARD);
//    sendTimerValtoPathMovement(MOVE_FORWARD);
//}
/************** end of Movement functions*************/


