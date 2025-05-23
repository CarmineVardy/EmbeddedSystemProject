/*
 * Project: Wearable System for Health Monitoring
 * Authors: Carmine Vardaro, Alessia Parente, Antonio D'Arienzo
 *
 * Licensed under the GNU General Public License v3.0 (GPL v3).
 * For more details, see: https://www.gnu.org/licenses/gpl-3.0.html
 *
 * Description:
 * This file contains the implementation of functions for processing ECG signals,
 * including signal filtering, peak detection, and heart rate (HR) and heart rate variability (HRV)
 * calculation. It also includes utilities to manage ECG data buffers, detect R-wave peaks,
 * and reset the ECG processing system for fresh measurements.
 */

#include "ECG_module.h"
#include <math.h>

/*
 * Initializes the ECG filter with the given buffer and length.
 */
void ECG_init_Filter(FilterECGParam* FilterECGparam, uint16_t* buffer, uint16_t length) {
    // Set the input buffer in the FilterECGParam structure
    FilterECGparam->buffer = buffer;

    // Initialize the filter parameters
    FilterECGparam->sum = 0.0;    // Sum of the values in the filter (used for moving average calculation)
    FilterECGparam->index = 0;    // Current index in the buffer (used for circular buffer)
    FilterECGparam->length = length;    // Length of the buffer
    FilterECGparam->filled = 0;   // Number of valid elements in the buffer

    // Initialize the buffer by setting all values to 0
    for (uint16_t i = 0; i < length; i++) {
        buffer[i] = 0;
    }
}

/*
 * Initializes the ECG parameters with the given buffer and length.
 */
void ECG_init_ECG(ECGParam* ECGparam, uint16_t* buffer, uint16_t length) {
    // Set the input buffer in the ECGParam structure
    ECGparam->buffer = buffer;

    // Initialize ECG parameters
    ECGparam->count = 0;         // Number of processed samples
    ECGparam->length = length;   // Length of the buffer
    ECGparam->HR = 0;            // Heart rate (in beats per minute, bpm)
    ECGparam->HRV = 0.0;         // Heart rate variability (HRV)
}

/*
 * Filters the ECG signal and stores the filtered value in the ECG buffer.
 */
void filter_signal(FilterECGParam* FilterECGparam, ECGParam* ECGparam, uint16_t d_out) {
    // Calculate the input voltage based on the output data
    float voltage = d_out * VREF / LEVELS;

    // Update the sum for moving average
    FilterECGparam->sum = FilterECGparam->sum - FilterECGparam->buffer[FilterECGparam->index] + voltage;
    FilterECGparam->buffer[FilterECGparam->index] = voltage;

    // Update the buffer index in a circular manner
    FilterECGparam->index = (FilterECGparam->index + 1) % FilterECGparam->length;

    // Increase the valid element counter in the filter
    if (FilterECGparam->filled < FilterECGparam->length) {
        FilterECGparam->filled++;
    }

    // Calculate the average value and store it in the ECG buffer
    ECGparam->buffer[ECGparam->count] = (uint16_t)(FilterECGparam->sum / FilterECGparam->filled);

    // Increment the processed sample counter
    ECGparam->count++;
}

/*
 * Processes the ECG data, calculates the heart rate (HR) and heart rate variability (HRV).
 */
HAL_StatusTypeDef elab_ECG(ECGParam* ECGparam, uint16_t samp_freq) {
    uint16_t peaks[MAX_ARRAY_DIM];  // Array to store detected peaks
    uint8_t num_peaks = 0;          // Number of detected peaks

    // Find the peaks in the ECG signal
    if (find_peaks(ECGparam, peaks, &num_peaks) != HAL_OK || num_peaks < 2) {
        return HAL_ERROR; // No valid peaks detected
    }

    float samplePeriod = 1000.0 / samp_freq; // Duration of one sample in milliseconds
    float intervals[MAX_ARRAY_DIM - 1];      // Array to store RR intervals
    uint8_t validIntervals = 0;              // Number of valid intervals

    // Calculate RR intervals in milliseconds
    for (int i = 1; i < num_peaks; i++) {
        float interval = (float)(peaks[i] - peaks[i - 1]) * samplePeriod;
        if (interval > 300.0 && interval < 2000.0) { // Filter physiological intervals (300-2000 ms)
            intervals[validIntervals++] = interval;
        }
    }

    // If no valid intervals, return error
    if (validIntervals < 2) {
        return HAL_ERROR;
    }

    // Calculate the mean of the intervals
    float meanInterval = 0.0;
    for (int i = 0; i < validIntervals; i++) {
        meanInterval += intervals[i];
    }
    meanInterval /= validIntervals;

    // Calculate the standard deviation of the intervals
    float sumSquareDiff = 0.0;
    for (int i = 0; i < validIntervals; i++) {
        sumSquareDiff += (intervals[i] - meanInterval) * (intervals[i] - meanInterval);
    }
    float stdDeviation = sqrt(sumSquareDiff / (validIntervals - 1));

    // Calculate the heart rate (HR) based on the average interval
    uint8_t HR = round(60000.0 / meanInterval); // HR = 60 / (avgInterval / 1000)

    // Calculate HRV as the standard deviation
    float HRV = stdDeviation;

    // Return the calculated HR and HRV
    ECGparam->HR = HR;
    ECGparam->HRV = HRV;

    return HAL_OK;
}

/*
 * Finds the peaks in the ECG signal.
 */
HAL_StatusTypeDef find_peaks(ECGParam* ECGparam, uint16_t *peaks, uint8_t *num_peaks) {
    // If there are less than 3 samples, no peaks can be detected
    if (ECGparam->count < 3) {
        *num_peaks = 0;
        return HAL_ERROR;
    }

    uint16_t max_value = find_max(ECGparam->buffer, ECGparam->count); // Maximum value in the ECG buffer
    uint16_t min_value = find_min(ECGparam->buffer, ECGparam->count); // Minimum value in the ECG buffer
    uint16_t threshold = (uint16_t)(TRESHOLDPEAKS * (max_value - min_value)) + min_value; // Peak detection threshold

    uint8_t count = 0;                  // Peak count
    uint8_t aboveThresholdState = 0;    // Flag to track threshold crossing

    // Scan the array from 1 to count-1 to avoid out-of-bounds access
    for (uint16_t i = 1; i < ECGparam->count - 1; i++) {
        int16_t derivative = (int16_t)(ECGparam->buffer[i] - ECGparam->buffer[i - 1]);

        // Check for crossing the threshold (rising and falling edges)
        if (aboveThresholdState == 0 && derivative > 0 && ECGparam->buffer[i] >= threshold && ECGparam->buffer[i - 1] < threshold) {
            aboveThresholdState = 1; // Entered above threshold
        } else if (aboveThresholdState == 1 && ECGparam->buffer[i] < threshold) {
            aboveThresholdState = 0; // Exited below threshold
        }

        // Detect local peak: point i must be greater than previous and next points
        if (ECGparam->buffer[i] > threshold &&
            ECGparam->buffer[i] > ECGparam->buffer[i - 1] &&
            ECGparam->buffer[i] > ECGparam->buffer[i + 1] &&
            aboveThresholdState == 1) {
            peaks[count] = i; // Add the peak to the array
            count++;

            if (count >= MAX_ARRAY_DIM) {
                break; // Prevent overflow of the peaks array
            }

            // Prevent duplicate peaks in the same area
            aboveThresholdState = 0;
        }
    }

    *num_peaks = count; // Set the number of detected peaks
    return HAL_OK;
}

/*
 * Finds the maximum value in the ECG buffer.
 */
uint16_t find_max(uint16_t *buffer, uint16_t length) {
    if (length == 0) {
        return 0; // Return 0 if the buffer is empty
    }

    uint16_t max = buffer[0]; // Initialize the max with the first value

    // Iterate through the buffer to find the maximum value
    for (uint16_t i = 1; i < length; i++) {
        if (buffer[i] > max) {
            max = buffer[i];
        }
    }

    return max;  // Return the maximum value found
}

/*
 * Finds the minimum value in the ECG buffer.
 */
uint16_t find_min(uint16_t *buffer, uint16_t length) {
    if (length == 0) {
        return 0; // Return 0 if the buffer is empty
    }

    uint16_t min = buffer[0]; // Initialize the min with the first value

    // Iterate through the buffer to find the minimum value
    for (uint16_t i = 1; i < length; i++) {
        if (buffer[i] < min) {
            min = buffer[i];
        }
    }

    return min;  // Return the minimum value found
}

/*
 * Resets the ECG filter parameters and ECG data.
 */
void reset_ECG(FilterECGParam* FilterECGparam, ECGParam* ECGparam) {
    // Reset the ECG filter parameters
    FilterECGparam->sum = 0.0;           // Reset the sum of filtered values
    FilterECGparam->index = 0;           // Reset the buffer index
    FilterECGparam->filled = 0;          // Reset the filled count

    // Reset ECG parameters
    ECGparam->count = 0;                 // Reset the ECG count
}
