/*
 * EEPROM driver - part of Blackmoon servo controller
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

#include "../../include/driver/eeprom.h"

#define EEPROM_DATA_OFFSET 16 //offset to protect system intial data

typedef enum {
    esIDLE, esUPDATE_RAM, esUPDATE_EEPROM
} eepromState;

static struct {
    eepromState state;

    bool imageModified;
} Prv;

static byte virtualEEPROM[VIRTUAL_EEPROM_SIZE];

void EEPROM_Boostrap() {
    EECON1bits.CFGS = 0; // acess eeprom or flash mem.
    EECON1bits.EEPGD = 0; // acess eeprom mem.

    // Interrupt configuration
    IPR2bits.EEIP = 0; // low priority flag to eeprom write interrupt
    PIE2bits.EEIE = 1; // enable eeprom write interrupt
    PIR2bits.EEIF = 0; // clean interrup flag

    Prv.imageModified = true;
    Prv.state = esUPDATE_RAM;
}

void EEPROM_Process() {
    switch (Prv.state) {
        case esIDLE:
        {
            if (Prv.imageModified) {
                Prv.imageModified = false;
                Prv.state = esUPDATE_EEPROM;
            }
            break;
        }
        case esUPDATE_RAM:
        {
            byte i;

            for (i = 0; i < VIRTUAL_EEPROM_SIZE; i++) { // send to the RAM all eeprom data
                EEADR = EEPROM_DATA_OFFSET + i;
                EECON1bits.RD = 1;
                virtualEEPROM[i] = EEDATA;
            }

            Prv.state = esIDLE;
            break;
        }
        case esUPDATE_EEPROM:
        {
            static byte i;

            if (!EECON1bits.WR) { // if writing is not being performed
                EEADR = EEPROM_DATA_OFFSET + i; // set address
                EEDATA = virtualEEPROM[i++]; // set data and increment

                EECON1bits.WREN = 1; // enable eeprom write
                INTCONbits.GIE = 0; // disable interrupts

                /// unlock sequence
                asm("MOVLW 55h");
                asm("MOVWF EECON2");
                asm("MOVLW 0AAh");
                asm("MOVWF EECON2");
                asm("BSF EECON1, WR"); // write!

                INTCONbits.GIE = 1; // enable interrupts
                EECON1bits.WREN = 0; // disable eeprom write
            }

            if (i == VIRTUAL_EEPROM_SIZE) {
                i = 0;
                Prv.state = esIDLE;
            }
            break;
        }
        default:
            break;
    }
}

void EEPROM_Write(byte* data, byte address) {
    if (*data != virtualEEPROM[address]) {
        virtualEEPROM[address] = (*data);
        Prv.imageModified = true;
    }
}

void EEPROM_Read(byte* data, byte address) {
    (*data) = virtualEEPROM[address];
}

