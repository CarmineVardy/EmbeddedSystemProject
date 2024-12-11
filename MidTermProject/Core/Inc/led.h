/*
 * led.h
 *
 *  Created on: Dec 9, 2024
 *      Author: utente
 */

#ifndef INC_LED_H_
#define INC_LED_H_

#include "gpio.h"

typedef struct {
	uint16_t GPIO_user_label;
	GPIO_TypeDef* GPIO_Port;
	GPIO_PinState state;
} ledConfig;


void led_init (ledConfig* led, uint16_t user_label, GPIO_TypeDef* port, GPIO_PinState init_state);
void turnON_led(ledConfig* led);
void turnOFF_led(ledConfig* led);


#endif /* INC_LED_H_ */
