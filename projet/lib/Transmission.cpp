#include "Transmission.h"




Transmission::Transmission(){} 

Transmission::~Transmission(){}


void Transmission::initialisationUART() 
{
// 2400 bauds. Nous vous donnons la valeur des deux

// premiers registres pour vous éviter des complications.

UBRR0H = 0;
UBRR0L = 0xCF;   // 207 ( decimal ) pour configuerer les 2400 bauds

// UCSR0A (controle du =statut)
UCSR0A |= (1<<RXC0)|(1<<TXC0);
// UCSROB : registre de controle de UART PERMET D'ACTIVER OU DESACTIVER LES FONCTIONNALITES ( TRANSMIT / RECEIVE) 
UCSR0B |= (1<<RXEN0) | (1<<TXEN0) ;

// ON LES METS A 1 POUR ACTIVER LA RECEPTION/TRANSMIUSSION



// Configuration du format de trame : 8 bits de données
 UCSR0C |= (1 << UCSZ01) | (1 << UCSZ00);
}


void Transmission::transmissionUART( uint8_t donnee ) {

/* Wait for empty transmit buffer */
    while ( !( UCSR0A & (1<<UDRE0)) );
    /* Put data into buffer, sends the data */
    UDR0 = donnee ;  // suivant la doc de atmel 
    
}
// Fonction pour la réception d'un octet via UART
uint8_t Transmission::receptionUART() {
    // Attendre qu'une donnée soit reçue
    while (!(UCSR0A & (1 << RXC0)));
    return UDR0;  // Retourner la donnée reçue
}

// Fonction pour envoyer un message (chaîne de caractères)
void Transmission::envoyerMessage(const char* message) {
    for (int i = 0; message[i] != '\0'; ++i) {
        transmissionUART(message[i]);  // Transmettre chaque caractère
    }

}
