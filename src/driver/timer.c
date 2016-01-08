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

#define TIMER0_PRESCALER 0b001 // 1:4
#define TIMER0_PRELOAD 55 // 200 increments to overflow (100us)

#define COUNT_PER_TICK 10

static struct {
    UINT16 time;
    byte auxTimerCounter;
} Prv;

static Timer* timers[NUM_MAX_TIMERS]; // all timers

void TIMER_Bootstrap() {
    // Timer0 Interrupt
    INTCON2bits.TMR0IP = 0; // low priority interrupt
    INTCONbits.TMR0IE = 1; // enable timer0 interrupt
    INTCONbits.TMR0IF = 0; // clean flag

    T0CONbits.T0PS = TIMER0_PRESCALER;
    T0CONbits.T016BIT = 1; // 8bits timer
    T0CONbits.T0CS = 0; // timer0 clock source on (FOSC/4)
    T0CONbits.PSA = 0; // assigns prescaler to timer0

    TMR0 = TIMER0_PRELOAD;
    T0CONbits.TMR0ON = 1; // turns on timer0

    byte i;
    for (i = 0; i < NUM_MAX_TIMERS; i++) {
        timers[i] = NULL; // null all timers
    }

    Prv.time = 0;
    Prv.auxTimerCounter = 0;
}

bool TIMER_Create(Timer* timer, bool autoReload, TimerCallbackFunction pCallbackFunction) {
    byte i;
    for (i = 0; i < NUM_MAX_TIMERS; i++) {
        if (timers[i] == NULL) { // if there is a slot
            timer->autoReload = autoReload;
            timer->pCallbackFunction = pCallbackFunction;

            timers[i] = timer; // storage the timer
            return true;
        }
    }
    return false;
}

void TIMER_SetPeriod(Timer* timer, UINT16 period) {
    timer->period = period;
}

void TIMER_Start(Timer* timer) {
    UINT16 timeNow = Prv.time;

    timer->triggerTime = timer->period + timeNow; // set trigger timer
    timer->needToOverflow = timer->triggerTime > timeNow ? false : true;

    timer->enabled = true; // enable
}

void TIMER_Stop(Timer* timer) {
    timer->enabled = false;
}

void TIMER_HwEventHandle(void) {
    TMR0 = TIMER0_PRELOAD; // reload

    Prv.auxTimerCounter++;
    if (Prv.auxTimerCounter >= COUNT_PER_TICK) {
        Prv.time++;
        Prv.auxTimerCounter = 0;
    }

    byte i;

    for (i = 0; timers[i] != NULL; i++) {
        if (timers[i]->enabled == true) {
            if (timers[i]->needToOverflow) {
                timers[i]->needToOverflow = false;
            } else if (Prv.time >= timers[i]->triggerTime) {
                timers[i]->pCallbackFunction();

                if (timers[i]->autoReload == true) {
                    timers[i]->triggerTime = timers[i]->period + Prv.time; // set trigger timer
                    timers[i]->needToOverflow = timers[i]->triggerTime > Prv.time ? false : true;
                } else {
                    timers[i]->enabled = false;
                }
            }

        }
    }
}



