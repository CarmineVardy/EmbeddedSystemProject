/*
 * Project: Wearable System for Health Monitoring
 * Authors: Carmine Vardaro, Alessia Parente, Antonio D'Arienzo
 *
 * Licensed under the GNU General Public License v3.0 (GPL v3).
 * For more details, see: https://www.gnu.org/licenses/gpl-3.0.html
 *
 * Description:
 * This file contains the declarations of functions used for managing the user interface.
 * The interface updates various health monitoring parameters, such as steps, activity status,
 * temperature, and ECG readings. It also handles the display of related messages and alarms.
 */

#ifndef INC_INTERFACE_H_
#define INC_INTERFACE_H_

#include <stdint.h>

void init_interface(); // Function to initialize the interface
void clear_screen(); // Function to clear the interface screen
void update_steps(int steps); // Function to update the steps on the display
void update_activity(const char* activity); // Function to update the activity status
void update_activity_message(const char *message); // Function to update the activity message
void update_temperature(float temp); // Function to update the temperature display
void update_temperature_progress(uint8_t progress); // Function to update the temperature progress
void clear_temp_interface(); // Function to clear the temperature interface
void update_temperature_message(const char *message); // Function to update the temperature message
void update_ECG(uint8_t HR, float HRV); // Function to update the ECG readings on the display
void update_ECG_progress(uint8_t progress); // Function to update the ECG progress
void update_ECG_message(const char *message); // Function to update the ECG message
void clear_ECG_interface(); // Function to clear the ECG interface
void print_ECG_alarm(const char *message, uint8_t HR, float HRV); // Function to display an ECG alarm message
void print_temp_alarm(const char *message, float temp); // Function to display a temperature alarm message

#endif /* INC_INTERFACE_H_ */
