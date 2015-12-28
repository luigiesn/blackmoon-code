/*
 * Timer driver - part of Blackmoon servo controller
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

#include "../../include/driver/timer.h"

#define TIMER0_PRESCALER_64 0b101
#define TIMER0_PRELOAD 130 // 125 increments to overflow

void TIMER_Boostrap() {
    T0CONbits.T0PS = TIMER0_PRESCALER_64;
    T0CONbits.T016BIT = 1; // 8bits timer
    T0CONbits.T0CS = 0; // timer0 clock source on (FOSC/4)
    T0CONbits.PSA = 0; // assigns prescaler to timer0

    TMR0 = TIMER0_PRELOAD;
    T0CONbits.TMR0ON = 1; // turns on timer0
}

void TIMER_Init() {

}

void TIMER_Process() {

}

