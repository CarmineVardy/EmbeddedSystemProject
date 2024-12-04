/*
 * ecgmodule.c
 *
 *  Created on: Nov 24, 2024
 *      Author: Martino Giaquinto
 */


#include <ECGmodule.h>
#include <stdio.h>
#include <math.h>

HAL_StatusTypeDef read_ECG(uint16_t *d_out, float *voltage)
{

    // Polling
    if (HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY) == HAL_OK)
    {
        //Get the converted value
        *d_out = HAL_ADC_GetValue(&hadc1);

        // Evaluate voltage and temperature
        *voltage = (*d_out) * VREF / LEVELS;

        return HAL_OK;
    }
    else
    {
    	return HAL_ERROR;
    }
}

uint16_t filter_signal(FilterParam* filter, uint16_t new_value)
{
    filter->sum = filter->sum - filter->buffer[filter->index] + new_value;
    filter->buffer[filter->index] = new_value;
    filter->index = (filter->index + 1) % filter->length;


    if (filter->filled < filter->length)
    {
        filter->filled++;
    }

    return filter->sum / filter->filled;

}

HAL_StatusTypeDef filter_init(FilterParam* filter, uint16_t* buffer, uint16_t length)
{
    if (filter == NULL || length == 0) {
        return HAL_ERROR;
    }

    filter->buffer = buffer;
    filter->sum = 0;
    filter->index = 0;
    filter->length = length;
    filter->filled = 0;

    for (uint16_t i = 0; i < length; i++)
    {
        buffer[i] = 0;
    }

    return HAL_OK;
}

uint16_t find_max(uint16_t *buffer, uint16_t length)
{
	//Find the absolute maximum in a signal window

    if (length == 0) {
        return 0;
    }

    uint16_t max = buffer[0];
    for (uint16_t i = 1; i < length; i++) {
        if (buffer[i] > max) {
            max = buffer[i];
        }
    }
    return max;
}

uint16_t find_min(uint16_t *buffer, uint16_t length)
{
	//Find the absolute minimum in a signal window

    if (length == 0) {
        return 0;
    }

    uint16_t min = buffer[0];
    for (uint16_t i = 1; i < length; i++) {
        if (buffer[i] < min) {
            min = buffer[i];
        }
    }
    return min;
}

HAL_StatusTypeDef find_peaks(uint16_t *buffer, uint16_t length, uint16_t *peaks, uint8_t *num_peaks)
{
    // Check if the buffer length is too small to contain peaks
    if (length < 3) {
        *num_peaks = 0;
        return HAL_ERROR;
    }

    // Calculate the threshold as 75% of the maximum value in the buffer
    uint16_t max_value = find_max(buffer, length);
    uint16_t min_value = find_min(buffer, length);
    uint16_t th = (uint16_t)(TRESHOLDPEAKS * (max_value-min_value)) + min_value; // Threshold value

    uint8_t count = 0; // Counter for the number of peaks found

    for (uint16_t i = 1; i < length - 1; i++) {
        // Check for a peak
        if ((buffer[i] > th) && (buffer[i] > buffer[i - 1]) && (buffer[i] > buffer[i + 1]))
        {
            peaks[count] = i; // Store the index of the peak

            //printf("\rP: %u\n", peaks[count]);

            count++;

            // Limit the number of peaks to prevent buffer overflow
            if (count >= MAX_ARRAY_DIM)
            {
                break;
            }
        }
    }

    *num_peaks = count;
    return HAL_OK;
}

HAL_StatusTypeDef elab_ECG(uint16_t *ECG_window, uint16_t samp_freq, uint16_t length, uint8_t *HR)
{
    // Array to store peak indices, adjusted to uint16_t
    uint16_t peaks[MAX_ARRAY_DIM];
    uint8_t num_peaks = 0;

    // Detect peaks in the ECG window
    find_peaks(ECG_window, length, peaks, &num_peaks);

    	//printf("\rN: %u\n", num_peaks);

    if (num_peaks < 2) {
        *HR = 0.0f; // invalid HR
        return HAL_ERROR;
    }

    // Calculate the average peak-to-peak interval
    float RR = 0.0f;
    for (uint8_t i = 1; i < num_peaks; i++) {
        RR += (float)(peaks[i] - peaks[i - 1]);
    }

    RR = RR / (num_peaks - 1) ; // Average interval
    RR = RR / samp_freq;

    *HR = round(60 / RR); // HR in bpm

    return HAL_OK;

}
