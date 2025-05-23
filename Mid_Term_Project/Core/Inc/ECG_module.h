/*
 * Project: Wearable System for Health Monitoring
 * Authors: Carmine Vardaro, Alessia Parente, Antonio D'Arienzo
 *
 * Licensed under the GNU General Public License v3.0 (GPL v3).
 * For more details, see: https://www.gnu.org/licenses/gpl-3.0.html
 *
 * Description:
 * This file contains the declarations of functions and structures for processing ECG signals.
 * It includes functions for filtering the ECG signal, detecting peaks, and calculating heart rate (HR)
 * and heart rate variability (HRV). The code also provides utilities to manage ECG data buffers
 * and reset or initialize the ECG system.
 */

#ifndef INC_ECG_MODULE_H_
#define INC_ECG_MODULE_H_

#include "adc.h"

// Constants definitions
#define MAX_ARRAY_DIM 100  // Maximum size of ECG data buffers
#define TRESHOLDPEAKS 0.8  // Threshold for peak detection (in %)

// Structure for ECG filter parameters
typedef struct {
    uint16_t* buffer;  // Filtered ECG data buffer
    float sum;         // Sum of values in the buffer (used for filter calculation)
    uint16_t index;    // Current index in the buffer
    uint16_t length;   // Buffer length
    uint16_t filled;   // Number of actual data points in the buffer
} FilterECGParam;

// Structure for ECG parameters
typedef struct {
    uint16_t* buffer;  // Raw ECG data buffer
    uint16_t count;    // Number of ECG samples collected
    uint16_t length;   // Buffer length
    uint8_t HR;        // Heart Rate (HR)
    float HRV;         // Heart Rate Variability (HRV)
} ECGParam;

// Function to initialize the ECG filter
void ECG_init_Filter(FilterECGParam* FilterECGparam, uint16_t* buffer, uint16_t length);

// Function to initialize ECG parameters
void ECG_init_ECG(ECGParam* ECGparam, uint16_t* buffer, uint16_t length);

// Function to apply the filter to the ECG signal
void filter_signal(FilterECGParam* FilterECGparam, ECGParam* ECGparam, uint16_t d_out);

// Function to process ECG data and calculate HR and HRV
HAL_StatusTypeDef elab_ECG(ECGParam* ECGparam, uint16_t samp_freq);

// Function to find peaks in the ECG signal
HAL_StatusTypeDef find_peaks(ECGParam* ECGparam, uint16_t *peaks, uint8_t *num_peaks);

// Function to find the maximum value in an ECG buffer
uint16_t find_max(uint16_t *buffer, uint16_t length);

// Function to find the minimum value in an ECG buffer
uint16_t find_min(uint16_t *buffer, uint16_t length);

// Function to reset ECG filter parameters and data
void reset_ECG(FilterECGParam* FilterECGparam, ECGParam* ECGparam);

#endif /* INC_ECG_MODULE_H_ */
