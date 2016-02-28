/*
 * Serial driver - part of Blackmoon servo controller
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

#ifndef _SERIAL_H_
#define	_SERIAL_H_

#include <xc.h>

#include "../defs.h"

/* Protocol:
 * <--------------LENGTH---------------->
 * OPEN | PARAM |      DATA       | CLOSE
 *                <----16-bits--->
 */

/**
 * @brief Serial bootstrap
 *
 * Initialize all serial driver internal parameters
 */
void Serial_Boostrap(void);

/**
 * @brief processes that manage the transmission
 *
 */
void Serial_TxProcess(void);

/**
 * @brief processes that manage the reception
 *
 */
void Serial_RxProcess(void);

/**
 * @brief Send a data vector
 *
 * Check and put a data vector in output buffer
 *
 * @param data Vector to send
 * @param size Size of the vector to send
 *
 * @return Returns "true" if the data vector was added
 *
 * @warning The data is not sent imediately. There is an output buffer, so lag is expected
 */
bool Serial_Send(byte* data, byte size);

/**
 * @brief Send a packet (protocol format)
 *
 * Check and put a packet in output buffer
 *
 * @param p Parameter to be sent
 * @param value Value to be assigned to the parameter
 * @return Returns "true" if the packet vector was added
 */
bool Serial_SendPacket(byte p, UINT16 value);

/**
 * @brief Send a byte
 *
 * Check and put a byte in output buffer
 *
 * @param data
 * @return Returns "true" if the byte vector was added
 */
bool Serial_SendByte(byte data);

/**
 * @brief Get byte if data exists
 *
 * @param byte pointer to data
 *
 * @return Returns true if exists data in the input buffer
 */
bool Serial_ReadByte(byte* data);

/**
 * @brief Receive interrupt handler
 *
 * On receive interrupt, received byte is stored in input buffer
 */
void Serial_ReceiveEventHandle(void);

#endif	/* _SERIAL_H_ */

