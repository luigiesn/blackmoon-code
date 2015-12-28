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

#define VIRTUAL_EEPROM_SIZE 32 //bytes (up to 112 bytes)

#include <xc.h>
#include "../defs.h"

/**
 * @brief EEPROM bootstrap
 * 
 * Initialize all EEPROM driver internal parameters
 */
void EEPROM_Boostrap(void);

/**
 * @brief EEPROM process
 * 
 * At intialization all data on eeprom will be load to the virtual eeprom
 * When the virtual eeprom chnages all its data will be writen in the real eeprom.
 * 
 * EEPROM processing
 */
void EEPROM_Process(void);

/**
 * @brief Write data on the virtual EEPROM
 * 
 * @param data Pointer to the byte that will be stored
 * @param address Address to the byte that will be stored
 */
void EEPROM_Write(byte* data, byte address);

/**
 * @brief Read data from the virtual EEPROM
 * 
 * @param data Pointer to the byte that will be read
 * @param address Address to the byte that will be read
 */
void EEPROM_Read(byte* data, byte address);

#endif	/* _EEPROM_H_ */

