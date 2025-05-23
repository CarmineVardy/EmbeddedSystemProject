/*
 * Project: Wearable System for Health Monitoring
 * Authors: Carmine Vardaro, Alessia Parente, Antonio D'Arienzo
 *
 * Licensed under the GNU General Public License v3.0 (GPL v3).
 * For more details, see: https://www.gnu.org/licenses/gpl-3.0.html
 *
 * Description:
 * This file contains the function implementations for configuring and switching ADC channels.
 * It includes functions for configuring an ADC channel for reading and switching between channels
 * to read the resulting data. These operations are fundamental for acquiring sensor data in the system.
 */

#include "switch_channel.h"

// Configures an ADC channel for reading data
// This function sets up the ADC channel configuration with the specified channel number
HAL_StatusTypeDef channel_config(ADC_HandleTypeDef *hadc, uint32_t channel) {
    ADC_ChannelConfTypeDef sConfig = {0};  // Structure to hold the ADC channel configuration

    sConfig.Channel = channel;  // Selects the ADC channel
    sConfig.Rank = 1;           // Sets the channel as the first in the conversion sequence
    sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;  // Sets the sampling time for the channel

    // Configures the ADC channel using the provided configuration
    if (HAL_ADC_ConfigChannel(hadc, &sConfig) != HAL_OK) {
        return HAL_ERROR;  // Returns an error if channel configuration fails
    }

    return HAL_OK;  // Returns OK if the channel configuration is successful
}

// Switches the ADC channel and reads the resulting data
// This function configures the selected ADC channel, starts the conversion,
// waits for the result, and reads the ADC value into the provided pointer
HAL_StatusTypeDef switch_channel_and_read(ADC_HandleTypeDef *hadc, uint32_t channel, uint16_t *d_out) {

    // Configures the selected ADC channel
    if( channel_config(hadc, channel) != HAL_OK) {
        return HAL_ERROR;  // Returns an error if channel configuration fails
    }

    HAL_ADC_Start(hadc);  // Starts the ADC conversion

    // Waits for the conversion to complete
    if( HAL_ADC_PollForConversion(hadc, HAL_MAX_DELAY) == HAL_OK ) {

        *d_out = HAL_ADC_GetValue(hadc);  // Reads the conversion value
        HAL_ADC_Stop(hadc);  // Stops the ADC conversion
        return HAL_OK;  // Returns OK if the read operation is successful

    } else {
        HAL_ADC_Stop(hadc);  // Stops the ADC conversion in case of error
        return HAL_ERROR;  // Returns error if the conversion fails
    }
}
