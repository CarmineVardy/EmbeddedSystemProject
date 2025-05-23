/*
 * Project: Wearable System for Health Monitoring
 * Authors: Carmine Vardaro, Alessia Parente, Antonio D'Arienzo
 *
 * Licensed under the GNU General Public License v3.0 (GPL v3).
 * For more details, see: https://www.gnu.org/licenses/gpl-3.0.html
 *
 * Description:
 * This file contains the definitions and functions for managing LEDs in the wearable health monitoring system.
 * The LEDs are used to provide visual feedback to the user regarding their activity state (resting, walking, running).
 * The code allows for initialization, turning on and off, and controlling multiple LEDs within an array.
 */

#ifndef INC_LED_H_
#define INC_LED_H_

#include "gpio.h"

// Structure to configure a single LED
typedef struct {
    uint16_t GPIO_user_label;  // GPIO pin label associated with the LED
    GPIO_TypeDef* GPIO_Port;   // GPIO port where the LED is connected
    GPIO_PinState state;       // Current state of the LED (on/off)
} ledConfig;

// Structure to manage multiple LEDs
typedef struct {
    ledConfig green_Led;  // Green LED configuration
    ledConfig yellow_Led; // Yellow LED configuration
    ledConfig red_Led;    // Red LED configuration
} LedArray;

/* Initializes an LED with a given GPIO pin, port, and initial state. */
void led_init (ledConfig* led, uint16_t user_label, GPIO_TypeDef* port, GPIO_PinState init_state);

/* Turns on the specified LED. */
void turnON_led(ledConfig* led);

/* Turns off the specified LED. */
void turnOFF_led(ledConfig* led);

/* Controls the LEDs and turns on the specified one from the array. */
void control_led(LedArray* leds, ledConfig* led_to_turn_on);

#endif /* INC_LED_H_ */
