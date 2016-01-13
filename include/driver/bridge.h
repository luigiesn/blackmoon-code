/*
 * H-bridge controller - part of Blackmoon servo controller
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

#ifndef _BRIDGE_H_
#define	_BRIDGE_H_

#include <xc.h>
#include "../defs.h"

/*
 * Driver 1 : RB2 e RB4(PWM2)
 * Driver 2 : RB3 e RB5(PWM3)
 */

// PWM defs
#define PWM_PERIOD 270                  // 29,52kHz
#define PWM_DUTYMAX 1023                //10 bits resolution - 94,72% (270*4*0,9472)
#define PWM_TIMEBASE_POSTSCALER 1       // 1 to 16

typedef enum {
    bdForward, bdBackward
} BridgeDirection;


void Bridge_Bootstrap(void);
void Bridge_Init(void);

void Bridge_On(void);
void Bridge_Off(void);

void Bridge_SetOutput(BridgeDirection brgDir, UINT16 duty);

#endif	/* _BRIDGE_H_ */

