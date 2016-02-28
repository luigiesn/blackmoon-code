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

#ifndef _EEPROM_HAL_H_
#define _EEPROM_HAL_H_

#include <xc.h>
#include "../defs.h"

#define EEPROM_HAL_MEM_SIZE 128

inline bool EEPROM_HAL_Init(void);

inline void EEPROM_HAL_Interrupt(bool enable, bool high_prior);

inline void EEPROM_HAL_Write(byte data, byte address);

inline bool EEPROM_HAL_Writing(void);

inline byte EEPROM_HAL_Read(byte address);

inline bool EEPROM_HAL_Reading(void);

#endif /* _EEPROM_HAL_H_ */