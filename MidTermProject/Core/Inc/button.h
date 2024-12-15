/*
 * button.h
 *
 *  Created on: Dec 9, 2024
 *      Author: utente
 */

#ifndef INC_BUTTON_H_
#define INC_BUTTON_H_

#include "gpio.h"

#define COUNT_LIMIT 2

typedef struct {
    uint16_t GPIO_user_label;
    GPIO_TypeDef* GPIO_Port;
    GPIO_PinState temp_state;
    GPIO_PinState stable_state;
    GPIO_PinState previous_state;
    uint8_t count;
} buttonConfig;



void button_init (buttonConfig* button, uint16_t user_label, GPIO_TypeDef* port, GPIO_PinState init_state);
GPIO_PinState read_button(buttonConfig* button);


#endif /* INC_BUTTON_H_ */
