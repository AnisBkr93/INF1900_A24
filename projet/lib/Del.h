#pragma once
#define F_CPU 8000000
#include <avr/io.h>
#include <util/delay.h>
/*
Auteurs: Anis Boukerdoune, Emmanuel Berberi, Buntep Chea, Fadi Mousa
Date: 2024-12-03
Description: Classe Del pour contrôler une LED bicolore (rouge, vert, ambre,
ou éteint).
Identifications matérielles: Deux broches configurées comme sorties.
*/
class Del {
public:
    Del(uint8_t pinA, uint8_t pinB);  // Constructeur avec pinA et pinB
    ~Del();

    // Initialise le port et les directions
    void initialiser(volatile uint8_t* ddrPort, volatile uint8_t* port);
    
    void rouge();   
    void vert();    
    void eteint();  
    void ambre();  

private:
    uint8_t pinA;   
    uint8_t pinB;   
    volatile uint8_t* port;  
};



