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

void EEPROM_Boostrap(void);
void EEPROM_Init(void);
void EEPROM_Process(void);

void EEPROM_Write(byte *data, byte startAdress, byte length);
void EEPROM_Read(byte *data, byte startAdress, byte length);

void EEPROM_WriteByte(byte data, byte adress);
byte EEPROM_ReadByte(byte adress);

#endif	/* _EEPROM_H_ */

