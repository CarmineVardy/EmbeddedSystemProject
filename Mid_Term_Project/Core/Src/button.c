/*
 * button.c
 *
 *  Created on: Dec 12, 2024
 *      Author: utente
 */

#include "button.h"

/*
 * Funzione per inizializzare la configurazione di un pulsante.
 * Assegna i parametri passati alla struttura `buttonConfig`
 * e imposta gli stati iniziali del pulsante.
 */
void button_init(buttonConfig* button, uint16_t user_label, GPIO_TypeDef* port, GPIO_PinState init_state) {
    button->GPIO_user_label = user_label;    // Etichetta del pulsante
    button->GPIO_Port = port;                // Porta GPIO a cui è connesso
    button->temp_state = init_state;         // Stato temporaneo iniziale
    button->stable_state = init_state;       // Stato stabile iniziale
    button->previous_state = init_state;     // Stato precedente iniziale
    button->count = 0;                       // Contatore per il debounce
}

/*
 * Funzione per leggere lo stato del pulsante applicando il debounce.
 * Restituisce lo stato stabile (debounced) del pulsante.
 */
GPIO_PinState read_button(buttonConfig* button) {
    GPIO_PinState current_state = HAL_GPIO_ReadPin(button->GPIO_Port, button->GPIO_user_label);  // Legge lo stato attuale del pulsante

    if (current_state == button->temp_state) {  // Stato stabile se il nuovo stato coincide con quello temporaneo
        button->count++;                        // Incrementa il contatore per il debounce
        if (button->count >= COUNT_LIMIT) {     // Se il contatore supera la soglia
            button->count = 0;                  // Reset del contatore
            button->previous_state = button->stable_state;  // Memorizza lo stato stabile precedente
            button->stable_state = current_state;  // Aggiorna lo stato stabile
        }
    } else {
        button->count = 0;                      // Reset del contatore se lo stato è cambiato
        button->temp_state = current_state;     // Aggiorna lo stato temporaneo
    }

    return button->stable_state;  // Restituisce lo stato stabile del pulsante
}
