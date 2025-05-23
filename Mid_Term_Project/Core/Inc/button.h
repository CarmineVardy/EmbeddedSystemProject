/*
 * Project: Wearable System for Health Monitoring
 * Authors: Carmine Vardaro, Alessia Parente, Antonio D'Arienzo
 *
 * Licensed under the GNU General Public License v3.0 (GPL v3).
 * For more details, see: https://www.gnu.org/licenses/gpl-3.0.html
 *
 * Description:
 * This file defines the structure and functions for handling button input in the wearable health monitoring system.
 * The button input is debounced to prevent multiple false triggers from mechanical bounce.
 */

#ifndef INC_BUTTON_H_
#define INC_BUTTON_H_

#include "gpio.h"

#define COUNT_LIMIT 2  // Threshold value for debounce

/*
 * Structure to configure the button.
 * It holds information about the button's GPIO pin, state, and debounce handling.
 */
typedef struct {
    uint16_t GPIO_user_label;    // GPIO pin label associated with the button
    GPIO_TypeDef* GPIO_Port;     // GPIO port the button is connected to
    GPIO_PinState temp_state;    // Temporary button state for debounce checking
    GPIO_PinState stable_state;  // Stable (debounced) state of the button
    GPIO_PinState previous_state;// Previous button state to compare for changes
    uint8_t count;               // Counter for debounce logic
} buttonConfig;

/*
 * Initializes the button configuration, including the GPIO pin, port, and initial state.
 */
void button_init(buttonConfig* button, uint16_t user_label, GPIO_TypeDef* port, GPIO_PinState init_state);

/*
 * Reads the stable state of the button, applying debounce logic to filter out noise.
 * Returns the debounced button state (pressed/released).
 */
GPIO_PinState read_button(buttonConfig* button);

#endif /* INC_BUTTON_H_ */
