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
#include "../../include/driver/serial.h"

#define TIMER0_PRESCALER 0b001 // 1:4
#define TIMER0_PRELOAD 55 // 200 increments to overflow (100us)

#define COUNT_PER_HWTICK 10

static struct {
    UINT16 time;
    byte auxTimerCounter;

    UINT16 lastTime;
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

    byte i;
    for (i = 0; i < NUM_MAX_TIMERS; i++) {
        timers[i] = NULL; // null all timers
    }

    Prv.time = 0;
    Prv.auxTimerCounter = 0;
    Prv.lastTime = 0;

    T0CONbits.TMR0ON = 1; // turns on timer0
}

void TIMER_Process(void) {
    byte i;
    UINT16 timeNow = Prv.time;
    UINT16 timeBetweenTimerProcessCalls;

    // calculate time elapsed between TIMER_Process() calls
    if (timeNow >= Prv.lastTime) {
        timeBetweenTimerProcessCalls = timeNow - Prv.lastTime;
    } else {
        timeBetweenTimerProcessCalls = 65535 - (Prv.lastTime - timeNow);
    }
    Prv.lastTime = timeNow;

    // check all timers
    for (i = 0; timers[i] != NULL; i++) {
        if (timers[i]->enabled == true) {
            if (timers[i]->elapsedTime >= timers[i]->period) { // if time passed enough
                timers[i]->pCallbackFunction(); // call

                if (timers[i]->autoReload == true) { // if reload option is true
                    timers[i]->elapsedTime = 0; // reset elapsed time counting
                } else {
                    timers[i]->enabled = false; // if not, turn-off this timer
                }
            } else {
                if (65535 - timers[i]->elapsedTime >= timeBetweenTimerProcessCalls) {
                    timers[i]->elapsedTime + timeBetweenTimerProcessCalls; // if not, increment elapsed time
                } else {
                    timers[i]->elapsedTime = 65535; // if elapsed time exceeds maximum period, force to call on next time
                }
            }
        }
    }
}

bool TIMER_Create(Timer* timer, TimerCallbackFunction pCallbackFunction) {
    byte i;
    for (i = 0; i < NUM_MAX_TIMERS; i++) {
        if (timers[i] == NULL) { // if there is a slot
            timers[i] = timer; // storage the timer

            timer->pCallbackFunction = pCallbackFunction;
            return true;
        }
    }
    return false;
}

void TIMER_SetPeriod(Timer* timer, UINT16 period) {
    timer->period = period;
}

void TIMER_Start(Timer* timer, bool autoReload) {
    timer->autoReload = autoReload;
    timer->enabled = true; // enable
}

void TIMER_Stop(Timer* timer) {
    timer->enabled = false;
}

void TIMER_HwEventHandle(void) {
    TMR0 = TIMER0_PRELOAD; // reload

    Prv.auxTimerCounter++;
    if (Prv.auxTimerCounter >= COUNT_PER_HWTICK) {
        Prv.time++;
        Prv.auxTimerCounter = 0;
    }
}



