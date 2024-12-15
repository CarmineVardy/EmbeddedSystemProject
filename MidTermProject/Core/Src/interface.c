/*
 * interface.c
 *
 *  Created on: Dec 10, 2024
 *      Author: utente
 */

#include "interface.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>  // Per sleep

// Variabile globale per tracciare se la frase è già stata stampata
static int calculation_started = 0;

// Funzione per cancellare lo schermo (per Linux/MacOS)
void clear_screen() {
    printf("\033[H\033[J");
}

// Funzione per inizializzare l'interfaccia dello smartwatch
void init_interface() {
    clear_screen();
    printf("+-------------------------------+\n");
    printf("|          SMARTWATCH            |\n");
    printf("+-------------------------------+\n");
    printf("| Time: 12:34 PM                 |\n");
    printf("+-------------------------------+\n");
    printf("| Temp: 0.0 C                    |\n");
    printf("|-------------------------------|\n");
    printf("| Heart Rate: 0 bpm              |\n");
    printf("|-------------------------------|\n");
    printf("| Steps: 0 steps                 |\n");
    printf("| Progress: -------------------- |\n");
    printf("+-------------------------------+\n");
    printf("| Battery: 85%% | Status: Active  |\n");
    printf("+-------------------------------+\n");
    printf("| Gruppo 1:                     |\n");
    printf("| Carmine Vardaro               |\n");
    printf("| Alessia Parente               |\n");
    printf("| Antonio D'Arienzo             |\n");
    printf("+-------------------------------+\n");
}

// Funzione per aggiornare la temperatura
void update_temperature(float new_temperature) {
    printf("\033[A");  // Sposta il cursore di una riga in alto
    printf("\033[K");  // Cancella la riga corrente
    printf("| Temp: %.1f C                    |\n", new_temperature);
}

// Funzione per aggiornare il battito cardiaco
void update_heart_rate(int new_heart_rate) {
    printf("\033[A");  // Sposta il cursore di una riga in alto
    printf("\033[K");  // Cancella la riga corrente
    printf("| Heart Rate: %d bpm              |\n", new_heart_rate);
}

// Funzione per aggiornare il numero di passi
void update_steps(int new_step_count) {
    printf("\033[A");  // Sposta il cursore di una riga in alto
    printf("\033[K");  // Cancella la riga corrente
    printf("| Steps: %d steps                 |\n", new_step_count);
}

// Funzione per aggiungere una riga con un calcolo
void update_calculation(int heart_rate, int step_count) {
    // Stampa "Calcolo in corso..." solo una volta
    if (!calculation_started) {
        printf("| Calcolo in corso...             |\n");
        calculation_started = 1;  // Imposta la variabile per non ripetere la frase
    }

    int result = heart_rate * step_count;  // Esempio di calcolo
    printf("| Calculation Result: %d          |\n", result);
}

// Funzione per aggiornare lo stato dell'attività
void update_activity_status(const char *status) {
    printf("\033[A");  // Sposta il cursore di una riga in alto
    printf("\033[K");  // Cancella la riga corrente
    printf("| Status: %s                    |\n", status);
}


/*
Nel main
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>  // Per sleep
#include "interface.h"

int main() {
    // Inizializzazione dell'interfaccia
    init_interface();

    // Variabili di esempio
    float temperature = 37.5;  // Temperatura
    int heart_rate = 75;       // Battito cardiaco
    int step_count = 850;      // Contatore passi

    // Simulazione dello smartwatch
    while (1) {
        // Aggiorna la temperatura
        temperature += 0.1;
        update_temperature(temperature);

        // Aggiorna il battito cardiaco
        heart_rate = (heart_rate + 1) % 120;
        update_heart_rate(heart_rate);

        // Aggiorna il contatore dei passi
        step_count += 1;
        if (step_count > 1000) step_count = 0;  // Reset del contatore dopo 1000 passi
        update_steps(step_count);

        // Aggiungi una riga con un calcolo (ad esempio: battito * passi)
        update_calculation(heart_rate, step_count);

        // Cambia lo stato (fermo, camminando, correndo)
        if (step_count == 0) {
            update_activity_status("Fermo");
        } else if (step_count < 500) {
            update_activity_status("Camminando");
        } else {
            update_activity_status("Correndo");
        }

        // Pausa di 1 secondo
        sleep(1);
    }

    return 0;
}
 *
 */

