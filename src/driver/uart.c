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

#include "../../include/driver/uart.h"
#include "../../include/ringbuffer.h"

#define RXBUFFER_SIZE 64 // bytes
#define TXBUFFER_SIZE 64 // bytes 

typedef enum {
    usREADY, usSEND
} UART_State;

static struct {
    UART_State state;

    ringbuffer_t rxRingbuffer;
    ringbuffer_t txRingbuffer;
    
    bool dataReceived;

    byte *buffer;
    byte elems;
    byte i_buffer;
} Prv;

static byte rxBuffer[RXBUFFER_SIZE];
static byte txBuffer[RXBUFFER_SIZE];

void UART_Boostrap() {
    // UART Interrupts
    PIE1bits.RCIE = 1; // enable uart received interrupt
    IPR1bits.RCIP = 1; // high priority interrupt

    PIE1bits.TXIE = 1; // enable uart transmitted interrupt
    IPR1bits.TXIP = 1; // high priority interrupt

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
    Prv.i_buffer = 0;

    Prv.state = usREADY;
}

void UART_Process() {
    switch (Prv.state) {
        case usREADY:
        {
            if(!ringbufferEmpty(&Prv.txRingbuffer)) // if output buffer is not empty
                Prv.state = usSEND;
            break;
        }
        case usSEND:
        {
            if (!Prv.elems) { // if elements number reaches to zero
                Prv.elems = ringbufferGetElements(&Prv.txRingbuffer, &Prv.buffer);
                Prv.i_buffer = 0;
                if (!Prv.elems) { // second verification to confirm buffer is empty
                    Prv.state = usREADY;
                    break;
                }
            }
            break;
        }
    }
}

bool UART_Send(byte* data, byte size){
    if (ringbufferFree(&Prv.txRingbuffer) < size) // if there is no free space
        return false;
    
    ringbufferAdd(&Prv.txRingbuffer, data, size);
   
    return true; 
}

bool UART_DataReceived(){
    return Prv.dataReceived;
}

byte UART_DataReceivedCnt(){    
    return ringbufferCount(&Prv.rxRingbuffer);
}

bool UART_ReadData(byte* data, byte size) {
    byte* elems;
    byte elemsCnt, i;
    
    elemsCnt = ringbufferGetElements(&Prv.rxRingbuffer, &elems);
    
    if(size > elemsCnt)
        return false;
    
    for(i = 0 ; i < size ; i++){ // copy
        data[i] = elems[i];
    }
    
    ringbufferRemove(&Prv.rxRingbuffer, elemsCnt); // remove readed bytes;
    return true;  
}

void UART_ReceiveEventHandle(void) {
    byte data = RCREG;
    ringbufferAdd(&Prv.rxRingbuffer, &data, 1);
    Prv.dataReceived = true;
}

void UART_TransmittedEventHandle(void) {
    if (Prv.i_buffer > Prv.elems) {
        Prv.elems = 0;
    }
    else{
        TXREG = Prv.buffer[Prv.i_buffer++]; // send byte then goto next position
    }
}

