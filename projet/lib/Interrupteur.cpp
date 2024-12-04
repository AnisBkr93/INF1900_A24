#include "Interrupteur.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>


const uint8_t CONFIG_PORT_SORTIE = 0xFF;  
const uint8_t TEMPS_ANTI_REBOND = 10;     

Interrupteur::Interrupteur() {
    DDRD &= ~(1 << PD2); 
    DDRA = CONFIG_PORT_SORTIE; 
}

Interrupteur::~Interrupteur() {}

// Initialisation de l'interruption externe INT0
void Interrupteur::initialisation() {
    cli();  

    EIMSK |= (1 << INT0); // Active l'interruption externe INT0

    
    EICRA |= (1 << ISC00); // ISC00 = 1 : interruption sur n'importe quel changement de PD2 

    sei();  // Réactive les interruptions globales
}

// Fonction anti-rebond pour détecter un appui stable sur l'interrupteur
bool Interrupteur::antiRebound() {
    if (PINA & (1 << PA3)) { // Vérifie si PD2 est haut (appui sur l'interrupteur)
        _delay_ms(TEMPS_ANTI_REBOND); // Attente pour ignorer les rebonds rapides

        if (PINA & (1 << PA3)) { // Re-vérifie si PD2 est toujours haut après le délai
            return true; // Confirme l'appui si le signal est stable
        }
    }
    return false; // Retourne false si aucun appui stable n'est détecté
}
