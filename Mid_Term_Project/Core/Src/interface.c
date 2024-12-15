/*
 * interface.c
 *
 *  Created on: Dec 12, 2024
 *      Author: utente
 */

#include "interface.h"
#include <stdio.h>
#include <stdint.h>
#include <string.h>

// Funzione per cancellare lo schermo
void clear_screen() {
    printf("\033[H\033[J");
}

// Funzione per inizializzare l'interfaccia
void init_interface() {
    clear_screen();

    printf("        +---------------------------------------------+\r\n");
    printf("        |               SMARTWATCH                    |\r\n");
    printf("        |                 GROUP 1                     |\r\n");
    printf("        |     Carmine Vardaro, Alessia Parente,       |\r\n");
    printf("        |            Antonio D'Arienzo                |\r\n");
    printf("        +---------------------------------------------+\r\n");
    printf("        |          TEMPERATURE MONITORING             |\r\n");
    printf("        | Temp: 0.0 \xB0 C                               |\r\n");
    printf("        |                                             |\r\n");
    printf("        |                                             |\r\n");
    printf("        +---------------------------------------------+\r\n");
    printf("        |             HEART MONITORING                |\r\n");
    printf("        | Heart Rate: 0 bpm                           |\r\n");
    printf("        | HR Variability: 0.0 ms                      |\r\n");
    printf("        |                                             |\r\n");
    printf("        |                                             |\r\n");
    printf("        +---------------------------------------------+\r\n");
    printf("        |           MOVEMENT MONITORING               |\r\n");
    printf("        | Steps: 0                                    |\r\n");
    printf("        | Activity: Standing...                       |\r\n");
    printf("        | Red LED is on                               |\r\n");
    printf("        +---------------------------------------------+\r\n");
    printf("        |               INFORMATION                   |\r\n");
    printf("        | Press button 1 to measure body temperature  |\r\n");
    printf("        | Press button 2 to monitor heart             |\r\n");
    printf("        +---------------------------------------------+\r\n");
}

void update_steps(int steps) {
    // Posiziona il cursore sulla riga 17, colonna 18 (dove inizia il numero dei passi)
    printf("\033[19;18H");

    // Cancella la parte del numero precedente (fino a 5 caratteri)
    printf("     ");  // Riempie con spazi per eliminare residui di numeri più lunghi

    // Torna alla posizione iniziale del numero
    printf("\033[19;18H");

    // Stampa il numero con la formattazione desiderata
    if (steps < 10) {
        printf("%d    ", steps); // Singola cifra seguita da quattro spazi
    } else if (steps < 100) {
        printf("%d   ", steps);  // Due cifre seguite da tre spazi
    } else if (steps < 1000) {
        printf("%d  ", steps);   // Tre cifre seguite da due spazi
    } else if (steps < 10000) {
        printf("%d ", steps);    // Quattro cifre seguite da uno spazio
    } else if (steps < 100000) {
        printf("%d", steps);     // Cinque cifre senza spazi
    } else {
        printf("#####");         // Gestione di valori fuori scala
    }

    fflush(stdout); // Assicura che il buffer venga svuotato immediatamente
}

void update_activity(const char* activity) {

	printf("\033[20;21H");
	printf("           ");  // Riempie con spazi per eliminare residui di numeri più lunghi
	printf("\033[20;21H");

    if (strcmp(activity, "RESTING") == 0) {
            printf("Standing...");
        } else if (strcmp(activity, "WALKING") == 0) {
            printf("Walking....");
        } else if (strcmp(activity, "RUNNING") == 0) {
            printf("Running....");
        } else {
            printf("Unknown....");
        }

    printf("\033[21;11H");
    printf("                ");
    printf("\033[21;11H");

    if (strcmp(activity, "RESTING") == 0) {
    	printf("Red LED is on   ");
    } else if (strcmp(activity, "WALKING") == 0) {
    	printf("Yellow LED is on");
    } else if (strcmp(activity, "RUNNING") == 0) {
    	printf("Green LED is on ");
    } else {
    	printf("Unknown....     ");
    }

    fflush(stdout);
}

void update_temperature(float temp){

	// Posiziona il cursore sulla riga 17, colonna 18 (dove inizia il numero dei passi)
	printf("\033[8;17H");

	// Cancella la parte del numero precedente (fino a 5 caratteri)
	printf("        ");  // Riempie con spazi per eliminare residui di numeri più lunghi

	// Torna alla posizione iniziale del numero
	printf("\033[8;17H");

	printf("%.1f \xB0 C \n", temp );

	fflush(stdout); // Assicura che il buffer venga svuotato immediatamente

}

void update_temperature_progress(uint8_t progress){

	printf("\033[10;15H");

		// Cancella la parte del numero precedente (fino a 5 caratteri)
	printf("                         ");  // Riempie con spazi per eliminare residui di numeri più lunghi

		// Torna alla posizione iniziale del numero
	printf("\033[10;15H");


	 printf("  [");
	     for (int i = 0; i < 25; i++) {
	         printf(i < (progress / 4) ? "#" : "-");
	     }
	     printf("] %d%%", progress);

	fflush(stdout);


}

void update_temperature_message(){

	// Posiziona il cursore sulla riga 17, colonna 18 (dove inizia il numero dei passi)
	printf("\033[9;10H");

	// Cancella la parte del numero precedente (fino a 5 caratteri)
	printf("                                             ");  // Riempie con spazi per eliminare residui di numeri più lunghi

	// Torna alla posizione iniziale del numero
	printf("\033[9;10H");

	printf("calculating temperature..possible variation..");

	fflush(stdout); // Assicura che il buffer venga svuotato immediatamente

}

void clear_temp_interface(){

	printf("\033[9;10H");
	printf("                                             ");

	printf("\033[10;11H");
	printf("                                            ");

	fflush(stdout);

}



