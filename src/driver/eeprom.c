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

/* define mem map size */
#define AUX_MAP_SIZE (EEPROM_PHYSICAL_SIZE/8)
#if EEPROM_PHYSICAL_SIZE % 8 != 0
    #define MAP_SIZE (AUX_MAP_SIZE+1)
#else
    #define MAP_SIZE (AUX_MAP_SIZE)
#endif

/* define mem max size */
#if (EEPROM_PHYSICAL_SIZE - 1 > 4294967295)
    #error EEPROM_PHYSICAL_SIZE must be less or equal than 4294967296 bytes
#endif

/* mem size verification */
#define EEPROM_USED_SPACE (VIRTUAL_EEPROM_SIZE*VIRTUAL_EEPROM_WSIZE)
#if EEPROM_USED_SPACE > EEPROM_PHYSICAL_SIZE
    #error The product of VIRTUAL_EEPROM_SIZE and VIRTUAL_EEPROM_WSIZE must be less or equal than EEPROM_PHYSICAL_SIZE
#endif

typedef enum {
    esIDLE, esLOAD_TO_RAM, esCHEKING_INTEGRITY
} eepromState;

static struct {
    eepromState state;
} Prv;

static byte virtualEEPROM[VIRTUAL_EEPROM_SIZE];
static byte eepromChangesMap[MAP_SIZE];

static inline void EEPROM_MarkChange(eeprom_address_t address);
static inline void EEPROM_UnmarkChange(eeprom_address_t address);

void EEPROM_Boostrap() {
    Prv.state = esLOAD_TO_RAM;
}

void EEPROM_Init(void) {
}

void EEPROM_Process() {
    switch (Prv.state) {
        case esIDLE:
        {
            break;
        }

        case esLOAD_TO_RAM:
        {
            static byte i = 0;

            if (!EEPROM_HAL_Reading()) {
                virtualEEPROM[i] = EEPROM_HAL_Read(i); /* Load eeprom to ram */
                i++;
            }
            if (i == EEPROM_USED_SPACE) {
                Prv.state = esCHEKING_INTEGRITY;
            }

            break;
        }

        case esCHEKING_INTEGRITY:
        {
            static byte mem_count = 0;
            byte temp_address, block_aux, byteoffset_aux;
            if (!EEPROM_HAL_Writing()) {

                mem_count++;
                if (mem_count == EEPROM_USED_SPACE) {
                    mem_count = 0;
                }

                byteoffset_aux = mem_count % 8;
                block_aux = mem_count >> 3; /* divide by eight */

                if (eepromChangesMap[block_aux] & (0b1 << byteoffset_aux)) { /* check for diferences */
                    temp_address = (block_aux * 8) + byteoffset_aux;

                    /* Refresh eeprom */
                    EEPROM_HAL_Write(virtualEEPROM[block_aux], temp_address);
                    EEPROM_UnmarkChange(temp_address);
                }
            }
            break;
        }

        default:
            break;
    }

}

void EEPROM_Write(eeprom_data_t data, eeprom_address_t address) {
    if (((eeprom_data_t*) virtualEEPROM)[address] != data) { // MSB | LSB
        ((eeprom_data_t*) virtualEEPROM)[address] = data;
        EEPROM_MarkChange(address);
    }
}

eeprom_data_t EEPROM_Read(eeprom_address_t address) {
    return (eeprom_data_t) ((eeprom_data_t*) virtualEEPROM)[address];
}

static inline void EEPROM_MarkChange(eeprom_address_t address) {
    eeprom_address_t block_aux, byteoffset_aux;

    /* Each bit in a byte represents one byte and
     * a group of 8 bits (a byte) represents a block.
     * i.e. a 128 bytes are divided in 16 blocks
     *
     * To mark a memory space in use, the block number and
     * bytes offset must be calculated.
     * i.e. if 34th byte is in use, the block is the 4th (34/8)
     *      and the byte offset is 2 (34%8).
     *          In this case: block_aux = 4
     *                        byteoffset_aux = 2
     */

    eeprom_address_t i;

    for (i = 0; i < sizeof (eeprom_data_t); i++) {
        block_aux = address >> 3; /* divide by eight */
        byteoffset_aux = address % 8;
        eepromChangesMap[block_aux] |= 0b1 << byteoffset_aux;

        address++;
    }
}

static inline void EEPROM_UnmarkChange(eeprom_address_t address) {
    eeprom_address_t block_aux, byteoffset_aux;

    /* Each bit in a byte represents one byte and
     * a group of 8 bits (a byte) represents a block.
     * i.e. a 128 bytes are divided in 16 blocks
     *
     * The unmarking procedure is similar to MarkChange()
     */

    eeprom_address_t i;

    for (i = 0; i < sizeof (eeprom_data_t); i++) {
        block_aux = address >> 3; /* divide by eight */
        byteoffset_aux = address % 8;
        eepromChangesMap[block_aux] &= ~(0b1 << byteoffset_aux);

        address++;
    }
}