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

#include "../include/comm.h"
#include "../include/driver/uart.h"

/* Data format:
 *
 * OPEN | CONFIG | DATA_H | DATA_L | CLOSE
 * 0x30   0 to 8    16-bits data     0x35
 *
 */

bool COMM_SendParameter(COMM_Param_t p, UINT16 value) {
    bool ret;

    byte protocol[PROTOCOL_LENGTH] = {START_BYTE, 0, 0, 0, END_BYTE};

    protocol[1] = (byte) p; // parameter ID
    protocol[2] = (value >> 8) & 0xff; // parameter higher value
    protocol[3] = value & 0xff; // parameter lower value

    ret = UART_Send(protocol, PROTOCOL_LENGTH); // send pack

    return ret;
}