/*
 * switch_channel.c
 *
 *  Created on: Dec 12, 2024
 *      Author: utente
 */

#include "switch_channel.h"

// Configura un canale ADC per la lettura del dato
HAL_StatusTypeDef channel_config(ADC_HandleTypeDef *hadc, uint32_t channel) {
    ADC_ChannelConfTypeDef sConfig = {0};  // Struttura di configurazione del canale

    sConfig.Channel = channel;  // Seleziona il canale ADC
    sConfig.Rank = 1;           // Imposta il canale come primo nella sequenza di conversione
    sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;  // Imposta il tempo di campionamento

    if (HAL_ADC_ConfigChannel(hadc, &sConfig) != HAL_OK) {  // Configura il canale ADC
        return HAL_ERROR;  // Se la configurazione fallisce, ritorna errore
    }

    return HAL_OK;  // Ritorna OK se la configurazione è andata a buon fine
}

// Cambia il canale ADC e legge il valore
HAL_StatusTypeDef switch_channel_and_read(ADC_HandleTypeDef *hadc, uint32_t channel, uint16_t *d_out) {

    // Configura il canale selezionato
	if( channel_config(hadc, channel) != HAL_OK) {
		return HAL_ERROR;  // Se la configurazione del canale fallisce, ritorna errore
	}

	HAL_ADC_Start(hadc);  // Avvia la conversione ADC

    // Attende la fine della conversione
    if( HAL_ADC_PollForConversion(hadc, HAL_MAX_DELAY) == HAL_OK ) {

    	*d_out = HAL_ADC_GetValue(hadc);  // Legge il valore della conversione
    	HAL_ADC_Stop(hadc);  // Ferma la conversione
    	return HAL_OK;  // Ritorna OK se la lettura è andata a buon fine

    } else {
    	HAL_ADC_Stop(hadc);  // Ferma la conversione in caso di errore
    	return HAL_ERROR;  // Ritorna errore se la conversione non è riuscita
    }
}

