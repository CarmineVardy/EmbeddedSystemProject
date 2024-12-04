/*
 * button.c
 *
 *  Created on: Mar 2, 2024
 *      Author: Martino Giaquinto
 */


#include "button.h"

void button_init (buttonDef* button, GPIO_TypeDef* GPIO_Port, uint16_t GPIO_button_Pin, GPIO_PinState InitState)
{
	/**
	 * @brief Initializes the button structure with the provided parameters.
	 *
	 * @param button       		Pointer to the buttonDef struct.
	 * @param GPIO_Port     	GPIO Port to which the button is connected.
	 * @param GPIO_button_Pin 	GPIO pin of the button.
	 * @param InitState     	Initial state of the button (pressed or released).
	 */

	button->GPIOx = GPIO_Port;
	button->GPIO_Pin = GPIO_button_Pin;
	button->temp_state = InitState;
	button->stable_state = InitState;
	button->count = 0;
}

#include "stdio.h"
GPIO_PinState read_button(buttonDef* button)
{

	/**
	 * @brief Reads the state of the button with debounce in non-blocking mode
	 *
	 * @param button        Pointer to the buttonDef structure representing the button.
	 *
	 * @return GPIO_PinState    The stable state of the button
	 */

	GPIO_PinState current_state = HAL_GPIO_ReadPin(button->GPIOx, button->GPIO_Pin);

	if (current_state == button->temp_state) //Stable read
	{
		button->count++;
		if (button->count >= COUNT_LIMIT)
		{
			button->count = 0;
			button->stable_state = current_state; //set the stable state
		}
		//NOTE: The stable time depends on the duration of other instruction in the while(1) loop.

	}	else //Unstable read
		{
			button->count = 0;
			button->temp_state = current_state; //update the temp state but not set the stable state
		}

	return button->stable_state;
}
