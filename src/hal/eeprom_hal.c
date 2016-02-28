/*
 * EEPROM HAL - part of Blackmoon servo controller
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

#include "../../include/hal/eeprom_hal.h"

inline bool EEPROM_HAL_Init() {
    EECON1bits.CFGS = 0; // acess eeprom or flash mem.
    EECON1bits.EEPGD = 0; // acess eeprom mem.
}

inline void EEPROM_HAL_Interrupt(bool enable, bool high_prior) {
    // Interrupt configuration
    PIE2bits.EEIE = enable ? 1 : 0; // enable eeprom write interrupt
    IPR2bits.EEIP = high_prior ? 1 : 0; // high priority flag to eeprom write interrupt

    PIR2bits.EEIF = 0; // clean interrupt flag
}

inline void EEPROM_HAL_Write(byte data, byte address) {

    // if writing is not being performed
    EEADR = address; // set address
    EEDATA = data; // set data and increment

    EECON1bits.WREN = 1; // enable eeprom write
    INTCONbits.GIEH = 0; // disable interrupts
    INTCONbits.GIEL = 0; // disable interrupts

    /// unlock sequence
    asm("MOVLW 55h");
    asm("MOVWF EECON2");
    asm("MOVLW 0AAh");
    asm("MOVWF EECON2");
    asm("BSF EECON1, 1"); // write!

    INTCONbits.GIEH = 1; // enable interrupts
    INTCONbits.GIEL = 1; // enable interrupts
    EECON1bits.WREN = 0; // disable eeprom write
}

inline bool EEPROM_HAL_Writing() {
    return EECON1bits.WR ? true : false;
}

inline byte EEPROM_HAL_Read(byte address) {
    EEADR = address;
    RD = 1;
    return EEDATA;
}

inline bool EEPROM_HAL_Reading() {
    return EECON1bits.RD ? true : false;
}