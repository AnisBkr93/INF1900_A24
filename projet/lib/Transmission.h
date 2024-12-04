#pragma once
#define F_CPU 8000000
#include <avr/io.h>
#include <util/delay.h>
/*
Auteurs: Anis Boukerdoune, Emmanuel Berberi, Buntep Chea, Fadi Mousa
Date: 2024-12-03
Description: Classe Transmission permettant la communication UART. Elle
configure l'UART pour transmettre et recevoir des données, et inclut des
méthodes pour envoyer des octets ou des messages complets.
Identifications matérielles: Module UART intégré au microcontrôleur.
*/

class Transmission
{

public:

    Transmission(); 

    ~Transmission();

    void initialisationUART(); 

    void transmissionUART( uint8_t donnee );

    uint8_t receptionUART();

    void envoyerMessage(const char* message);


};