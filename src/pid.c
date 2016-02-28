#include "../include/pid.h"

void pid_init(pid_t* pid) {
    pid->set_point = 0;

    pid->k_p = 0;
    pid->k_i = 0;
    pid->k_d = 0;

    pid->p_action = 0;
    pid->i_action = 0;
    pid->d_action = 0;

    pid->pid_output = 0;

    pid->err_accum = 0;
    pid->current_error = 0;
    pid->last_error = 0;
}

float pid_Compute(pid_t* pid) {
    pid->now_point = now_point;
    pid->current_error = pid->set_point - pid->now_point;


    pid->p_action = pid->current_error * pid->k_p; // proporcional

    pid->i_action = (pid->err_accum + pid->current_error) * pid->k_i; // integrative

    pid->d_action = (pid->last_error - pid->current_error) * pid->k_d; // derivative


    pid->pid_output = (pid->p_action + pid->i_action + pid->d_action) * pid->k_s;

    // output saturation handling
    if (pid->pid_output > pid->upper_sat_point) {
        pid->pid_output = pid->upper_sat_point;
    } else if (pid->pid_output < pid->lower_sat_point) {
        pid->pid_output = pid->lower_sat_point;
    }

    pid->last_error = pid->current_error;
    return pid->p_action + pid->i_action + pid->d_action;
}

