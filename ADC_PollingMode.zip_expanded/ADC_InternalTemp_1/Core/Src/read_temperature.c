/*
 * read_temperature.c
 *
 *  Created on: Nov 11, 2024
 *      Author: Martino Giaquinto
 */


#include "read_temperature.h"

HAL_StatusTypeDef read_temperature(uint16_t *d_out, float *voltage, float *temp)
{
    // Start conversion
    HAL_ADC_Start(&hadc1);

    // Polling
    if (HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY) == HAL_OK)
    {
        //Get the converted value
        *d_out = HAL_ADC_GetValue(&hadc1);
        HAL_ADC_Stop(&hadc1);

        // Evaluate voltage and temperature
        *voltage = (*d_out) * VREF / LEVELS;
        *temp = (*voltage - V0) / S + T0;

        return HAL_OK;
    }
    else
    {
    	HAL_ADC_Stop(&hadc1);
    	return HAL_ERROR;
    }
}
