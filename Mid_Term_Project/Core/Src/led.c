/*
 * led.c
 *
 *  Created on: Dec 12, 2024
 *      Author: utente
 */

#include "led.h"

// Funzione per inizializzare la configurazione del LED
void led_init (ledConfig* led, uint16_t user_label, GPIO_TypeDef* port, GPIO_PinState init_state){
	led->GPIO_user_label = user_label;  // Imposta l'etichetta del pin GPIO
	led->GPIO_Port = port;  // Imposta la porta GPIO
	led->state = init_state;  // Imposta lo stato iniziale del LED (acceso/spento)
}

void turnON_led(ledConfig* led) {
    if (led->state == GPIO_PIN_RESET) { // Accende il LED solo se è spento
        HAL_GPIO_WritePin(led->GPIO_Port, led->GPIO_user_label, GPIO_PIN_SET);
        led->state = GPIO_PIN_SET; // Aggiorna lo stato a acceso
    }
}

void turnOFF_led(ledConfig* led) {
    if (led->state == GPIO_PIN_SET) { // Spegne il LED solo se è acceso
        HAL_GPIO_WritePin(led->GPIO_Port, led->GPIO_user_label, GPIO_PIN_RESET);
        led->state = GPIO_PIN_RESET; // Aggiorna lo stato a spento
    }
}

void control_led(LedArray* leds, ledConfig* led_to_turn_on) {
    // Accende il LED passato e spegne gli altri due
    if (led_to_turn_on->state == GPIO_PIN_RESET) {
        turnON_led(led_to_turn_on);
    }

    // Spegne gli altri LED
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
