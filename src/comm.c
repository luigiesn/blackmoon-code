/*
 * Communication module - part of Blackmoon servo controller
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

#include "../include/comm.h"
#include "../include/driver/uart.h"

#define PROTOCOL_LENGTH 5 //bytes
#define START_BYTE 0x30
#define END_BYTE 0x35

typedef enum {
    cpsSTART_VERIFICATION, cpsSTARTED, cpsPARAM, cpsVALUE, cpsSTOP_VERIFICATION
} COMM_ParserState_t;

typedef enum {
    csCHECK, csINTERPRETING
} COMM_State_t;

static struct {
    COMM_State_t state;
    COMM_ParserState_t parserState;
} Prv;

void COMM_Process(void) {
    switch (Prv.state) {
        case csCHECK:
        {
            byte* dataVec;
            byte totalCnt, instantCnt;
            byte i, dataToRemove = 0;
            byte* preValidData;

            totalCnt = UART_DataOnInput();

            if (totalCnt >= PROTOCOL_LENGTH) { // received a frame?
                instantCnt = UART_ReadData(&dataVec); // count

                while (i < instantCnt) {
                    switch (Prv.parserState) {
                        case cpsSTART_VERIFICATION:
                        {
                            if (dataVec[i] == START_BYTE) { // fisrt step to be a valid data
                                Prv.parserState = cpsSTARTED;
                            } else dataToRemove++;
                            break;
                        }
                        case cpsSTARTED:
                        {
                            preValidData = &(dataVec[i]); // hold a possible valid data
                            i = i + 2; // jump to the last verification (STOP_VERIFICATION)
                            Prv.parserState = cpsSTOP_VERIFICATION;
                            break;
                        }
                        case cpsSTOP_VERIFICATION:
                        {
                            if (dataVec[i] == END_BYTE) {
                                /////////////////////////////////
                                // now, you got a valid param. value
                                // DO THE ACTION(EEPROM_Write())!
                                /////////////////////////////////
                                dataToRemove = dataToRemove + PROTOCOL_LENGTH;
                                Prv.parserState = cpsSTART_VERIFICATION;
                            }
                            break;
                        }
                    }
                    i = i + 1;
                    if (i == instantCnt) { // verify if buffer is empty
                        UART_RemoveData(dataToRemove); // remove read data
                        instantCnt = UART_ReadData(&dataVec); // count data on buffer
                        if (instantCnt == 0) //if empty, break
                            break;
                        else // if not, restart count
                            i = 0;
                    }
                }
            }
            break;
        }
        default:
            break;
    }
}

bool COMM_SendParameter(COMM_Param_t p, UINT16 value) {
    bool ret;

    byte protocol[PROTOCOL_LENGTH] = {START_BYTE, 0, 0, 0, END_BYTE};

    protocol[1] = (byte) p; // parameter ID
    protocol[2] = (value >> 8) & 0xff; // parameter higher value
    protocol[3] = value & 0xff; // parameter lower value

    ret = UART_Send(protocol, PROTOCOL_LENGTH); // send pack

    return ret;
}

