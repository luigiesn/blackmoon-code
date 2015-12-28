/*
 * Main Blackmoon-servo application - part of Blackmoon servo controller
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

#ifndef _BLACKMOONAPP_H_
#define	_BLACKMOONAPP_H_

#include <xc.h>

/**
 * @brief Blackmoon-servo program
 * 
 * It controls the Blackmoon board as a servo controler. 
 * This code uses PID to control the motor shaft rotation angle. Feedback is given by a 
 * potentiometer coupled to the shaft.
 */
void BlackmoonServo(void);

#endif	/* _BLACKMOONAPP_H_ */

