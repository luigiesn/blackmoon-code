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

#include <pic18f1330.h>

#include "../../include/driver/uart.h"
#include "../../include/ringbuffer.h"

#define RXBUFFER_SIZE 6 // bytes
#define TXBUFFER_SIZE 6 // bytes

typedef enum {
    usOPERATING
} UART_State;

static struct {
    UART_State state;

    ringbuffer_t rxRingbuffer;
    ringbuffer_t txRingbuffer;

    byte *buffer;
    byte elems;
} Prv;

static byte rxBuffer[RXBUFFER_SIZE];
static byte txBuffer[TXBUFFER_SIZE];

void UART_Boostrap() {
    // UART Interrupts
    IPR1bits.RCIP = 1; // high priority flag to rx interrupt
    PIE1bits.RCIE = 1; // enable uart received interrupt

    //    IPR1bits.TXIP = 1; // high priority flag to tx interrupt
    //    PIE1bits.TXIE = 1; // enable uart transmitted interrupt

    SPEN = 1; // Serial Port Enable bit
    CREN = 1; // Continuous Receive Enable bit

    RXDTP = 0; // Received Data Polarity Select bit(if RX = 1, data is inverted)
    SPBRGH = 0x03; // 9615bps
    SPBRG = 0x41; // 9615bps
    BRG16 = 1;
    BRGH = 1;

    TXEN = 1; // Transmision Enabled
    SYNC = 0; // async

    ringbufferInit(&Prv.rxRingbuffer, rxBuffer, RXBUFFER_SIZE);
    ringbufferInit(&Prv.txRingbuffer, txBuffer, TXBUFFER_SIZE);

    Prv.elems = 0;

    Prv.state = usOPERATING;
}

void UART_Process() {
    switch (Prv.state) {
        case usOPERATING:
        {
            if (!ringbufferEmpty(&Prv.txRingbuffer)) { // if output buffer is not empty
                byte i, elems;
                byte* buffer;

                elems = ringbufferGetElements(&Prv.txRingbuffer, &buffer);
                i = 0;

                while (i < elems) {
                    if (TXSTAbits.TRMT) {
                        TXREG = buffer[i]; // send byte then goto next position
                        i++;
                    }
                }
                ringbufferRemove(&Prv.txRingbuffer, elems); // remove sent data
            }
            break;
        }
    }
}

bool UART_Send(byte* data, byte size) {
    if (ringbufferFree(&Prv.txRingbuffer) < size) // if there is no free space
        return false;

    ringbufferAdd(&Prv.txRingbuffer, data, size);

    return true;
}

bool UART_SendByte(byte data) {
    if (!ringbufferFree(&Prv.txRingbuffer)) // if there is no free space
        return false;

    ringbufferAdd(&Prv.txRingbuffer, &data, 1);

    return true;
}

byte UART_DataOnInput() {
    return ringbufferCount(&Prv.rxRingbuffer);
}

byte UART_ReadData(byte** data) {
    byte dataCnt;

    dataCnt = ringbufferGetElements(&Prv.rxRingbuffer, data);

    return dataCnt;
}

void UART_RemoveData(byte amount) {
    ringbufferRemove(&Prv.rxRingbuffer, amount); // remove bytes;
}

void UART_ReceiveEventHandle(void) {
    byte data = RCREG;
    ringbufferAdd(&Prv.rxRingbuffer, &data, 1);
}


