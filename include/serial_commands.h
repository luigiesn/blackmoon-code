/*
 * Serial commands identifiers - part of Blackmoon servo controller
 * Copyright (C) 2016 - Luigi E. Sica Nery
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

#ifndef _SERIAL_COMMANDS_H_
#define _SERIAL_COMMANDS_H_

/* Blackmoon Servo Commands Identifiers */

#define BOARD_STATUS 0

// PID loop
#define PID_KP 1
#define PID_KI 2
#define PID_KD 3
#define PID_KS 4
#define PID_PERIOD 5
#define PID_SET_POINT 6

// bridge
#define OUTPUT_MAX 7
#define INPUT_MAX 8
#define DEADZONE 9
#define INVERT_BRIDGE 10

#endif /* _SERIAL_COMMANDS_H_ */