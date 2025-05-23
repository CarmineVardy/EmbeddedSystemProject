/*
 * Project: Wearable System for Health Monitoring
 * Authors: Carmine Vardaro, Alessia Parente, Antonio D'Arienzo
 *
 * Licensed under the GNU General Public License v3.0 (GPL v3).
 * For more details, see: https://www.gnu.org/licenses/gpl-3.0.html
 *
 * Description:
 * This file contains the implementation of functions to control the LEDs in the wearable health monitoring system.
 * The LEDs provide feedback on the user's activity state (resting, walking, running), and the functions allow
 * turning LEDs on/off and managing multiple LEDs within an array.
 */

#include "led.h"

/* Initializes the configuration of the LED with GPIO pin, port, and initial state. */
void led_init (ledConfig* led, uint16_t user_label, GPIO_TypeDef* port, GPIO_PinState init_state) {
	led->GPIO_user_label = user_label;  // Set the GPIO pin label
	led->GPIO_Port = port;  // Set the GPIO port
	led->state = init_state;  // Set the initial state (on/off)
}

/* Turns on the specified LED if it is currently off. */
void turnON_led(ledConfig* led) {
    if (led->state == GPIO_PIN_RESET) { // Turn on the LED only if it is off
        HAL_GPIO_WritePin(led->GPIO_Port, led->GPIO_user_label, GPIO_PIN_SET);
        led->state = GPIO_PIN_SET; // Update the state to "on"
    }
}

/* Turns off the specified LED if it is currently on. */
void turnOFF_led(ledConfig* led) {
    if (led->state == GPIO_PIN_SET) { // Turn off the LED only if it is on
        HAL_GPIO_WritePin(led->GPIO_Port, led->GPIO_user_label, GPIO_PIN_RESET);
        led->state = GPIO_PIN_RESET; // Update the state to "off"
    }
}

/* Controls the LEDs: turns on the specified LED and turns off the others. */
void control_led(LedArray* leds, ledConfig* led_to_turn_on) {
    // Turns on the selected LED if it is currently off
    if (led_to_turn_on->state == GPIO_PIN_RESET) {
        turnON_led(led_to_turn_on);
    }

    // Turns off the other LEDs
    if (&leds->green_Led != led_to_turn_on) {
        turnOFF_led(&leds->green_Led);
    }
    if (&leds->yellow_Led != led_to_turn_on) {
        turnOFF_led(&leds->yellow_Led);
    }
    if (&leds->red_Led != led_to_turn_on) {
        turnOFF_led(&leds->red_Led);
    }
}
