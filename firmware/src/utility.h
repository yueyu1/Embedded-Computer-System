/* 
 * File:   utility.h
 * Author: Yue
 *
 * Created on April 4, 2017, 3:21 AM
 */

#ifndef UTILITY_H
#define	UTILITY_H

#ifdef	__cplusplus
extern "C" {
#endif
    
#include "communication_public.h"
#include "motorControl_public.h"
    
/* Utility functions */
void clear(char *str);
int getStrLen(char* str);
char* strndup_custom (const char *json, jsmntok_t *tok, char *dest);
void emptyCharArray(char* arr, int size);
void emptyIntArray(int* arr, int size);
void copyNonNull(char *dest, char *source, int size_dest);
void cleanUpTail(char *msg);
bool pointEqual(coordinate c1, coordinate c2);

/* Checksum functions */
int getSendingChecksum(char *arr);
int getReceivedChecksum(char *arr);
int countReceivedChecksum(char *arr);
bool compareChecksum(int check1, int check2);

/* JSON functions*/
int jsoneq(char *json, jsmntok_t *tok, char *s);
message parseJsonMsg(char *jsonStr);
void parse(char *JSON_STRING, char *field, char *dest);
void parseArrayPayload(char *JSON_STRING, char *field);
void constructPathJson(char *buf, message exMsg);
void constructJson(char *buf, message exMsg);
void constructNextMovementJson(char *buf, message exMsg);

/* Map functions*/
void storeMapData(char* field, int n);
void transformMapData();

/* Movement functions */
coordinate getNextPoint();
void getNextMovement(coordinate orig, coordinate next);
void moveRover(char* nextMovement);
void makeMove();

/* Reset functions*/
void resetAStartGlobalVariables();
void resetMapDataGlobalVariables();
void resetPathMovementGlobalVariables();

#ifdef	__cplusplus
}
#endif

#endif	/* UTILITY_H */

