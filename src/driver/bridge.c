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

#include "../../include/driver/bridge.h"

#define IN1_PIN 0
#define IN2_PIN 1
#define SD1_PIN 2
#define SD2_PIN 3

inline void Bridge_SetDuty(UINT16 duty);
inline bool Bridge_BreakEnabled(void);
inline void Bridge_OverridePwm(const unsigned char pin, bool value);
inline void Bridge_AllowPwm(const unsigned char pin);

void Bridge_Bootstrap() {
    PTCON0 = 0x00;

    PWMCON0 = 0x37; // All PWM I/O pins enabled for PWM output, PWM I/O pin pair is in the Independent mode

    PTMRL = 0x00; // cleans PWM timebase

    PTCON0bits.PTOPS = PWM_TIMEBASE_POSTSCALER;

    // disable all outputs
    Bridge_OverridePwm(IN1_PIN, false);
    Bridge_OverridePwm(IN2_PIN, false);
    Bridge_OverridePwm(SD1_PIN, false);
    Bridge_OverridePwm(SD2_PIN, false);

    OSYNC = 0; //Output overrides via the OVDCON register are synchronized to the PWM time base

    // config PWM period
    PTPERH = PWM_PERIOD >> 8; //  t = ((PTPER + 1) x PTMRPS)/(FOSC/4)
    PTPERL = PWM_PERIOD;

    // config intial duty-cicle (zero)
    PDC1L = 0;
    PDC1H = 0;

    PTCON1 |= 0x80; // run
}

void Bridge_Init(void) {
}

void Bridge_On(void) {
    Bridge_AllowPwm(SD1_PIN);
    Bridge_AllowPwm(SD2_PIN);
}

void Bridge_Off(void) {
    Bridge_OverridePwm(SD1_PIN, false);
    Bridge_OverridePwm(SD2_PIN, false);
    Bridge_OverridePwm(IN1_PIN, false);
    Bridge_OverridePwm(IN2_PIN, false);
}

void Bridge_SetOutput(BridgeDirection brgDir, UINT16 duty) {
    Bridge_SetDuty(duty);

    if (brgDir == bdForward) {
        Bridge_OverridePwm(SD2_PIN, 1); // bridge left side
        Bridge_OverridePwm(IN2_PIN, 0);

        Bridge_OverridePwm(IN1_PIN, 1); // bridge right side
        Bridge_AllowPwm(SD1_PIN);
    } else {
        Bridge_OverridePwm(SD1_PIN, 1); // bridge left side
        Bridge_OverridePwm(IN1_PIN, 0);

        Bridge_OverridePwm(IN2_PIN, 1); // bridge right side
        Bridge_AllowPwm(SD2_PIN);
    }
}

inline void Bridge_SetDuty(UINT16 duty) {
    if (duty > PWM_DUTYMAX) {
        duty = PWM_DUTYMAX;
    }
    PDC1L = duty;
    PDC1H = duty >> 8;
}

inline bool Bridge_BreakEnabled(void) {
    return !PORTBbits.RB7 ? true : false;
}

inline void Bridge_OverridePwm(const unsigned char pin, bool value) {
    OVDCOND &= ~(1UL << pin);
    OVDCONS = value ? (OVDCONS | (1UL << pin)) : (OVDCONS & (~(1UL << pin)));
}

inline void Bridge_AllowPwm(const unsigned char pin) {
    OVDCOND |= (1UL << pin);
}
