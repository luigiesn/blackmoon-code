/*
 * UART module driver - part of Blackmoon servo controller
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

#ifndef _UART_H_
#define	_UART_H_

#include <xc.h>

#include "../defs.h"

/**
 * @brief UART bootstrap
 * 
 * Initialize all UART driver internal parameters
 */
void UART_Boostrap(void);

/**
 * @brief UART process
 * 
 * UART processing
 */
void UART_Process(void);

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
bool UART_Send(byte* data, byte size);

/**
 * @brief Send a byte
 * 
 * Check and put a byte in output buffer
 * 
 * @param data
 * @return Returns "true" if the byte vector was added
 */
bool UART_SendByte(byte data);

/**
 * @brief Count total amount of data on buffer
 * 
 * @return Number of byte on input buffer
 */
byte UART_DataOnInput(void);

/**
 * @brief Count amount and get the readable data in the input buffer
 * 
 * @param data pointer to a vector
 * 
 * @return Returns how many readable bytes are in the input buffer
 */
byte UART_ReadData(byte** data);

/**
 * @brief Remove data from input buffer
 * 
 * @param amount Amount of data to remove from input buffer
 */
void UART_RemoveData(byte amount);

/**
 * @brief Receive interrupt handler
 * 
 * On receive interrupt, received byte is stored in input buffer
 */
void UART_ReceiveEventHandle(void);

#endif	/* _UART_H_ */

