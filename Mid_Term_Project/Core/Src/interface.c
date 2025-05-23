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
#include "interface.h"
#include "stm32f4xx_hal.h"
#include <stdio.h>
#include <stdint.h>
#include <string.h>

// Function to clear the screen
void clear_screen() {
    // Clear the console screen using ANSI escape codes
    printf("\033[H\033[J");
}

// Function to initialize the interface with basic information
void init_interface() {
    clear_screen();  // Clear the screen at the start

    // Display basic information about the smartwatch and the monitoring system
    printf("        +---------------------------------------------+\r\n");
    printf("        |               SMARTWATCH                    |\r\n");
    printf("        |                 GROUP 1                     |\r\n");
    printf("        |     Carmine Vardaro, Alessia Parente,       |\r\n");
    printf("        |            Antonio D'Arienzo                |\r\n");
    printf("        +---------------------------------------------+\r\n");
    printf("        |          TEMPERATURE MONITORING             |\r\n");
    printf("        | Temp: 0.0 \xB0 C                               |\r\n");
    printf("        |                                             |\r\n");
    printf("        |                                             |\r\n");
    printf("        +---------------------------------------------+\r\n");
    printf("        |             HEART MONITORING                |\r\n");
    printf("        | Heart Rate: 0 bpm                           |\r\n");
    printf("        | HR Variability: 0.0 ms                      |\r\n");
    printf("        |                                             |\r\n");
    printf("        |                                             |\r\n");
    printf("        +---------------------------------------------+\r\n");
    printf("        |           MOVEMENT MONITORING               |\r\n");
    printf("        | Steps: 0                                    |\r\n");
    printf("        | Activity: Standing...                       |\r\n");
    printf("        | Red LED is on                               |\r\n");
    printf("        +---------------------------------------------+\r\n");
    printf("        |               INFORMATION                   |\r\n");
    printf("        | Press button 1 to measure body temperature  |\r\n");
    printf("        | Press button 2 to monitor heart             |\r\n");
    printf("        +---------------------------------------------+\r\n");
}

// Function to update the step count displayed on the interface
void update_steps(int steps) {
    // Move the cursor to the correct position to display the step count
    printf("\033[19;18H");

    // Clear the previous number (up to 5 characters)
    printf("     ");  // Fill with spaces to remove previous number

    // Return to the original position and print the new step count
    printf("\033[19;18H");

    // Print step count with formatting
    if (steps < 10) {
        printf("%d    ", steps); // Single digit with four spaces
    } else if (steps < 100) {
        printf("%d   ", steps);  // Two digits with three spaces
    } else if (steps < 1000) {
        printf("%d  ", steps);   // Three digits with two spaces
    } else if (steps < 10000) {
        printf("%d ", steps);    // Four digits with one space
    } else if (steps < 100000) {
        printf("%d", steps);     // Five digits without space
    } else {
        printf("#####");         // Handle large values
    }

    fflush(stdout); // Ensure the buffer is flushed immediately
}

// Function to update the activity displayed on the interface
void update_activity(const char* activity) {

    // Move the cursor to the correct position for activity
    printf("\033[20;21H");
    printf("           ");  // Fill with spaces to remove previous activity

    // Return to the original position and display the current activity
    printf("\033[20;21H");

    if (strcmp(activity, "RESTING") == 0) {
            printf("Standing...");
        } else if (strcmp(activity, "WALKING") == 0) {
            printf("Walking....");
        } else if (strcmp(activity, "RUNNING") == 0) {
            printf("Running....");
        } else {
            printf("Unknown....");
        }

    // Update the LED status based on the activity
    printf("\033[21;11H");
    printf("                ");
    printf("\033[21;11H");

    if (strcmp(activity, "RESTING") == 0) {
    	printf("Red LED is on   ");
    } else if (strcmp(activity, "WALKING") == 0) {
    	printf("Yellow LED is on");
    } else if (strcmp(activity, "RUNNING") == 0) {
    	printf("Green LED is on ");
    } else {
    	printf("Unknown....     ");
    }

    fflush(stdout);
}

// Function to update the activity message displayed on the interface
void update_activity_message(const char *message) {

    // Position the cursor on the correct line and column
    printf("\033[20;34H");

    // Clear the previous message
    printf("              ");

    // Return to the original position and print the new message
    printf("\033[20;34H");

    printf("%s", message);

    // Flush the buffer immediately
    fflush(stdout);
}

// Function to update the body temperature displayed on the interface
void update_temperature(float temp){

	// Move the cursor to the correct position for temperature display
	printf("\033[8;17H");

	// Clear previous temperature reading
	printf("        ");  // Fill with spaces to remove previous number

	// Return to the original position and print the new temperature
	printf("\033[8;17H");

	// Display the temperature with one decimal point
	printf("%.1f \xB0 C \n", temp );

	fflush(stdout); // Ensure the buffer is flushed immediately
}

// Function to update the progress bar for body temperature measurement
void update_temperature_progress(uint8_t progress){

	// Move the cursor to the correct position for progress display
	printf("\033[10;15H");

	// Clear the previous progress bar
	printf("                         ");  // Fill with spaces to remove previous progress

	// Return to the original position and print the new progress bar
	printf("\033[10;15H");

	// Display the progress bar (25 segments)
	 printf("  [");
	     for (int i = 0; i < 25; i++) {
	         printf(i < (progress / 4) ? "#" : "-");
	     }
	     printf("] %d%%", progress);

	fflush(stdout);
}

// Function to display a custom temperature message
void update_temperature_message(const char *message){

	// Move the cursor to the correct position for the temperature message
	printf("\033[9;10H");

	// Clear the previous message
	printf("                                             ");  // Fill with spaces to remove previous message

	// Return to the original position and print the new message
	printf("\033[9;10H");

	printf("%s", message);

	fflush(stdout); // Ensure the buffer is flushed immediately
}

// Function to clear the temperature interface elements
void clear_temp_interface(){

	// Clear the temperature message and progress bar
	printf("\033[9;10H");
	printf("                                             ");

	printf("\033[10;11H");
	printf("                                            ");

	fflush(stdout);
}

// Function to update the ECG data displayed on the interface
void update_ECG(uint8_t HR, float HRV){

	// Position the cursor and clear previous heart rate (HR) and heart rate variability (HRV)
	printf("\033[13;23H");
	printf("       ");  // Fill with spaces to remove previous HR value

	printf("\033[14;27H");
	printf("        ");  // Fill with spaces to remove previous HRV value

	// Display the updated heart rate (HR) and HR variability (HRV)
	printf("\033[13;23H");
    if (HR < 10) {
        printf("%d bpm  ", HR);
    } else if (HR < 100) {
        printf("%d bpm ", HR);
    } else if (HR < 1000) {
        printf("%d bpm", HR);
    } else {
        printf("#####  ");
    }

	printf("\033[14;27H");

	if (HRV < 10) {
		printf("%.1f ms  ", HRV);
	} else if (HRV < 100) {
		printf("%.1f ms ", HRV);
	} else if (HRV < 1000) {
		printf("%.1f ms", HRV);
	} else {
		printf("###### ");
	}

	fflush(stdout);
}

// Function to update the ECG progress bar
void update_ECG_progress(uint8_t progress){

	// Position the cursor for the ECG progress bar
	printf("\033[16;15H");

	// Clear the previous progress bar
	printf("                         ");  // Fill with spaces to remove previous progress

	// Return to the original position and display the new progress bar
	printf("\033[16;15H");

	printf("  [");
	for (int i = 0; i < 25; i++) {
		printf(i < (progress / 4) ? "#" : "-");
	}
	printf("] %d%%", progress);

	fflush(stdout);
}

// Function to display a custom ECG message
void update_ECG_message(const char *message){

	// Position the cursor and clear previous ECG message
	printf("\033[15;10H");
	printf("                                             ");  // Fill with spaces to remove previous message

	// Return to the original position and print the new ECG message
	printf("\033[15;10H");

	printf("%s", message);

	fflush(stdout);
}

// Function to clear the ECG interface elements
void clear_ECG_interface(){

	// Clear the ECG message and progress bar
	printf("\033[15;10H");
	printf("                                             ");

	printf("\033[16;11H");
	printf("                                            ");

	fflush(stdout);
}

void print_ECG_alarm(const char *message, uint8_t HR, float HRV) {
    // Split the message into three parts of 45 characters each
    char part1[46], part2[46], part3[46];
    strncpy(part1, message, 45);
    part1[45] = '\0';
    strncpy(part2, message + 45, 45);
    part2[45] = '\0';
    strncpy(part3, message + 90, 45);
    part3[45] = '\0';

    // Delay to allow for ECG reading before starting the alarm
    HAL_Delay(100);

    // Blink the alarm 10 times
    for (int blink = 0; blink < 10; blink++) {
        if (blink % 2 == 0) {
            // Show the alarm
            printf("\033[13;10H");
            printf("                  ALARM!!!!             ");

            printf("\033[14;10H");
            printf("%s", part1);

            printf("\033[15;10H");
            printf("%s", part2);

            printf("\033[16;10H");
            printf("%s", part3);
            fflush(stdout);
        } else {
            // Hide the alarm (overwrite with spaces)
            for (int j = 0; j < 4; j++) {
                printf("\033[%d;10H", 13 + j);
                printf("                                             ");
                fflush(stdout);
            }
        }

        // Delay for blinking effect
        if (blink % 2 == 0) {
            HAL_Delay(1500);  // Visible part
        } else {
            HAL_Delay(100);  // Invisible part
        }
    }

    // Display HR and HRV data after blinking
    printf("\033[13;10H");
    printf(" Heart Rate                              ");
    printf("\033[14;10H");
    printf(" HR Variability:                          ");
    update_ECG(HR, HRV);

    fflush(stdout);
}

void print_temp_alarm(const char *message, float temp) {
    // Split the message into two parts of 45 characters each
    char part1[46], part2[46];
    strncpy(part1, message, 45);
    part1[45] = '\0';
    strncpy(part2, message + 45, 45);
    part2[45] = '\0';

    // Delay to allow for temperature reading before starting the alarm
    HAL_Delay(100);

    // Blink the alarm for 10 times
    for (int blink = 0; blink < 10; blink++) {
        if (blink % 2 == 0) {
            // Show the alarm
            printf("\033[8;10H");
            printf("                  ALARM!!!!            ");

            printf("\033[9;10H");
            printf("%s", part1);

            printf("\033[10;10H");
            printf("%s", part2);

            fflush(stdout);
        } else {
            // Hide the alarm (overwrite with spaces)
            for (int j = 0; j < 3; j++) {
                printf("\033[%d;10H", 8 + j);
                printf("                                             ");
                fflush(stdout);
            }
        }

        // Delay for blinking effect
        if (blink % 2 == 0) {
            HAL_Delay(1000);  // Visible part
        } else {
            HAL_Delay(200);  // Invisible part
        }
    }

    // Display the temperature data after blinking
    printf("\033[8;10H");
    printf(" Temp:                                    ");
    update_temperature(temp);

    fflush(stdout);
}





