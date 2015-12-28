/*
 * Communication module - part of Blackmoon servo controller
 * Copyright (C) 2015 - Luigi E. Sica Nery
 *
 * This program is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of  MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _COMM_H_
#define _COMM_H_

#include "../include/defs.h"

#define PROTOCOL_LENGTH 5 //bytes
#define START_BYTE 0x30
#define END_BYTE 0x35

enum COMM_Param {
    ucpSTATUS, ucpPID_KP, ucpPID_KI, ucpPID_KD, ucpFREQ, ucpPWM_MAX, ucpINPUT_MAX, ucpDEADZONE, ucpADC_INPUT 
};

typedef enum COMM_Param COMM_Param_t;

/**
 * @brief Send a internal parameter
 * 
 * Make a valid single parameter pack
 * 
 * @param p Parameter to send
 * @param value Parameter value
 * 
 * @return Return "true" if parameter was sent, "false" if not.
 */
bool COMM_SendParameter(COMM_Param_t p, UINT16 value);

#endif // _COMM_H_