/*
 * PWM module driver - part of Blackmoon servo controller
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

#ifndef _PWM_H
#define	_PWM_H

#include <xc.h>
#include "../defs.h"

#define PWM_PERIOD 270 // 29,52kHz
#define PWM_DUTYMAX 1023 //10 bits resolution - 94,72% (270*4*0,9472)
#define PWM_TIMEBASE_POSTSCALER 1  // 1 to 16

void PWM_Bootstrap(void);

void PWM_SetDuty(UINT16 duty);

#endif	/* _PWM_H_ */

