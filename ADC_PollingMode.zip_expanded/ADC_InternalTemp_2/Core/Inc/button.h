/*
 * button.h
 *
 *  Created on: Mar 2, 2024
 *      Author: Martino Giaquinto
 */

#ifndef SRC_BUTTON_H_
#define SRC_BUTTON_H_

#include "gpio.h"

#define COUNT_LIMIT 2

typedef struct {
	GPIO_TypeDef* GPIOx;				//GPIO Port
	uint16_t GPIO_Pin;					//GPIO Pin connected to the button
	GPIO_PinState temp_state;			//Temporary stable state
	GPIO_PinState stable_state;			//State stable for count>= COUNT_LIMIT
	uint16_t count;
} buttonDef;

void button_init (buttonDef* button, GPIO_TypeDef* GPIO_Port, uint16_t GPIO_button_Pin, GPIO_PinState InitState);

GPIO_PinState read_button(buttonDef* button);

#endif /* SRC_BUTTON_H_ */
