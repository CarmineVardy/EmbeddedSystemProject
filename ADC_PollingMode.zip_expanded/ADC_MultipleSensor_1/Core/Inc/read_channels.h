/*
 * read_channels.h
 *
 *  Created on: Nov 18, 2024
 *      Author: Martino Giaquinto
 */

#ifndef INC_READ_CHANNELS_H_
#define INC_READ_CHANNELS_H_

#include "adc.h"

HAL_StatusTypeDef ADC_channel_user_config(ADC_HandleTypeDef *hadc, uint32_t channel, uint32_t sampling_time);
uint16_t ADC_read_channel(ADC_HandleTypeDef *hadc, uint32_t channel, uint32_t sampling_time);

#endif /* INC_READ_CHANNELS_H_ */
