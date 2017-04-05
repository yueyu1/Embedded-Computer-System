/* ************************************************************************** */
/** Deugging routines for Max32

  @File Name
    debug.h

  @Summary
    Debugging routines to read/write values from I/O ports
 * 
 */
/* ************************************************************************** */

#ifndef _DEBUG_H    /* Guard against multiple inclusion */
#define _DEBUG_H


/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
/* ************************************************************************** */

/* This section lists the other files that are included in this file.
 */
#include "system/ports/sys_ports.h"
#include "driver/usart/drv_usart_static.h"

/* Provide C++ Compatibility */
#ifdef __cplusplus
extern "C" {
#endif


    /* ************************************************************************** */
    /* ************************************************************************** */
    /* Section: Constants                                                         */
    /* ************************************************************************** */
    /* ************************************************************************** */
    
#define DLOC_INIT_BEGIN 0x00
#define DLOC_TASK_ENTER 0x01
#define DLOC_ENTER_APP_STATE_INIT 0x02
#define DLOC_STATE_INIT_BEFORE_SEND_MSG 0x03
#define DLOC_STATE_INIT_AFTER_SEND_MSG 0x04
#define DLOC_ENTER_APP_STATE_MSG_Q_WAIT 0x05
#define DLOC_STATE_MSG_Q_WAIT_BEFORE_RECEIVE_MSG 0x06
#define DLOC_STATE_MSG_Q_WAIT_AFTER_RECEIVE_MSG 0x07
#define DLOC_ENTER_APP_STATE_DEBUG_ROUTINES 0x08
#define DLOC_ISR_BEFORE_SEND_MSG 0x09
#define DLOC_ISR_AFTER_SEND_MSG 0x0a
#define DLOC_ENTER_ISR 0x0b
#define DLOC_EXIT_ISR 0x0c
#define DLOC_PRE_WHILE_LOOP 0x0d
#define DLOC_MSG_Q_SETUP_FAIL 0xf0
#define DLOC_MSG_Q_FULL 0xf1
#define DLOC_MSG_Q_RX_FAIL 0xf2
#define DLOC_UART_BUF_FULL 0xf3
    
#define SERVER_MSG_DELIMITER '@'

    // *****************************************************************************
    // *****************************************************************************
    // Section: Data Types
    // *****************************************************************************
    // *****************************************************************************

    // *****************************************************************************

    /** Descriptive Data Type Name

    // *****************************************************************************
    // *****************************************************************************
    // Section: Interface Functions
    // *****************************************************************************
    // *****************************************************************************

    // *****************************************************************************
    /**
      @Function
        void dbgOutputVal ( unsigned char outVal ) 

      @Summary
        Outputs an 8 bit value to J3-1 to J3-8


      @Parameters
        @param outVal Value to write to the PIO pins.
     */
    void dbgOutputVal(unsigned char outVal);
    
    // *****************************************************************************
    /**
      @Function
        void dbgUARTVal ( unsigned char outVal ) 

      @Summary
        Outputs an 8 bit value UART0


      @Parameters
        @param outVal Value to write to the UART Tx buffer.
     */
    bool dbgUARTVal(unsigned char outVal);
    
    void dbgSendMsgServer(char *msg);
    
    char dbgRcvUARTVal();
    
    void dbgReceiveMsgServer();
    
    // *****************************************************************************
    /**
      @Function
        void dbgOutputLoc ( unsigned char outVal ) 

      @Summary
        Outputs an 8 bit value to J3-8 to J3-16


      @Parameters
        @param outVal Value to write to the PIO ports
     */
    void dbgOutputLoc(unsigned char outVal);
    
    // *****************************************************************************
    /**
      @Function
        void dbgSysHalt ( unsigned char outVal ) 

      @Summary
        Outputs an 8 bit value to J3-8 to J3-16 and keeps the system halted in a while loop


      @Parameters
        @param outVal Value to write to the PIO ports
     */
    void dbgSysHalt(unsigned char outVal);

    /* Provide C++ Compatibility */
#ifdef __cplusplus
}
#endif

#endif /* _EXAMPLE_FILE_NAME_H */

/* *****************************************************************************
 End of File
 */
