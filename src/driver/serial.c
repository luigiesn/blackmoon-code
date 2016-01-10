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
#include "../../include/driver/led.h"

#define PROTOCOL_LENGTH 5 //bytes
#define START_BYTE 0x30
#define END_BYTE 0x35

#define RXBUFFER_SIZE 5 // bytes
#define TXBUFFER_SIZE 6 // bytes

typedef enum {
    usOPERATING
} UART_State;

typedef struct {
    byte parameter;
    UINT16 value;
} BoardParam;

static struct {
    UART_State state;

    ringbuffer_t txRingbuffer;

    byte rxCnt;
    byte readCnt;

    byte *buffer;
    byte elems;
} Prv;

static byte rxBuffer[RXBUFFER_SIZE];
static byte txBuffer[TXBUFFER_SIZE];

void Serial_Boostrap() {
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

    ringbufferInit(&Prv.txRingbuffer, txBuffer, TXBUFFER_SIZE);

    Prv.elems = 0;

    Prv.state = usOPERATING;
}

void Serial_TxProcess() {
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

void Serial_RxProcess(void) {
    static byte readCnt;
    static BoardParam input;
    byte temp;

    if (Serial_ReadByte(&temp)) {
        if (temp == START_BYTE || readCnt) {
            if (readCnt == 1) {
                input.parameter = temp;
            } else if (readCnt == 2) {
                input.value = temp;
            } else if (readCnt == 3) {
                input.value += (temp << 8);
            }
            readCnt++;
        }
        if (readCnt == PROTOCOL_LENGTH) {
            readCnt = 0;
            if (input.parameter == BOARD_STATUS) {
                if (input.value) LED_Mode(ledsCONSTANT_ON);
                else LED_Mode(ledsCONSTANT_OFF);
            }
        }
    }
}

bool Serial_Send(byte* data, byte size) {
    if (ringbufferFree(&Prv.txRingbuffer) < size) // if there is no free space
        return false;

    ringbufferAdd(&Prv.txRingbuffer, data, size);

    return true;
}

bool Serial_SendPacket(byte p, UINT16 value) {
    bool ret;
    byte packet[PROTOCOL_LENGTH] = {START_BYTE, 0, 0, 0, END_BYTE};

    if (ringbufferFree(&Prv.txRingbuffer) < PROTOCOL_LENGTH) // if there is no free space
        return false;

    packet[1] = p; // parameter ID
    packet[2] = (value >> 8) & 0xff; // parameter higher value
    packet[3] = value & 0xff; // parameter lower value

    ringbufferAdd(&Prv.txRingbuffer, packet, PROTOCOL_LENGTH);

    return true;
}

bool Serial_SendByte(byte data) {
    if (!ringbufferFree(&Prv.txRingbuffer)) // if there is no free space
        return false;

    ringbufferAdd(&Prv.txRingbuffer, &data, 1);

    return true;
}

bool Serial_ReadByte(byte* data) {
    if (Prv.readCnt < Prv.rxCnt) {
        *data = rxBuffer[Prv.readCnt];
        Prv.readCnt++;
        if (Prv.readCnt == Prv.rxCnt) {
            Prv.readCnt = 0;
            Prv.rxCnt = 0;
        }
        return true;
    }

    return false;
}

void Serial_ReceiveEventHandle(void) {
    if (Prv.rxCnt < RXBUFFER_SIZE) {
        rxBuffer[Prv.rxCnt] = RCREG;
        Prv.rxCnt++;
    }
}


