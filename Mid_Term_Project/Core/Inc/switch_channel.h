/*
 * switch_channel.h
 *
 *  Created on: Dec 12, 2024
 *      Author: utente
 */

#ifndef INC_SWITCH_CHANNEL_H_
#define INC_SWITCH_CHANNEL_H_

#include "adc.h"

// Configura un canale ADC specifico per la lettura
HAL_StatusTypeDef channel_config(ADC_HandleTypeDef *hadc, uint32_t channel);

// Commutazione del canale ADC e lettura del dato risultante
HAL_StatusTypeDef switch_channel_and_read(ADC_HandleTypeDef *hadc, uint32_t channel, uint16_t *d_out);


#endif /* INC_SWITCH_CHANNEL_H_ */
