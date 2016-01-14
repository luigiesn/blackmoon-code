/*
 * PIC18F1330 ADC driver - part of Blackmoon servo controller
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

#ifndef _ADC_H_
#define	_ADC_H_

#include <xc.h>
#include "../defs.h"

#define PWM_TIME_BASE 0
#define MANUAL 1

#define ADC_TRG_SRC PWM_TIME_BASE

#define PWM_TIME_BASE_POSTSCALER 16 // 1 to 16

void ADC_Bootstrap(void);

/**
 * @brief Force conversion
 */
void ADC_Convert(void);

/**
 * @brief Selects ADC channel to do the conversion
 *
 * Select the new channel and set as "false" the NewSample flag
 *
 * @param channel Channel number to be selected
 */
void ADC_SelectChannel(byte channel);

/**
 * @brief Start the conversion
 */
void ADC_Start(void);

/**
 * @brief Stop the conversion
 */
void ADC_Stop(void);

/*
 * @brief Get last readed value
 *
 * Read the last acquired value and set the NewSample flag as "false"
 *
 * @return Returns the conversion result
 */
UINT16 ADC_Read(void);

/*
 * @brief Check there is a new sample
 *
 * @return Returns "true" if data is new
 */
bool ADC_NewSample(void);

/*
 * @brief Handle the conversion done event
 *
 * Read the conversion result and store on internal buffer (1 sample buffer).
 * When it is called the NewSample flag will be set as "true".
 */
void ADC_ConversionDoneEventHandle(void);

#endif	/* _ADC_H_ */

