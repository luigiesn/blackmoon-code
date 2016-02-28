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

#ifndef _EEPROM_H_
#define	_EEPROM_H_

#include <xc.h>
#include "../defs.h"
#include "../hal/eeprom_hal.h"

// eeprom definitions
#define EEPROM_PHYSICAL_SIZE EEPROM_HAL_MEM_SIZE

// virtual eeprom definitions
#define VIRTUAL_EEPROM_SIZE 32 // number of words
#define VIRTUAL_EEPROM_WSIZE 2 // bytes number of a word

/* Define types according to eeprom sizes */
#if VIRTUAL_EEPROM_WSIZE <= 1
typedef byte eeprom_data_t;
#elif VIRTUAL_EEPROM_WSIZE <= 2
typedef UINT16 eeprom_data_t;
#elif VIRTUAL_EEPROM_WSIZE <= 4
typedef UINT32 eeprom_data_t;
#else
#error VIRTUAL_EEPROM_SIZE must be less or equal than 4
#endif

#if VIRTUAL_EEPROM_SIZE <= 256
typedef byte eeprom_address_t;
#elif VIRTUAL_EEPROM_SIZE <= 65536
typedef UINT16 eeprom_address_t;
#elif VIRTUAL_EEPROM_SIZE <= 4294967295
typedef UINT32 eeprom_address_t;
#endif

/**
 * @brief EEPROM bootstrap
 *
 * Initialize all EEPROM driver internal parameters
 */
void EEPROM_Boostrap(void);

/**
 * @brief Initialize external dependencies
 */
void EEPROM_Init(void);

/**
 * @brief EEPROM process
 *
 * At intialization all data on eeprom will be load to the virtual eeprom
 * When the virtual eeprom chnages all its data will be writen in the real eeprom.
 *
 * EEPROM processing
 */
void EEPROM_Process(void);

void EEPROM_Write(eeprom_data_t data, eeprom_address_t address);

eeprom_data_t EEPROM_Read(eeprom_address_t address);

#endif	/* _EEPROM_H_ */

