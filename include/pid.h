#ifndef _PID_H_
#define _PID_H_

#include "../include/defs.h"

#include "../include/fixedptc.h"
#include "../include/driver/timer.h"

typedef struct {
    /* private */
    INT16 *input;
    INT16 *output;

    INT16 setPoint;

    float kP; // proportional gain
    float kI; // integrative gain
    float kD; // derivative gain

    /* private */
    INT16 lastInput;
} pid_t;

/**
 * @brief Initilize a instance of pid_t (PID)
 *
 * @param pid Pointer to a PID object
 */
void pid_Init(pid_t* pid);

/**
 * @brief Calculates a new action to PID loop
 *
 * @param pid Pointer to a PID instance
 * @param now_point New plant feedback from plant
 *
 * @return Returns a value to output from PID controller
 */
int16_t pid_Compute(pid_t* pid);

void pid_SetParameters(void);

#endif // _PID_H_
