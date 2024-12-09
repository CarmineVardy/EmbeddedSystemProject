/*
 * step_Counter.c
 *
 *  Created on: Dec 9, 2024
 *      Author: anton
 */

#include "step_Counter.h"

void read_forceSensor(uint16_t *d_out, float *resistence){

	float voltage;

	voltage = ( (double) *d_out ) * VREF / LEVELS;

	*resistence = (VREF*RM-voltage*RM)/voltage;


}
