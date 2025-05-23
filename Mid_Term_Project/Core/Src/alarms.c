/*
 * Project: Wearable System for Health Monitoring
 * Authors: Carmine Vardaro, Alessia Parente, Antonio D'Arienzo
 *
 * Licensed under the GNU General Public License v3.0 (GPL v3).
 * For more details, see: https://www.gnu.org/licenses/gpl-3.0.html
 *
 * Description:
 * This file contains the implementation of alarm checks for temperature and ECG measurements.
 * Alarms are triggered based on the user's activity state and health metrics.
 */

#include "alarms.h"  // Include the alarms header file for function declarations
#include "interface.h"  // Include the interface for printing alarm messages

/*
 * Checks the temperature and triggers an alarm based on the user's activity and temperature value.
 * It checks if the temperature is high or low, and issues different messages based on the activity state.
 * print_temp_alarm is implemented in interface.c
 */
void checkTemperature_Alarm(float temp, UserState state) {
    // High temperature condition
    if (temp > 27.0) {
        if (state == RESTING || state == WALKING) {
            // Low activity: may indicate fever
            print_temp_alarm("High temperature detected... Symptoms: headache, cold, sore throat.. Consult a doctor.....", temp);
        } else {
            // High activity: temperature may be due to physical effort
            print_temp_alarm("High temperature detected, possibly from running. Stop, cool down, retake in a few minutes", temp);
        }
    }
    // Very low temperature condition
    else if (temp < 21.0) {
        print_temp_alarm("Very low body temperature detected. Warm up and retake the measurement after a few minutes....", temp);
    }
}

/*
 * Checks the ECG data (Heart Rate and HRV) and triggers alarms based on the user's activity state.
 * For each state (resting, walking, running), it checks if the heart rate and HRV values are abnormal and triggers appropriate messages.
 * print_ECG_alarm is implemented in interface.c
 */
void checkECG_Alarm(uint8_t HR, float HRV, UserState state) {
    // HRV is now measured in milliseconds, so thresholds are adjusted accordingly
    // Example: HRV below 50 milliseconds could indicate an abnormal condition

    // Resting state checks
    if (state == RESTING) {
        if (HR > 100 && HRV < 50.0) {  // HRV in milliseconds
            print_ECG_alarm("Abnormal heart activity at rest. It could indicate stress or tachycardia. Consult your doctor immediately for a proper evaluation......", HR, HRV);
        } else if (HR > 100 && HRV > 150.0) {  // HRV in milliseconds
            print_ECG_alarm("Unusual heart behavior at rest. It could indicate vagal excitation or measurement errors. Verify your data and consult a doctor........", HR, HRV);
        } else if (HR < 50 && HRV < 50.0) {  // HRV in milliseconds
            print_ECG_alarm("Abnormal heart readings at rest. It could indicate bradycardia or fatigue. Consult a doctor for a proper evaluation and guidance.......", HR, HRV);
        } else if (HR < 50 && HRV > 150.0) {  // HRV in milliseconds
            print_ECG_alarm("Irregular heart patterns detected at rest. Check the setup or health conditions if the patterns seem unusual. Seek medical advice......", HR, HRV);
        }
    }
    // Walking state checks
    else if (state == WALKING) {
        if (HR > 140 && HRV < 30.0) {  // HRV in milliseconds
            print_ECG_alarm("Elevated heart strain while walking. It could indicate overload or stress. Check your health status and rest if you feel unwell....... ", HR, HRV);
        } else if (HR > 140 && HRV > 100.0) {  // HRV in milliseconds
            print_ECG_alarm("Irregular heart behavior while walking. Ensure values are correct or rest if you feel unwell. Consult a doctor if symptoms persist.....", HR, HRV);
        } else if (HR < 50 && HRV < 30.0) {  // HRV in milliseconds
            print_ECG_alarm("Abnormal heart activity while walking. It could indicate imbalance or stress. Seek medical advice for proper evaluation and guidance...", HR, HRV);
        } else if (HR < 50 && HRV > 100.0) {  // HRV in milliseconds
            print_ECG_alarm("Unusual heart pattern while walking. Check for errors or consult a doctor if the issue persists or if you feel unwell..................", HR, HRV);
        }
    }
    // Running state checks
    else if (state == RUNNING) {
        if (HR > 190 && HRV < 20.0) {  // HRV in milliseconds
            print_ECG_alarm("Critical heart overload while running. Stop and rest immediately to ensure safety. Seek medical attention if symptoms continue.........", HR, HRV);
        } else if (HR > 190 && HRV > 70.0) {  // HRV in milliseconds
            print_ECG_alarm("Unusual heart response while running. Check values for accuracy or stop immediately if experiencing symptoms like dizziness............", HR, HRV);
        } else if (HR < 60 && HRV < 20.0) {  // HRV in milliseconds
            print_ECG_alarm("Severe imbalance while running. Stop immediately and seek help if symptoms persist or if you feel unwell to ensure your safety.........", HR, HRV);
        } else if (HR < 60 && HRV > 70.0) {  // HRV in milliseconds
            print_ECG_alarm("Abnormal recovery while running. Verify the conditions or check the measurement setup for accuracy and consult a doctor if persist.....", HR, HRV);
        }
    }
}
