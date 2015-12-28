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

#include "../../include/driver/pwm.h"

void PWM_Bootstrap(void) {
    PTCON0 = 0x00; 

    PWMCON0 = 0x37; // All PWM I/O pins enabled for PWM output, PWM I/O pin pair is in the Independent mode

    PTMRL = 0x00; // cleans PWM timebase
    
    PTCON0bits.PTOPS = PWM_TIMEBASE_POSTSCALER;

    OVDCOND = 0x00; // override all
    OVDCONS = 0x00; // with low-state

    OSYNC = 0; //Output overrides via the OVDCON register are synchronized to the PWM time base

    // config PWM periodo
    PTPERH = PWM_PERIOD >> 8; //  t = ((PTPER + 1) x PTMRPS)/(FOSC/4)
    PTPERL = PWM_PERIOD;

    // config intial duty-cicle
    PDC1L = 0;
    PDC1H = 0;
    
    PTCON1 |= 0x80;     // run
}

void PWM_SetDuty(UINT16 duty) {
    if (duty > PWM_DUTYMAX) {
        duty = PWM_DUTYMAX;
    }
    PDC1L = duty;
    PDC1H = duty >> 8;
}