/*
 * read_temperature.h
 *
 *  Created on: Nov 11, 2024
 *      Author: Martino Giaquinto
 */

#ifndef INC_READ_TEMPERATURE_H_
#define INC_READ_TEMPERATURE_H_

#include "adc.h"


#define S		2.5
#define	T0		25
#define	V0		760
#define	VREF	3340
#define	LEVELS	4096

HAL_StatusTypeDef read_temperature_c(uint16_t *d_out, float *voltage, float *temp);

#endif /* INC_READ_TEMPERATURE_H_ */
