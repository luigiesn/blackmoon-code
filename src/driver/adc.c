/*
 * PIC18F1330 ADC driver - part of Blackmoon servo controller
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

#include <pic18f1330.h>

#include "../../include/driver/adc.h"

static struct {
    UINT16 conversionBuffer;
    bool newSample;
} Prv;

void ADC_Bootstrap(void) {
#if ADC_TRG_SRC == PWM_TIME_BASE
    ADCON0 = 0x80; // triggered by PWM time base, adc off
    PWMCON1bits.SEVOPS = PWM_TIME_BASE_POSTSCALER - 1;
#endif // ADC_TRG_SRC == PWM_TIME_BASE

#if ADC_TRG_SRC == MANUAL
    ADCON0 = 0x00; // manually triggered, adc off
#endif // ADC_TRG_SRC == MANUAL

    ADCON1bits.PCFG = 0x04; // AN0, AN1 and AN3 are active as analog input
    ADCON2 = 0xba; // 32 TAD(total), TAD = TOSC*32

    // Interrupt configuration
    IPR1bits.ADIP = 0; // low priority flag to ADC interrupt
    PIE1bits.ADIE = 1; // enable ADC interrupt
    PIR1bits.ADIF = 0; // clean ADC flag

    Prv.newSample = false;
}

void ADC_Convert(void) {
    if (ADCON0bits.GO == 0)
        ADCON0bits.GO = 1;
}

void ADC_SelectChannel(byte channel) {
    ADCON0bits.CHS = channel;
    Prv.newSample = false;
}

void ADC_Start(void) {
    ADCON0 |= 0x01;
}

void ADC_Stop(void) {
    ADCON0 &= 0xfe;
}

UINT16 ADC_Read(void) {
    Prv.newSample = false;
    return Prv.conversionBuffer;
}

bool ADC_NewSample(void) {
    return Prv.newSample;
}

void ADC_ConversionDoneEventHandle(void) {
    Prv.conversionBuffer = (UINT16) ADRES;
    Prv.newSample = true;
}

