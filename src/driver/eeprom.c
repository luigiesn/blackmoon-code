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

typedef enum{
    esIDLE, esWRITING, esWRITE
}eepromState;

static struct {
    eepromState state;
    
    byte addr;
    byte* buffer;
    byte length;
    
} Prv;

void EEPROM_Boostrap() {
    EECON1bits.CFGS = 0; // acess eeprom or flash mem.
    EECON1bits.EEPGD = 0; // acess eeprom mem.
}

void EEPROM_Init() {

}

void EEPROM_Process() {
    switch(Prv.state){
        case esWRITE:
        {
            byte i;
            
            for(i=0; i < Prv.length; i++) {
                EEADR = Prv.addr;
                EEDATA = Prv.buffer[i];
                EECON1bits.WREN = 1;
                INTCONbits.GIE = 0;

                /// unlock sequence
                asm("MOVLW 55h");
                asm("MOVWF EECON2");
                asm("MOVLW 0AAh");
                asm("MOVWF EECON2");
                asm("BSF EECON1, WR");

                INTCONbits.GIE = 1;
            }
            break;
        }
        case esWRITING:
        {
            if(!EECON1bits.WR)
                Prv.state = esIDLE;
            break;
        }
        default:
            break;
    }
}

void EEPROM_Write(byte *buffer, byte addr, byte length) {
    Prv.buffer = buffer;
    Prv.length = length;
    Prv.addr = addr;
    
    Prv.state = esWRITE;
}

void EEPROM_Read(byte *data, byte startAdress, byte length) {
    byte i;
    for (i = 0; i < length; i++) {
        EEADR = startAdress + i;
        EECON1bits.RD = 1; // start to read
        data[i] = EEDATA;
    }
}

void EEPROM_WriteByte(byte data, byte adress) {
    EEPROM_Write(&data, adress, 1);
}

byte EEPROM_ReadByte(byte adress) {
    byte data;
    EEPROM_Read(&data, adress, 1);
    return data;
}

