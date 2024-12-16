/*
 * activity_tracking.c
 *
 *  Created on: Dec 12, 2024
 *      Author: utente
 */

#include "activity_tracking.h"
#include "led.h"
#include "interface.h"
#include <stdio.h>

void init_fsr_sensor(FSR_Sensor *sensor) {
    sensor->data = 0.0f;
    sensor->foot_on_ground = 0;  // Piede sollevato inizialmente
    sensor->prev_foot_on_ground = 0; // Stato precedente inizialmente sollevato
}

void UserActivity_init(UserActivity *user_activity, LedArray *ledArray) {
    user_activity->steps = 0;
    user_activity->last_step_time = 0;
    user_activity->consecutive_steps = 0;
    init_fsr_sensor(&user_activity->left_foot);
    init_fsr_sensor(&user_activity->right_foot);
    user_activity->state = RESTING;  // Imposta lo stato a RESTING
    control_led(ledArray, &ledArray->red_Led);  // Accende il LED rosso per indicare lo stato di riposo
}


void StepBuffer_init(StepBuffer *buffer, uint32_t* steps, uint16_t length) {

	buffer->buffer = steps;
	buffer->total_step_time = 0.0;
	buffer->index = 0;
	buffer->length = length;
	buffer->filled = 0;


    for (uint16_t i = 0; i < length; i++) {
    	steps[i] = 0;
    }

}

void read_forceSensor(uint16_t *d_out, FSR_Sensor *sensor) {
    float voltage = ((float)(*d_out)) * VREF / LEVELS;
    sensor->data = voltage;
    sensor->prev_foot_on_ground = sensor->foot_on_ground;
    sensor->foot_on_ground = (voltage > STEP_THRESHOLD) ? 1 : 0;
}

void update_step_count(UserActivity *user_activity, uint32_t current_time, StepBuffer *step_buffer, LedArray *ledArray) {
    uint32_t step_time; // Variabile per calcolare il tempo tra due passi consecutivi

    // Controlla il piede sinistro
    if (user_activity->left_foot.foot_on_ground && !user_activity->left_foot.prev_foot_on_ground) {
        // Se il piede sinistro tocca il suolo e il piede destro non è a terra
        if (user_activity->right_foot.foot_on_ground == 0) {
            // Calcola il tempo del passo
            step_time = current_time - user_activity->last_step_time;
            // Incrementa il conteggio totale dei passi
            user_activity->steps++;
            update_steps(user_activity->steps);  // Aggiorna l'interfaccia con il numero di passi corrente
            // Aggiunge il tempo del passo al buffer dei passi
            add_step_time(step_buffer, step_time);
            // Aggiorna il timestamp dell'ultimo passo
            user_activity->last_step_time = current_time;

            // Incrementa il conteggio dei passi consecutivi
            user_activity->consecutive_steps++;

            // Se l'utente era fermo e ha fatto abbastanza passi consecutivi, passa a camminare
            if (user_activity->state == RESTING && user_activity->consecutive_steps >= MIN_WALKING_STEPS) {
                set_walking_state(user_activity, ledArray);  // Chiamata alla funzione di gestione dello stato WALKING
            }
        }
    }

    // Controlla il piede destro
    if (user_activity->right_foot.foot_on_ground && !user_activity->right_foot.prev_foot_on_ground) {
        // Se il piede destro tocca il suolo e il piede sinistro non è a terra
        if (user_activity->left_foot.foot_on_ground == 0) {
            // Calcola il tempo del passo
            step_time = current_time - user_activity->last_step_time;
            // Incrementa il conteggio totale dei passi
            user_activity->steps++;
            update_steps(user_activity->steps);  // Aggiorna l'interfaccia con il numero di passi corrente
            // Aggiunge il tempo del passo al buffer dei passi
            add_step_time(step_buffer, step_time);
            // Aggiorna il timestamp dell'ultimo passo
            user_activity->last_step_time = current_time;

            // Incrementa il conteggio dei passi consecutivi
            user_activity->consecutive_steps++;

            // Se l'utente era fermo e ha fatto abbastanza passi consecutivi, passa a camminare
            if (user_activity->state == RESTING && user_activity->consecutive_steps >= MIN_WALKING_STEPS) {
                set_walking_state(user_activity, ledArray);  // Chiamata alla funzione di gestione dello stato WALKING
            }
        }
    }
    // Determina l'attività in base all'inattività o ai passi effettuati
    determine_activity(user_activity, current_time, step_buffer, ledArray);
}




void determine_activity(UserActivity *user_activity, uint32_t current_time, StepBuffer *step_buffer, LedArray *ledArray) {
    if (current_time - user_activity->last_step_time >= INACTIVITY_TIMEOUT) {
    	set_resting_state(user_activity, ledArray);
        user_activity->consecutive_steps = 0;  // Reset dei passi consecutivi
        reset_step_buffer(step_buffer); // Reset dello StepBuffer, se mi fermo non mi baso più sui valori precedenti
    } else if (user_activity->state != RESTING) { // Verifica che non sia in stato RESTING
        uint32_t average_time = 0;

        if (step_buffer->filled > 0) {
            average_time = step_buffer->total_step_time / step_buffer->filled;
        }

        if (average_time < RUNNING_THRESHOLD) {
        	set_running_state(user_activity, ledArray);
        } else {
        	set_walking_state(user_activity, ledArray);
        }
    }
}


void add_step_time(StepBuffer *buffer, uint32_t step_time) {
	buffer->total_step_time += step_time - buffer->buffer[buffer->index];
	buffer->buffer[buffer->index] = step_time;
	buffer->index = (buffer->index + 1) % buffer->length;
	if(buffer->filled < buffer->length){
		buffer->filled++;
	}
}

void reset_step_buffer(StepBuffer *buffer) {
    buffer->total_step_time = 0;       // Azzerare il tempo totale
    buffer->index = 0;                // Ripartire dall'inizio
    buffer->filled = 0;               // Indicare che non ci sono valori
    for (uint16_t i = 0; i < buffer->length; i++) {
        buffer->buffer[i] = 0;        // Facoltativo: azzerare i valori nel buffer
    }
}


const char* userState_to_string(UserState state) {
    switch (state) {
        case RESTING: return "RESTING";
        case WALKING: return "WALKING";
        case RUNNING: return "RUNNING";
        default: return "UNKNOWN";
    }
}

void set_walking_state(UserActivity *user_activity, LedArray *ledArray) {
    // Cambia stato solo se l'utente non è già in stato WALKING
    if (user_activity->state != WALKING) {
        user_activity->state = WALKING;  // Imposta lo stato a WALKING
        control_led(ledArray, &ledArray->yellow_Led);  // Accende il LED giallo per indicare lo stato di camminata
        update_activity(userState_to_string(WALKING)); //Aggiorno l'interfaccia con la nuova attività

    }
}

void set_running_state(UserActivity *user_activity, LedArray *ledArray) {
    // Cambia stato solo se l'utente non è già in stato RUNNING
    if (user_activity->state != RUNNING) {
        user_activity->state = RUNNING;  // Imposta lo stato a RUNNING
        control_led(ledArray, &ledArray->green_Led);  // Accende il LED verde per indicare lo stato di corsa
        update_activity(userState_to_string(RUNNING)); //Aggiorno l'interfaccia con la nuova attività
    }
}

void set_resting_state(UserActivity *user_activity, LedArray *ledArray) {
    // Cambia stato solo se l'utente non è già in stato RESTING
    if (user_activity->state != RESTING) {
        user_activity->state = RESTING;  // Imposta lo stato a RESTING
        control_led(ledArray, &ledArray->red_Led);  // Accende il LED rosso per indicare lo stato di riposo
        update_activity(userState_to_string(RESTING)); //Aggiorno l'interfaccia con la nuova attività
    }
}




