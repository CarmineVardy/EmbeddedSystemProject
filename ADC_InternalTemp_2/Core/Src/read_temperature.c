/*
 * read_temperature.c
 *
 *  Created on: Nov 11, 2024
 *      Author: Martino Giaquinto
 */


#include "read_temperature.h"
#include "stdio.h"

#define MAX_TS		100
#define	N			5
#define	TOLERANCE	5
#define	WAIT		MAX_TS/N - TOLERANCE

//WARNING: IT IS A BLOCKING FUNCTION

HAL_StatusTypeDef read_temperature_c(uint16_t *d_out, float *voltage, float *temp)
{
	uint32_t t , delay, sum = 0;

	for (uint8_t i = 0; i < N; i++)
	{
		t = HAL_GetTick();

		if (HAL_ADC_PollForConversion(&hadc1, WAIT) == HAL_OK)
		{
			sum += HAL_ADC_GetValue(&hadc1);
            delay = WAIT - (HAL_GetTick() - t);
            if (delay > 0)
                HAL_Delay(delay);
		}
	    else
	    {
	        return HAL_ERROR;
	    }

	}
		// Calculate average value
        *d_out = sum / N;

        // Evaluate voltage and temperature
        *voltage = (*d_out) * VREF / LEVELS;
        *temp = (*voltage - V0) / S + T0;

        return HAL_OK;
}
