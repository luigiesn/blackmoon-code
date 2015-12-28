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

#define ACTIVE 1
#define INACTIVE 0

#define HW_BreakEnabled() (!PORTBbits.RB7)

#define BRIDGE_IN1(IN) (POUT0 = IN)  // ligado ao pino IN
#define BRIDGE_IN2(IN) (POUT1 = IN)

#define BRIDGE_PWM1_OVERRIDE_VALUE (POUT2) // ligado ao pino nSD
#define BRIDGE_PWM2_OVERRIDE_VALUE (POUT3)

#define BRIDGE_PWM1_STATE(STATE) (POVD2 = STATE)
#define BRIDGE_PWM2_STATE(STATE) (POVD3 = STATE)

void BRG_Boostrap(void);
void BRG_Init(void);
void BRG_Process(void);

void BRG_DirSet(UniDimVector brgDir);
UniDimVector BRG_DirGet(void);

void BRG_LoadShortCircuit(void);

void BRG_On(void);
void BRG_Off(void);

#endif	/* _BRIDGE_H_ */

