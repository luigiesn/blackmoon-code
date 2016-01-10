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

#include "../include/app.h"

#include "../include/driver/led.h"
#include "../include/driver/serial.h"
#include "../include/driver/adc.h"

typedef enum {
    asPID, asTEST
} app_mode;

static struct {
    app_mode mode;
    Timer PIDtimer;
} Prv;

void App_Boostrap(void) {
    Prv.mode = MODE;
}

void App_Init(void) {
    LED_Mode(LED_CONST_ON);

    ADC_SelectChannel(ADC_FEEDBACK_CH);
    ADC_Start();

    //    TIMER_Create(&Prv.PIDtimer, true, ADC_Convert);
    //    TIMER_SetPeriod(&Prv.PIDtimer, 500);
    //    TIMER_Start(&Prv.PIDtimer);
}

void App_Process(void) {
    switch (Prv.mode) {
        case asPID:
        {
            break;
        }
        case asTEST:
        {

            break;
        }
    }
}