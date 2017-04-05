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

#ifndef _APP_PUBLIC_H    /* Guard against multiple inclusion */
#define _APP_PUBLIC_H


/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
/* ************************************************************************** */

/* This section lists the other files that are included in this file.
 */

//#include "app.h"
#include "jsmn.h"
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


/* Provide C++ Compatibility */
#ifdef __cplusplus
extern "C" {
#endif

    /* ************************************************************************** */
    /* ************************************************************************** */
    /* Section: Constants                                                         */
    
    
bool receiveBytes;
//typedef struct message message;
#define FLAGROVER "flag"
#define TAGGERROVER "tagger"
#define SENSORROVER "sensor"
#define CMROVER "cm"


typedef struct msg_t {
    char *to;
    char *from;
    int sequence;
    char *payload;
}message;

#define JSON_startbrace '{'
#define JSON_endbrace '}'
#define JSON_comma ','
#define JSON_colon ':'
#define JSON_quotes '"'
#define JSON_to * "to"
#define JSON_from * "from"
#define JSON_sequence * "sequence"
#define JSON_payload * "payload"

message ex_msg;
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
    char* constructJson(message exMsg);
    void sendMessage(char *to, char *from, int sequence, char* payload);
    int jsoneq(const char *json, jsmntok_t *tok, const char *s);
    char* strndup_custom (const char *json, jsmntok_t *tok);
    char* parse_json(char *JSON_STRING);


    /* Provide C++ Compatibility */
#ifdef __cplusplus
}
#endif

#endif /* _EXAMPLE_FILE_NAME_H */

/* *****************************************************************************
 End of File
 */
