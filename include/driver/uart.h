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
 * Put a data vector in output buffer
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
 * @brief Count amount of data in input buffer
 * 
 * @return Returns how many bytes are in the input buffer
 */
byte UART_DataReceivedCnt(void);

/**
 * @brief Get data in input buffer
 * 
 * Copy bytes on input buffer to given bytes vector
 * 
 * @param data Vector to paste data
 * @param size Size vector to obtain
 * 
 * @return Returns "true" if the data vector was copied
 * 
 */
bool UART_ReadData(byte* data, byte size);

/**
 * @brief Receive interrupt handler
 * 
 * On receive interrupt, received byte is stored in input buffer
 */
void UART_ReceiveEventHandle(void);

/**
 * @brief Transmit interrupt handler
 * 
 * On transmit interrupt, transmit next byte on output buffer
 */
void UART_TransmittedEventHandle(void); 

#endif	/* _UART_H_ */

