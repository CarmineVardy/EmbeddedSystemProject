/*
 * Project: Wearable System for Health Monitoring
 * Authors: Carmine Vardaro, Alessia Parente, Antonio D'Arienzo
 *
 * Licensed under the GNU General Public License v3.0 (GPL v3).
 * For more details, see: https://www.gnu.org/licenses/gpl-3.0.html
 *
 * Description:
 * This file contains the implementation of functions for reading and processing temperature data.
 * It includes functions to initialize temperature parameters, calculate temperature from ADC readings,
 * check temperature stability, and reset temperature parameters. The code also interacts with the user interface
 * to display temperature status and progress.
 */

#include "read_temperature.h"
#include "interface.h"
#include "math.h"
#include "alarms.h"

// Function to initialize the temperature parameters structure
void init_TemperatureParams(TempParam *tempParam) {

    // Initialize current temperature to 0.0 (assuming no reading has been made yet)
    tempParam->currentTemperature = 0.0;

    // Previous temperature is also initially set to 0.0
    tempParam->previousTemperature = 0.0;

    // Stable temperature is initially set to 0.0
    tempParam->stableTemperature = 0.0;

    // Set the stability flag to 0 (unstable by default)
    tempParam->isStable = 0;

    // Initialize stability counter to track consecutive stable samples
    tempParam->stabilityCounter = 0;

    // Initialize variation detected flag to 0 (no variation detected)
    tempParam->variationDetected = 0;
}


// Function to calculate temperature based on ADC output value
double calculate_Temperature(uint16_t d_out) {

    // Calculate input voltage from ADC value (d_out)
    // VREF is the reference voltage, LEVELS is the number of ADC levels
    double voltage = ((double)d_out) * VREF / LEVELS;

    // Calculate resistance using thermistor formula (based on voltage)
    // RS is the known resistance value, VREF is the reference voltage
    double resistance = (-RS * (voltage)) / (voltage - VREF);

    // Calculate temperature using the Steinhart-Hart equation
    // BETA, R0, and T0 are thermistor-specific parameters
    // The formula returns temperature in Celsius, so subtract 273.15 to get the result in °C
    return (BETA) / (log(resistance / R0) + BETA / T0) - 273.15;
}


// Main function to read temperature and check its stability
void read_Temperature(TempParam *tempParam, uint16_t d_out, UserState state) {

    // Calculate the current temperature using the ADC value (d_out)
    tempParam->currentTemperature = calculate_Temperature(d_out);

    // Check the stability of the current temperature compared to the previous one
    // The 'check_Stability' function handles detecting temperature variations
    check_Stability(tempParam, state);

    // Store the current temperature as the previous temperature for the next read cycle
    // This is necessary for comparison in future readings and stability management
    tempParam->previousTemperature = tempParam->currentTemperature;
}


// Function to check if the temperature is stable (based on temperature variation)
void check_Stability(TempParam *tempParam, UserState state) {
    // Calculate the difference between current and previous temperature
    double tempDifference = fabs(tempParam->currentTemperature - tempParam->previousTemperature);

    // If the difference exceeds the stability threshold
    if (tempDifference > STABILITY_THRESHOLD) {
        // A significant variation has been detected
        tempParam->variationDetected = 1;
        tempParam->isStable = 0;  // Mark temperature as unstable

        // Reset the stability counter because the temperature has changed
        tempParam->stabilityCounter = 0;

        // Update the interface to show that temperature is unstable
        update_temperature_message("calculating temperature..possible variation..");
    } else {
        if (tempParam->variationDetected) {
            // Increment the stability counter
            tempParam->stabilityCounter++;

            uint8_t progress = (tempParam->stabilityCounter * 100) / STABILITY_SAMPLES;
            update_temperature_progress(progress);

            // Check if the number of stable samples has reached the threshold
            if (tempParam->stabilityCounter >= STABILITY_SAMPLES) {
                // Temperature is considered stable
                tempParam->isStable = 1;
                tempParam->variationDetected = 0;

                // Update the stable value only if it has changed significantly
                if (fabs(tempParam->currentTemperature - tempParam->stableTemperature) > STABILITY_THRESHOLD) {
                    tempParam->stableTemperature = tempParam->currentTemperature;

                    // Update and display the stable value
                    update_temperature(tempParam->stableTemperature);
                    checkTemperature_Alarm(tempParam->stableTemperature, state);
                }
                // Clear the interface to indicate the value is stable
                clear_temp_interface();
            }
        }
    }
}


// Function to reset the temperature parameters structure to its initial state
void reset_TemperatureParams(TempParam *tempParam) {

    // Reset the current temperature
    tempParam->currentTemperature = 0.0;

    // Reset the previous temperature
    tempParam->previousTemperature = 0.0;

    // Reset the stable temperature
    tempParam->stableTemperature = 0.0;

    // Mark the temperature as unstable (initial state)
    tempParam->isStable = 0;

    // Reset the stability counter
    tempParam->stabilityCounter = 0;

    // Reset the variation detected flag
    tempParam->variationDetected = 0;
}
