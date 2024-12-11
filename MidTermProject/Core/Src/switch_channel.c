/*
 * switch_channel.c
 *
 *  Created on: Dec 5, 2024
 *      Author: anton
 */

#include "switch_channel.h"

HAL_StatusTypeDef channel_config(ADC_HandleTypeDef *hadc, uint32_t channel, uint32_t sampling_time, int singleConvMode) {

	ADC_ChannelConfTypeDef sConfig = {0};

	hadc1.Instance = ADC1;
	hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4;
	hadc1.Init.Resolution = ADC_RESOLUTION_12B;
	hadc1.Init.ScanConvMode = DISABLE;

	if(singleConvMode){
		hadc1.Init.ContinuousConvMode = DISABLE;
		hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
	}
	else {
		hadc1.Init.ContinuousConvMode = ENABLE;
		hadc1.Init.EOCSelection = ADC_EOC_SEQ_CONV;
	}

	hadc1.Init.DiscontinuousConvMode = DISABLE;
	hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
	hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
	hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
	hadc1.Init.NbrOfConversion = 1;
	hadc1.Init.DMAContinuousRequests = DISABLE;

	if (HAL_ADC_Init(&hadc1) != HAL_OK)
	{
	  Error_Handler();
	  return HAL_ERROR;
	}


    sConfig.Channel = channel;
    sConfig.Rank = 1;
    sConfig.SamplingTime = sampling_time;

    if (HAL_ADC_ConfigChannel(hadc, &sConfig) != HAL_OK) {
        Error_Handler();
        return HAL_ERROR;
    }

    return HAL_OK;
}


// cambia canale e legge il valore ADC
HAL_StatusTypeDef switch_channel_and_read(uint16_t *d_out, ADC_HandleTypeDef *hadc, uint32_t channel, uint32_t sampling_time, int singleConvMode) {

	HAL_ADC_Stop(&hadc1);

	if( channel_config(hadc, channel, sampling_time, singleConvMode) != HAL_OK){
		return HAL_ERROR;
	}

	HAL_ADC_Start(&hadc1);


    if( HAL_ADC_PollForConversion(hadc, HAL_MAX_DELAY) == HAL_OK ){
    	*d_out = HAL_ADC_GetValue(hadc); // ritorna il valore letto

    	if(singleConvMode)
    		HAL_ADC_Stop(hadc);

    	return HAL_OK;
    }else{
    	HAL_ADC_Stop(hadc);
    	return HAL_ERROR;
    }

}

