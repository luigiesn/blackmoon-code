/*
 * User led driver - part of Blackmoon servo controller
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

#include "../../include/driver/led.h"

void LED_Bootstrap(void){
    ADCON1bits.PCFG2 = 1; // Port is configured as RA4
    
    LED_PORT_DIR &= (~LED_MASK);
    
    LED_Mode(ledsCONSTANT_ON);
}

void LED_Mode(led_mode_t mode){
    switch(mode){
        case ledsCONSTANT_OFF:
        {
            LED_ForceOff();
            break;
        }
        case ledsBLINK_SLOW:
        {
            break;
        }
        case ledsBLINK_FAST:
        {
            break;
        }
        case ledsCONSTANT_ON:
        {
            LED_ForceOn();
            break;
        }
    }
}
