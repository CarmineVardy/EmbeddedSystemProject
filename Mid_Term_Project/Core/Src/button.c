/*
 * Project: Wearable System for Health Monitoring
 * Authors: Carmine Vardaro, Alessia Parente, Antonio D'Arienzo
 *
 * Licensed under the GNU General Public License v3.0 (GPL v3).
 * For more details, see: https://www.gnu.org/licenses/gpl-3.0.html
 *
 * Description:
 * This file defines the functions for handling button input in the wearable health monitoring system.
 * The debounce logic ensures that the button input is stable and not affected by mechanical bounce.
 */

#include "button.h"

/*
 * Initializes the button configuration with the provided parameters.
 * It assigns the GPIO pin label, port, and initial states to the `buttonConfig` structure.
 * Also initializes the debounce counter and states.
 */
void button_init(buttonConfig* button, uint16_t user_label, GPIO_TypeDef* port, GPIO_PinState init_state) {
    button->GPIO_user_label = user_label;    // Set the button's GPIO pin label
    button->GPIO_Port = port;                // Set the GPIO port the button is connected to
    button->temp_state = init_state;         // Set the initial temporary state
    button->stable_state = init_state;       // Set the initial stable state
    button->previous_state = init_state;     // Set the initial previous state
    button->count = 0;                       // Initialize debounce counter to 0
}

/*
 * Reads the current state of the button and applies debounce logic.
 * If the state is stable (no bounce detected), it updates the stable state and returns it.
 * Returns the debounced button state (either pressed or released).
 */
GPIO_PinState read_button(buttonConfig* button) {
    // Read the current state of the button
    GPIO_PinState current_state = HAL_GPIO_ReadPin(button->GPIO_Port, button->GPIO_user_label);

    // If the current state matches the temporary state, increment the debounce counter
    if (current_state == button->temp_state) {
        button->count++;  // Increment debounce counter
        // If the debounce counter exceeds the threshold, update the stable state
        if (button->count >= COUNT_LIMIT) {
            button->count = 0;  // Reset the counter
            button->previous_state = button->stable_state;  // Store the previous stable state
            button->stable_state = current_state;  // Update the stable state
        }
    } else {
        button->count = 0;  // Reset the counter if the state changes
        button->temp_state = current_state;  // Update the temporary state
    }

    // Return the debounced stable state of the button
    return button->stable_state;
}
