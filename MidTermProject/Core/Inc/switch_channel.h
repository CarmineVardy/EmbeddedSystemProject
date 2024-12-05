/*
 * switch_channel.h
 *
 *  Created on: Dec 5, 2024
 *      Author: anton
 */

#ifndef INC_SWITCH_CHANNEL_H_
#define INC_SWITCH_CHANNEL_H_



#include "adc.h"

HAL_StatusTypeDef channel_config(ADC_HandleTypeDef *hadc, uint32_t channel, uint32_t sampling_time);
uint16_t read_channel(ADC_HandleTypeDef *hadc, uint32_t channel, uint32_t sampling_time);


#endif /* INC_SWITCH_CHANNEL_H_ */
