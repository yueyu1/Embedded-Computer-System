/* ************************************************************************** */
/** Public methods to access "app" task
  @File Name
    app_public.h
  @Summary
    This file hosts public function declarations to access public features of the 
    app task.
  @Description
    The purpose of this file is to allow ISRs to access the mailbox of "app"
 */
/* ************************************************************************** */

#ifndef _COMM_PUBLIC_H    /* Guard against multiple inclusion */
#define _COMM_PUBLIC_H


/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
/* ************************************************************************** */

/* This section lists the other files that are included in this file.
 */
#include "jsmn.h"
#include "astar.h"
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "pathMovement.h"


/* Provide C++ Compatibility */
#ifdef __cplusplus
extern "C" {
#endif

    /* ************************************************************************** */
    /* ************************************************************************** */
    /* Section: Constants                                                         */

#define FLAGROVER "FLAG"
#define TAGGERROVER "TAGGER"
#define SENSORROVER "SENSOR"
#define CMROVER "CM"

#define JSON_VAL_MAX_SIZE 20
#define MAP_VAL_MAX_SIZE 200

#define JSON_startbrace '{'
#define JSON_endbrace '}'
#define JSON_comma ','
#define JSON_colon ':'
#define JSON_quotes '"'
#define JSON_to * "to"
#define JSON_from * "from"
#define JSON_sequence * "sequence"
#define JSON_payload * "payload"
    
#define ROVERSIGN_THIS "TAGGER"
    
//#define ADDRESS_WRITE 0x52
//#define ADDRESS_READ 0x53

typedef struct msg_t {
    char to[JSON_VAL_MAX_SIZE];
    char from[JSON_VAL_MAX_SIZE];
    int sequence;
    char payload[JSON_VAL_MAX_SIZE];
}message;

typedef struct map_t {
    int obstacle[MAP_VAL_MAX_SIZE]; // In the form of x1, y1, x2, y2...
    int roverpos[MAP_VAL_MAX_SIZE]; // In the form of x1, y1, x2, y2...
    int flagpos[MAP_VAL_MAX_SIZE];  // In the form of x, y...
    int boundary[MAP_VAL_MAX_SIZE]; // In the form of min_x, min_y, max_x, max_y...
    int startpos[MAP_VAL_MAX_SIZE]; // In the form of x, y...
}map;

message ex_msg;
map ex_map;

bool receiveBytes;

static coordinate coord;
static o_length;
static flag_length;
static boundary_length;
static startPos_length;

/* 0: East
 * 1: South
 * 2: West
 * 3: North
 */
static int ORIENTATION;
static coordinate CURRPOINT;
static int FINISHED;

    /* ************************************************************************** */
    /* ************************************************************************** */

    /*  A brief description of a section can be given directly below the section
        banner.
     */


    /* ************************************************************************** */
    /** Descriptive Constant Name
      @Summary
        Brief one-line summary of the constant.
    
      @Description
        Full description, explaining the purpose and usage of the constant.
        <p>
        Additional description in consecutive paragraphs separated by HTML 
        paragraph breaks, as necessary.
        <p>
        Type "JavaDoc" in the "How Do I?" IDE toolbar for more information on tags.
    
      @Remarks
        Any additional remarks
     */
//#define EXAMPLE_CONSTANT 0


    // *****************************************************************************
    // *****************************************************************************
    // Section: Data Types
    // *****************************************************************************
    // *****************************************************************************

    /*  A brief description of a section can be given directly below the section
        banner.
     */


    // *****************************************************************************

    /** Descriptive Data Type Name
      @Summary
        Brief one-line summary of the data type.
    
      @Description
        Full description, explaining the purpose and usage of the data type.
        <p>
        Additional description in consecutive paragraphs separated by HTML 
        paragraph breaks, as necessary.
        <p>
        Type "JavaDoc" in the "How Do I?" IDE toolbar for more information on tags.
      @Remarks
        Any additional remarks
        <p>
        Describe enumeration elements and structure and union members above each 
        element or member.
     */
//    typedef struct _example_struct_t {
//        /* Describe structure member. */
//        int some_number;
//
//        /* Describe structure member. */
//        bool some_flag;
//
//    } example_struct_t;


    // *****************************************************************************
    // *****************************************************************************
    // Section: Interface Functions
    // *****************************************************************************
    // *****************************************************************************

    /*  A brief description of a section can be given directly below the section
        banner.
     */

    // *****************************************************************************
    /**
      @Function
        int appSendTimerValToQ(unsigned int msecElapsed)
      @Summary
     Places the value of Timer2 into app message q.
      @Precondition
     QueueHandle_t must exist for app
      @Parameters
        @param msecElapsed Value of timer to place into q
    
      @Returns
        List (if feasible) and describe the return values of the function.
        <ul>
          <li>0   message was placd into q
          <li>1   q was full
        </ul>
     */
    int appSendTimerValToQ(unsigned int msecElapsed);
    void doneReceiving(char rx);
    void processMessage(message exMsg);
    void sendMessage(char *to, char *from, int sequence, char* payload);
    void sendTapeSensorData(int direction);
    void sendColorSensorData(int appearVal);

    /* Provide C++ Compatibility */
#ifdef __cplusplus
}
#endif

#endif /* _EXAMPLE_FILE_NAME_H */

/* *****************************************************************************
 End of File
 */