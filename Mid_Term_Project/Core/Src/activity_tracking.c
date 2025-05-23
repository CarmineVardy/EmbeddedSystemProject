/*
 * Project: Wearable System for Health Monitoring
 * Authors: Carmine Vardaro, Alessia Parente, Antonio D'Arienzo
 *
 * Licensed under the GNU General Public License v3.0 (GPL v3).
 * For more details, see: https://www.gnu.org/licenses/gpl-3.0.html
 *
 * Description:
 * This file contains the code for activity tracking in a wearable health monitoring system.
 * It manages the step counting, user activity state (resting, walking, running),
 * and the interaction with the FSR sensors for detecting foot pressure and determining steps.
 * The system updates the user's activity state based on the number of consecutive steps
 * and provides visual feedback using LEDs.
 */

#include "activity_tracking.h"
#include "led.h"
#include "interface.h"
#include <stdio.h>

/*
 * Initializes the FSR (Force Sensitive Resistor) sensor
 * It sets the initial foot-on-ground state and resets the sensor data.
 */
void init_fsr_sensor(FSR_Sensor *sensor) {
    sensor->data = 0.0f;
    sensor->foot_on_ground = 0;  // Foot is initially up
    sensor->prev_foot_on_ground = 0; // Previous state is also up
}

/*
 * Initializes the user's activity tracking, setting initial values for step count,
 * the last step time, consecutive steps, and the user’s activity state.
 */
void UserActivity_init(UserActivity *user_activity, LedArray *ledArray) {
    user_activity->steps = 0;
    user_activity->last_step_time = 0;
    user_activity->consecutive_steps = 0;
    init_fsr_sensor(&user_activity->left_foot);
    init_fsr_sensor(&user_activity->right_foot);
    user_activity->state = RESTING;  // Set initial state to RESTING
    control_led(ledArray, &ledArray->red_Led);  // Turn on the red LED to indicate resting state (led.c)
}

/*
 * Initializes the step buffer with the specified length and resets all step times.
 */
void StepBuffer_init(StepBuffer *buffer, uint32_t* steps, uint16_t length) {
    buffer->buffer = steps;
    buffer->total_step_time = 0.0;
    buffer->index = 0;
    buffer->length = length;
    buffer->filled = 0;

    // Initialize the buffer with zero step times
    for (uint16_t i = 0; i < length; i++) {
        steps[i] = 0;
    }
}

/*
 * Reads the data from the FSR sensor, calculating the voltage and determining
 * if the foot is on the ground based on a threshold.
 */
void read_forceSensor(uint16_t *d_out, FSR_Sensor *sensor) {
    float voltage = ((float)(*d_out)) * VREF / LEVELS;
    sensor->data = voltage;
    sensor->prev_foot_on_ground = sensor->foot_on_ground;
    sensor->foot_on_ground = (voltage > STEP_THRESHOLD) ? 1 : 0; // Foot is on the ground if voltage exceeds threshold
}

/*
 * Updates the step count and determines the current activity state (resting, walking, or running)
 * based on the pressure sensor data and time.
 */
void update_step_count(UserActivity *user_activity, uint32_t current_time, StepBuffer *step_buffer, LedArray *ledArray) {
    uint32_t step_time; // Variable to calculate the time between two consecutive steps

    // Check left foot
    if (user_activity->left_foot.foot_on_ground && !user_activity->left_foot.prev_foot_on_ground) {
        // If left foot touches the ground and the right foot is not on the ground
        if (user_activity->right_foot.foot_on_ground == 0) {
            // Calculate step time
            step_time = current_time - user_activity->last_step_time;
            user_activity->steps++; // Increment step count
            update_steps(user_activity->steps); // Update the interface with the current step count (interface.c)
            add_step_time(step_buffer, step_time); // Add step time to the buffer
            user_activity->last_step_time = current_time; // Update last step time

            user_activity->consecutive_steps++; // Increment consecutive steps

            // If user was resting and now walks enough consecutive steps, switch to walking state
            if (user_activity->state == RESTING && user_activity->consecutive_steps >= MIN_WALKING_STEPS) {
                set_walking_state(user_activity, ledArray); // Switch to walking state and turn on yellow LED
            }
        }
    }

    // Check right foot
    if (user_activity->right_foot.foot_on_ground && !user_activity->right_foot.prev_foot_on_ground) {
        // If right foot touches the ground and the left foot is not on the ground
        if (user_activity->left_foot.foot_on_ground == 0) {
            // Calculate step time
            step_time = current_time - user_activity->last_step_time;
            user_activity->steps++; // Increment step count
            update_steps(user_activity->steps); // Update the interface with the current step count (inteface.c)
            add_step_time(step_buffer, step_time); // Add step time to the buffer
            user_activity->last_step_time = current_time; // Update last step time

            user_activity->consecutive_steps++; // Increment consecutive steps

            // If user was resting and now walks enough consecutive steps, switch to walking state
            if (user_activity->state == RESTING && user_activity->consecutive_steps >= MIN_WALKING_STEPS) {
                set_walking_state(user_activity, ledArray); // Switch to walking state and turn on yellow LED
            }
        }
    }

    // Determine the current activity based on inactivity or number of steps
    determine_activity(user_activity, current_time, step_buffer, ledArray);
}

/*
 * Determines the activity state (resting, walking, or running) based on time and step buffer.
 */
void determine_activity(UserActivity *user_activity, uint32_t current_time, StepBuffer *step_buffer, LedArray *ledArray) {
    // If user has been inactive for a while, set to resting state
    if (current_time - user_activity->last_step_time >= INACTIVITY_TIMEOUT) {
        set_resting_state(user_activity, ledArray); // Set to resting state and turn on red LED
        user_activity->consecutive_steps = 0;  // Reset consecutive steps
        reset_step_buffer(step_buffer); // Reset step buffer if user stops
    } else if (user_activity->state != RESTING) { // Ensure user is not already resting
        uint32_t average_time = 0;

        if (step_buffer->filled > 0) {
            average_time = step_buffer->total_step_time / step_buffer->filled; // Calculate average step time
        }

        // If step time is below the running threshold, set state to running; otherwise, walking
        if (average_time < RUNNING_THRESHOLD) {
            set_running_state(user_activity, ledArray); // Set to running state and turn on green LED
        } else {
            set_walking_state(user_activity, ledArray); // Set to walking state and turn on yellow LED
        }
    }
}

/*
 * Adds a step time to the buffer and updates the total step time.
 */
void add_step_time(StepBuffer *buffer, uint32_t step_time) {
    buffer->total_step_time += step_time - buffer->buffer[buffer->index];
    buffer->buffer[buffer->index] = step_time;
    buffer->index = (buffer->index + 1) % buffer->length; // Circular buffer
    if (buffer->filled < buffer->length) {
        buffer->filled++; // Fill buffer until it reaches the defined length
    }
}

/*
 * Resets the step buffer, clearing all previous step times and totals.
 */
void reset_step_buffer(StepBuffer *buffer) {
    buffer->total_step_time = 0;       // Reset total step time
    buffer->index = 0;                // Reset buffer index
    buffer->filled = 0;               // Reset filled count
    for (uint16_t i = 0; i < buffer->length; i++) {
        buffer->buffer[i] = 0;        // Optionally, reset all values in the buffer
    }
}

/*
 * Converts user activity state to a string representation for display
 */
const char* userState_to_string(UserState state) {
    switch (state) {
        case RESTING: return "RESTING";
        case WALKING: return "WALKING";
        case RUNNING: return "RUNNING";
        default: return "UNKNOWN"; // Return UNKNOWN if state is not recognized
    }
}

/*
 * Sets the user's state to walking and updates the corresponding LED and activity interface
 */
void set_walking_state(UserActivity *user_activity, LedArray *ledArray) {
    // Change state only if user is not already walking
    if (user_activity->state != WALKING) {
        user_activity->state = WALKING;  // Set state to WALKING
        control_led(ledArray, &ledArray->yellow_Led);  // Turn on yellow LED to indicate walking (led.c)
        update_activity(userState_to_string(WALKING)); // Update interface with walking activity (interface.c)
    }
}

/*
 * Sets the user's state to running and updates the corresponding LED and activity interface
 */
void set_running_state(UserActivity *user_activity, LedArray *ledArray) {
    // Change state only if user is not already running
    if (user_activity->state != RUNNING) {
        user_activity->state = RUNNING;  // Set state to RUNNING
        control_led(ledArray, &ledArray->green_Led);  // Turn on green LED to indicate running (lec.c)
        update_activity(userState_to_string(RUNNING)); // Update interface with running activity (interface.c)
    }
}

/*
 * Sets the user's state to resting and updates the corresponding LED and activity interface
 */
void set_resting_state(UserActivity *user_activity, LedArray *ledArray) {
    // Change state only if user is not already resting
    if (user_activity->state != RESTING) {
        user_activity->state = RESTING;  // Set state to RESTING
        control_led(ledArray, &ledArray->red_Led);  // Turn on red LED to indicate resting (led.c)
        update_activity(userState_to_string(RESTING)); // Update interface with resting activity (interface.c)
    }
}
