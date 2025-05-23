/*
 * Project: Wearable System for Health Monitoring
 * Authors: Carmine Vardaro, Alessia Parente, Antonio D'Arienzo
 *
 * Licensed under the GNU General Public License v3.0 (GPL v3).
 * For more details, see: https://www.gnu.org/licenses/gpl-3.0.html
 *
 * Description:
 * This file contains the declarations of functions and structures for reading and processing temperature data.
 * It includes functions for calculating the temperature from ADC readings, checking the temperature stability,
 * and maintaining the system state regarding temperature fluctuations. The code also provides utilities to reset
 * or initialize the temperature-related parameters.
 */

#ifndef INC_READ_TEMPERATURE_H_
#define INC_READ_TEMPERATURE_H_

#include "adc.h"
#include "activity_tracking.h"

// Definitions for temperature calculation parameters
#define RS 10000           // Series resistor value for thermistor (nominal value in ohms)
#define BETA 3950          // Beta value of the thermistor, used in the Steinhart-Hart formula
#define T0 298.15          // Nominal temperature of the thermistor (25°C in Kelvin)
#define R0 10000           // Nominal resistance of the thermistor at T0 (in ohms)

#define STABILITY_SAMPLES 20  // Number of samples to determine temperature stability
#define STABILITY_THRESHOLD 1.0  // Variation threshold to consider temperature stable/unstable

typedef struct {
    // Structure to hold temperature and stability-related data
    float currentTemperature;   // Current temperature reading
    float previousTemperature;  // Previously read temperature (used for comparison)
    float stableTemperature;    // Stable temperature when the system is in equilibrium
    uint8_t isStable;           // Flag indicating if the temperature is stable (1 if stable, 0 if unstable)
    uint8_t stabilityCounter;   // Counter tracking consecutive stable samples
    uint8_t variationDetected;  // Flag signaling if a variation in temperature has been detected
} TempParam;


// Function to initialize the parameters of the TempParam structure
void init_TemperatureParams(TempParam *tempParam);

// Function to calculate the temperature from the ADC reading (d_out)
double calculate_Temperature(uint16_t d_out);

// Main function to read the temperature and check its stability
void read_Temperature(TempParam *tempParam, uint16_t d_out, UserState state);

// Function to check if the temperature is stable (based on temperature variation)
void check_Stability(TempParam *tempParam, UserState state);

// Function to reset the parameters of the TempParam structure and restore it to the initial state
void reset_TemperatureParams(TempParam *tempParam);

#endif /* INC_READ_TEMPERATURE_H_ */
