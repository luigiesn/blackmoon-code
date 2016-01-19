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

typedef enum {
    esINIT_VIRTUAL_EEPROM, esVERIFY_DIFF
} eepromState;

static struct {
    eepromState state;
} Prv;

static byte virtualEEPROM[VIRTUAL_EEPROM_SIZE];

inline byte EEPROM_Read(byte address);
inline bool EEPROM_Write(byte data, byte address);
void EEPROM_UpdateByte(void);

void EEPROM_Boostrap() {
    EECON1bits.CFGS = 0; // acess eeprom or flash mem.
    EECON1bits.EEPGD = 0; // acess eeprom mem.

    // Interrupt configuration
    IPR2bits.EEIP = 0; // low priority flag to eeprom write interrupt
    PIE2bits.EEIE = 0; // enable eeprom write interrupt
    PIR2bits.EEIF = 0; // clean interrup flag

    Prv.state = esINIT_VIRTUAL_EEPROM;
}

void EEPROM_Init(void) {
}

void EEPROM_Process() {
    switch (Prv.state) {
        case esINIT_VIRTUAL_EEPROM:
        {
            byte i;

            for (i = 0; i < VIRTUAL_EEPROM_SIZE; i++) { // send to the RAM all eeprom data
                virtualEEPROM[i] = EEPROM_Read(i + EEPROM_OFFSET);
            }
            Prv.state = esVERIFY_DIFF;
            break;
        }
        case esVERIFY_DIFF:
        {
            byte i, i_aux;

            // check eeprom for changes
            for (i = 0; i < VIRTUAL_EEPROM_SIZE; i++) {
                for (i_aux = 0x80; i_aux != 0; i_aux = i_aux >> 1) {
                    if (EEPROM_Read(EEPROM_OFFSET + i) != virtualEEPROM[i]) {
                        EEPROM_Write(virtualEEPROM[i], EEPROM_OFFSET + i);
                    }
                }
            }
            break;
        }
        default:
            break;
    }
}

void EEPROM_VirtualWrite8(byte* data, byte address) {
    virtualEEPROM[address] = (*data);

}

void EEPROM_VirtualWrite16(UINT16* data, byte address) {
    virtualEEPROM[address] = (*data) >> 8; // MSB | LSB
    virtualEEPROM[address + 1] = (*data);

}

void EEPROM_VirtualRead8(byte* data, byte address) {
    (*data) = virtualEEPROM[address];
}

void EEPROM_VirtualRead16(UINT16* data, byte address) {
    (*data) = virtualEEPROM[address + 1];
    (*data) = virtualEEPROM[address] << 8;
}

inline bool EEPROM_Write(byte data, byte address) {
    if (EECON1bits.WR)
        return false;

    // if writing is not being performed
    EEADR = address; // set address
    EEDATA = virtualEEPROM[address]; // set data and increment

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

    return true;
}

inline byte EEPROM_Read(byte address) {
    EEADR = address;
    RD = 1;
    return EEDATA;
}