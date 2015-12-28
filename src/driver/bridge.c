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

typedef enum {
    brgsIDLE, brgsCHANGE_DIR
} BRG_State;

static struct {
    BRG_State state;

    UniDimVector bridgeDir;

} Prv;

void BRG_Boostrap() {
    BRG_Off();
}

void BRG_Init() {
    Prv.state = brgsIDLE;
}

void BRG_Process() {
    switch (Prv.state) {
        case brgsIDLE:
        {
            break;
        }
        case brgsCHANGE_DIR:
        {
            if (Prv.bridgeDir == DIR_A) { // DIR_A
                BRIDGE_PWM1_OVERRIDE_VALUE = 1; // override value from left-side driver input
                BRIDGE_PWM1_STATE(INACTIVE); // deactivate left-side driver PWM
                BRIDGE_IN1(0); // turn on lower mosfet

                BRIDGE_PWM2_STATE(ACTIVE);
                BRIDGE_IN2(1); // turn-on PWM on upper mosfet on right-side driver
            } else if (Prv.bridgeDir == DIR_B) { // DIR_B
                BRIDGE_PWM2_OVERRIDE_VALUE = 1;
                BRIDGE_PWM2_STATE(INACTIVE);
                BRIDGE_IN2(0);

                BRIDGE_PWM1_STATE(ACTIVE);
                BRIDGE_IN1(1);
            } else if (HW_BreakEnabled() && (Prv.bridgeDir == NEUTRAL)) {
                BRG_LoadShortCircuit();
            }
            Prv.state = brgsIDLE;
            break;
        }
    }
}

void BRG_DirSet(UniDimVector brgDir) {
    Prv.bridgeDir = brgDir;
    Prv.state = brgsCHANGE_DIR;
}

UniDimVector BRG_DirGet() {
    return Prv.bridgeDir;
}

void BRG_LoadShortCircuit() {
    BRIDGE_IN1(0);
    BRIDGE_IN2(0);
    BRIDGE_PWM1_OVERRIDE_VALUE = 1;
    BRIDGE_PWM2_OVERRIDE_VALUE = 1;
    BRIDGE_PWM1_STATE(INACTIVE);
    BRIDGE_PWM2_STATE(INACTIVE);
}

void BRG_On() {
    OVDCOND = 0xFF; // todos em override
}

void BRG_Off() {
    OVDCOND = 0x00; // todos em override
    OVDCONS = 0x00; // valor de override do PWM, todos em baixo
}
