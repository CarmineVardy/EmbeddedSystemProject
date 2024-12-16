/*
 * ECG_module.h
 *
 *  Created on: Dec 15, 2024
 *      Author: utente
 */

#ifndef INC_ECG_MODULE_H_
#define INC_ECG_MODULE_H_

#include "adc.h"

// Definizioni delle costanti
#define MAX_ARRAY_DIM 100  // Dimensione massima degli array per i buffer ECG
#define TRESHOLDPEAKS 0.75  // Soglia per la rilevazione dei picchi (70% della differenza tra valore massimo e minimo)

// Struttura per i parametri del filtro ECG
typedef struct {
    uint16_t* buffer;  // Buffer dei dati ECG filtrati
    float sum;         // Somma dei valori nel buffer (utilizzato per il calcolo del filtro)
    uint16_t index;    // Indice corrente nel buffer
    uint16_t length;   // Lunghezza del buffer
    uint16_t filled;   // Numero di valori effettivamente presenti nel buffer
} FilterECGParam;

// Struttura per i parametri ECG
typedef struct {
    uint16_t* buffer;  // Buffer dei dati ECG grezzi
    uint16_t count;    // Numero di campioni ECG acquisiti
    uint16_t length;   // Lunghezza del buffer
    uint8_t HR;        // Frequenza cardiaca (Heart Rate)
    float HRV;         // Variabilità della frequenza cardiaca (Heart Rate Variability)
} ECGParam;

// Funzione di inizializzazione del filtro ECG
void ECG_init_Filter(FilterECGParam* FilterECGparam, uint16_t* buffer, uint16_t length);

// Funzione di inizializzazione dei parametri ECG
void ECG_init_ECG(ECGParam* ECGparam, uint16_t* buffer, uint16_t length);

// Funzione per applicare il filtro al segnale ECG
void filter_signal(FilterECGParam* FilterECGparam, ECGParam* ECGparam, uint16_t d_out);

// Funzione per elaborare i dati ECG e calcolare HR e HRV
HAL_StatusTypeDef elab_ECG(ECGParam* ECGparam, uint16_t samp_freq);

// Funzione per trovare i picchi nel segnale ECG
HAL_StatusTypeDef find_peaks(ECGParam* ECGparam, uint16_t *peaks, uint8_t *num_peaks);

// Funzione per trovare il valore massimo in un buffer ECG
uint16_t find_max(uint16_t *buffer, uint16_t length);

// Funzione per trovare il valore minimo in un buffer ECG
uint16_t find_min(uint16_t *buffer, uint16_t length);

// Funzione per resettare i parametri del filtro ECG e i dati ECG
void reset_ECG(FilterECGParam* FilterECGparam, ECGParam* ECGparam);


#endif /* INC_ECG_MODULE_H_ */
