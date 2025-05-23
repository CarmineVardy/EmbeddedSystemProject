/*
 * Project: Wearable System for Health Monitoring
 * Authors: Carmine Vardaro, Alessia Parente, Antonio D'Arienzo
 *
 * Licensed under the GNU General Public License v3.0 (GPL v3).
 * For more details, see: https://www.gnu.org/licenses/gpl-3.0.html
 *
 * Description:
 * This file contains the function declarations for configuring and switching ADC channels.
 * It includes functions to configure a specific ADC channel for reading and to switch between channels
 * while reading the resulting data. These operations are essential for the accurate acquisition of sensor
 * data in the system.
 */

#ifndef INC_SWITCH_CHANNEL_H_
#define INC_SWITCH_CHANNEL_H_

#include "adc.h"

// Function to configure a specific ADC channel for reading
// This function sets the ADC channel configuration based on the specified channel number
HAL_StatusTypeDef channel_config(ADC_HandleTypeDef *hadc, uint32_t channel);

// Function to switch the ADC channel and read the resulting data
// This function switches to the specified ADC channel and stores the read value in the provided pointer
HAL_StatusTypeDef switch_channel_and_read(ADC_HandleTypeDef *hadc, uint32_t channel, uint16_t *d_out);

#endif /* INC_SWITCH_CHANNEL_H_ */
