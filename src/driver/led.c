/*
 * User led driver - part of Blackmoon servo controller
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

#include "../../include/driver/led.h"

static struct {
    Timer ledTimer;
} Prv;

void LED_Change(void);

void LED_Bootstrap(void) {
    ADCON1bits.PCFG2 = 1; // Port is configured as RA4

    LED_PORT &= (~LED_MASK);
    LED_PORT_DIR &= (~LED_MASK);
}

void LED_Init(void) {
    TIMER_Create(&Prv.ledTimer, true, LED_Change);
}

void LED_Mode(UINT16 period) {
    if (period == LED_CONST_OFF) {
        TIMER_Stop(&Prv.ledTimer);
        LED_ForceOff();
    } else if (period == LED_CONST_OFF) {
        TIMER_Stop(&Prv.ledTimer);
        LED_ForceOn();
    } else {
        TIMER_SetPeriod(&Prv.ledTimer, period);
        TIMER_Start(&Prv.ledTimer);
    }
}

void LED_Change(void) {
    if (LED_PORT & LED_MASK)
        LED_ForceOff();
    else
        LED_ForceOn();
}
