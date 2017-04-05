/* ************************************************************************** */
/** Descriptive File Name

  @Company
    Company Name

  @File Name
    filename.h

  @Summary
    Brief description of the file.

  @Description
    Describe the purpose of this file.
 */
/* ************************************************************************** */

#ifndef _MOTORCONTROL_PUBLIC_H    /* Guard against multiple inclusion */
#define _MOTORCONTROL_PUBLIC_H

#define MOTOR_CONTROL_HALT 0
#define MOTOR_CONTROL_FORWARD 1
#define MOTOR_CONTROL_REVERSE 2
#define MOTOR_CONTROL_LEFT 3
#define MOTOR_CONTROL_RIGHT 4

#define MOTOR_REQUEST_ENCVAL 5


void motorControlSendValToMsgQFromISR(char motorMacro);
void motorControlSendValToMsgQ(char motorMacro);

static char requestState;

    /* Provide C++ Compatibility */
#ifdef __cplusplus
}
#endif

#endif /* _EXAMPLE_FILE_NAME_H */

/* *****************************************************************************
 End of File
 */
