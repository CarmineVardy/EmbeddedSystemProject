/*
 * read_temperature.c
 *
 *  Created on: Dec 5, 2024
 *      Author: utente
 */

#include "read_temperature.h"

void read_temperature(uint16_t *d_out, float *voltage, float *temp)
{
	float resistence;

	*voltage = ( (double) *d_out ) * VREF / LEVELS;

	resistence = (-RS*(*voltage))/(*voltage-VREF);

	*temp = (BETA)/(log(resistence/R0) + BETA/T0) - 273.15;

}
