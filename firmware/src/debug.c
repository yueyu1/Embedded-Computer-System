/* ************************************************************************** */
/** Debugging routines

  @File Name
    debug.c

  @Summary
    Implementation of debug.h prototypes
 */
/* ************************************************************************** */

/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
/* ************************************************************************** */

/* This section lists the other files that are included in this file.
 */

#include "debug.h"

/** 
  @Function
    void dbgOutputVal ( unsigned char outVal ) 

  @Summary
    Write an 8-bit value to J3-1 to J3-8.

  @Remarks
    Refer to the debug.h interface header for function usage details.
 */
void dbgOutputVal(unsigned char outVal) {
    // write bit 0 to RD12
    SYS_PORTS_PinWrite (PORTS_ID_0, PORT_CHANNEL_D, PORTS_BIT_POS_12, outVal & 0x01);
    // write bit 1 to RG1
    SYS_PORTS_PinWrite (PORTS_ID_0, PORT_CHANNEL_G, PORTS_BIT_POS_1, outVal & (0x01 << 1));
    // write bit 2 to RD3
    SYS_PORTS_PinWrite (PORTS_ID_0, PORT_CHANNEL_D, PORTS_BIT_POS_3, outVal & (0x01 << 2));
    // write bit 3 to RG0
    SYS_PORTS_PinWrite (PORTS_ID_0, PORT_CHANNEL_G, PORTS_BIT_POS_0, outVal & (0x01 << 3));
    // write bit 4 to RD4
    SYS_PORTS_PinWrite (PORTS_ID_0, PORT_CHANNEL_D, PORTS_BIT_POS_4, outVal & (0x01 << 4));
    // write bit 5 to RA6
    SYS_PORTS_PinWrite (PORTS_ID_0, PORT_CHANNEL_A, PORTS_BIT_POS_6, outVal & (0x01 << 5));
    // write bit 6 to RC4
    SYS_PORTS_PinWrite (PORTS_ID_0, PORT_CHANNEL_C, PORTS_BIT_POS_4, outVal & (0x01 << 6));
    // write bit 7 to RA7
    SYS_PORTS_PinWrite (PORTS_ID_0, PORT_CHANNEL_A, PORTS_BIT_POS_7, outVal & (0x01 << 7));
}

void dbgOutputLoc(unsigned char outVal){
    // write bit 0 to RA2
    SYS_PORTS_PinWrite (PORTS_ID_0, PORT_CHANNEL_A, PORTS_BIT_POS_14, outVal & 0x01);
    // write bit 1 to RG14
    SYS_PORTS_PinWrite (PORTS_ID_0, PORT_CHANNEL_A, PORTS_BIT_POS_15, outVal & (0x01 << 1));
    // write bit 2 to RA3
    SYS_PORTS_PinWrite (PORTS_ID_0, PORT_CHANNEL_D, PORTS_BIT_POS_14, outVal & (0x01 << 2));
    // write bit 3 to RG12
    SYS_PORTS_PinWrite (PORTS_ID_0, PORT_CHANNEL_D, PORTS_BIT_POS_15, outVal & (0x01 << 3));
    // write bit 4 to RG13
    SYS_PORTS_PinWrite (PORTS_ID_0, PORT_CHANNEL_F, PORTS_BIT_POS_4, outVal & (0x01 << 4));
    // write bit 5 to RA10
    SYS_PORTS_PinWrite (PORTS_ID_0, PORT_CHANNEL_F, PORTS_BIT_POS_5, outVal & (0x01 << 5));
    // write bit 6 to RA9
    SYS_PORTS_PinWrite (PORTS_ID_0, PORT_CHANNEL_F, PORTS_BIT_POS_12, outVal & (0x01 << 6));
    // write bit 7 to RA14
    SYS_PORTS_PinWrite (PORTS_ID_0, PORT_CHANNEL_F, PORTS_BIT_POS_13, outVal & (0x01 << 7));
}

/** 
  @Function
    void dbgUARTVal ( unsigned char outVal ) 

  @Summary
    Write an 8-bit value to UART Tx Buffer.

  @Remarks
    Refer to the debug.h interface header for function usage details.
 */
bool dbgUARTVal(unsigned char outVal) {
    if(!DRV_USART0_TransmitBufferIsFull()) {
        DRV_USART0_WriteByte(outVal);
        return true;
    }
    else {
//        dbgOutputLoc(DLOC_UART_BUF_FULL);
        return false;
    }
}

void dbgSendMsgServer(char *msg){
    int i;
    int j;
//    dbgOutputVal(strlen(msg));
    for(j = 0; j < strlen(msg); j++){
        if(!dbgUARTVal(msg[j])) {
            j--; 
       }
//        dbgOutputVal(msg[j]);
    }
    for(i = 0; i < 3; i++){
       if(!dbgUARTVal(SERVER_MSG_DELIMITER)) {
           i--;
       }
    }
}

char dbgRcvUARTVal(){
    char rxChar;
    while(!PLIB_USART_ReceiverDataIsAvailable(USART_ID_1)) {
        
    }
    rxChar = PLIB_USART_ReceiverByteReceive(USART_ID_1);
    return rxChar;
}
void dbgReceiveMsgServer(){
    while(!DRV_USART0_ReceiverBufferIsEmpty()){ 
        dbgOutputVal(dbgRcvUARTVal());
    }
   //return msg;
}

void controlLED(int switchON){
    if (switchON) 
        SYS_PORTS_PinWrite (PORTS_ID_0, PORT_CHANNEL_E, PORTS_BIT_POS_8, 1);
    else
        SYS_PORTS_PinWrite (PORTS_ID_0, PORT_CHANNEL_E, PORTS_BIT_POS_8, 0);
}
/** 
  @Function
    void dbgSysHalt ( unsigned char outVal ) 

  @Summary
    Write an 8-bit value to UART Tx Buffer.

  @Remarks
    Refer to the debug.h interface header for function usage details.
 */
void dbgSysHalt(unsigned char outVal) {
    // Debug output
    dbgOutputLoc(outVal);
    // Disable all interrupts
    SYS_INT_Disable();
    // Clear timer interrupt flag 
    PLIB_INT_SourceFlagClear(INT_ID_0,INT_SOURCE_TIMER_2);
    // Enable LED4
    SYS_PORTS_PinSet(PORTS_ID_0, PORT_CHANNEL_A, PORTS_BIT_POS_3);
    while(1) {
        
    }
}

/* *****************************************************************************
 End of File
 */
