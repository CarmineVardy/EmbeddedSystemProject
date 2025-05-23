/*
 * Project: Wearable System for Health Monitoring
 * Authors: Carmine Vardaro, Alessia Parente, Antonio D'Arienzo
 *
 * Licensed under the GNU General Public License v3.0 (GPL v3).
 * For more details, see: https://www.gnu.org/licenses/gpl-3.0.html
 *
 * This header file contains declarations related to the activity tracking system of the wearable health monitoring device.
 * The system tracks user activity (resting, walking, running) based on the data from force-sensitive resistors (FSR) placed in the shoes.
 * It manages the detection of steps, determines the user's state, and updates the corresponding LEDs for feedback.
 */

#ifndef INC_ACTIVITY_TRACKING_H_
#define INC_ACTIVITY_TRACKING_H_

#include "main.h"
#include <stdint.h>
#include "led.h"

// Threshold values and constants
#define STEP_THRESHOLD 100       // Threshold for detecting foot contact with the ground
#define RUNNING_THRESHOLD 500    // Threshold to determine the "RUNNING" state
#define MIN_WALKING_STEPS 2      // Minimum consecutive steps to confirm walking state
#define INACTIVITY_TIMEOUT 2000  // Timeout for inactivity (2 seconds) to confirm resting state

// Enum to define user states (resting, walking, running)
typedef enum {
    RESTING,
    WALKING,
    RUNNING
} UserState;

// Struct for storing force sensor data for each foot
typedef struct {
    float data;                // Sensor data value
    uint8_t foot_on_ground;    // Current state of the foot (1 = on the ground, 0 = lifted)
    uint8_t prev_foot_on_ground; // Previous state of the foot
} FSR_Sensor;

// Struct for tracking user activity, including step count and state
typedef struct {
    uint32_t steps;                 // Total step count
    uint32_t last_step_time;        // Time of the last detected step (in ms)
    uint32_t consecutive_steps;     // Consecutive steps since last resting state
    FSR_Sensor left_foot;           // Left foot sensor data
    FSR_Sensor right_foot;          // Right foot sensor data
    UserState state;                // Current state of the user (resting, walking, running)
} UserActivity;

// Struct for buffering step times and calculating step statistics
typedef struct {
    uint32_t* buffer;              // Buffer to store step times
    uint32_t total_step_time;      // Sum of step times
    uint16_t index;                // Current index in the buffer
    uint16_t length;               // Length of the buffer
    uint16_t filled;               // Number of filled entries in the buffer
} StepBuffer;

// Function prototypes for initializing sensors, updating activity states, and processing steps
void init_fsr_sensor(FSR_Sensor *sensor); // Initialize the FSR sensor with default values
void UserActivity_init(UserActivity *user_activity, LedArray *ledArray); // Initialize user activity and sensors
void StepBuffer_init(StepBuffer *buffer, uint32_t* steps, uint16_t length); // Initialize step buffer with specified length

void read_forceSensor(uint16_t *d_out, FSR_Sensor *sensor); // Read data from the FSR sensor and update its parameters
void update_step_count(UserActivity *user_activity, uint32_t current_time, StepBuffer *step_buffer, LedArray *ledArray); // Update the step count and user activity state
void determine_activity(UserActivity *user_activity, uint32_t current_time, StepBuffer *step_buffer, LedArray *ledArray); // Determine the user's activity state based on step data

void add_step_time(StepBuffer *buffer, uint32_t step_time); // Add step time to the step buffer
void reset_step_buffer(StepBuffer *buffer); // Reset the step buffer, clearing total step time and indices

const char* userState_to_string(UserState state); // Convert the user's state to a readable string

void set_walking_state(UserActivity *user_activity, LedArray *ledArray); // Set user state to "walking" and activate the yellow LED
void set_running_state(UserActivity *user_activity, LedArray *ledArray); // Set user state to "running" and activate the green LED
void set_resting_state(UserActivity *user_activity, LedArray *ledArray); // Set user state to "resting" and activate the red LED

#endif /* INC_ACTIVITY_TRACKING_H_ */
