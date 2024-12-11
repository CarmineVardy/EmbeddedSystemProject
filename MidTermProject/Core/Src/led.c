/*
 * led.c
 *
 *  Created on: Dec 9, 2024
 *      Author: utente
 */

#include "led.h"


void led_init (ledConfig* led, uint16_t user_label, GPIO_TypeDef* port, GPIO_PinState init_state){

	led->GPIO_user_label = user_label;
	led->GPIO_Port = port;
	led->state = init_state;

}

void turnON_led(ledConfig* led){

	if (led->state != GPIO_PIN_SET) {  // Se il LED non è già acceso
        HAL_GPIO_WritePin(led->GPIO_Port, led->GPIO_user_label, GPIO_PIN_SET);  // Accendi il LED
        led->state = GPIO_PIN_SET;  // Aggiorna lo stato del LED
    }

}

void turnOFF_led(ledConfig* led){

	if (led->state != GPIO_PIN_RESET) {  // Se il LED non è già spento
        HAL_GPIO_WritePin(led->GPIO_Port, led->GPIO_user_label, GPIO_PIN_RESET);  // Spegni il LED
        led->state = GPIO_PIN_RESET;  // Aggiorna lo stato del LED
    }

}
