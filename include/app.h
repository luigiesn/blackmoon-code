/*
 * Main application of Blackmoon Servo - part of Blackmoon servo controller
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

#ifndef _APP_H_
#define	_APP_H_

#include "defs.h"

#define ADC_FEEDBACK_CH 1

// mode selector
#define MODE TEST
#define PID_CONTROLLER 0
#define TEST 1

void App_Boostrap(void);

void App_Init(void);

void App_Process(void);

#endif	/* _APP_H_ */

