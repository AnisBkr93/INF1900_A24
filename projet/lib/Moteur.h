#pragma once
#define F_CPU 8000000
#include <avr/io.h> 
#include <util/delay.h>
#include "MakerLineRead.h"
/*
Auteurs: Anis Boukerdoune, Emmanuel Berberi, Buntep Chea, Fadi Mousa
Date: 2024-12-03
Description: Classe Moteur pour contrôler les moteurs du robot. Elle gère
les déplacements, les virages, et les ajustements précis en utilisant le
PWM pour la vitesse et des broches spécifiques pour la direction.
Identifications matérielles: PD5 et PD4 (PWM), PD2 et PD3 (direction).
*/


class Moteur{
public:

    Moteur();
    ~Moteur();

    void changerVitesse(uint8_t pwmA, uint8_t pwmB);
    void avancerMoteur(uint8_t pwmA , uint8_t pwmB);
    void avancerMoteur(); 
    void reculerMoteur(uint8_t pwmA , uint8_t pwmB);
    void reculerMoteur();
    void tournerGauche();
    void tournerGauche(uint8_t pwmA , uint8_t pwmB);
    void tournerDroite();
    void tournerDroite(uint8_t pwmA , uint8_t pwmB);
    void arreterMoteur();
    bool tournerAngleGauche(MakerLineRead lineReader);
    bool tournerAngleDroite(MakerLineRead lineReader);
    bool avancerPetiteLigne(MakerLineRead lineReader);
    bool avancerPetiteLigne2(MakerLineRead lineReader);
    void reculerPetiteLigne();
    void reculerPetiteLigne2();
    void incrementalGauche();
    void tournerAngleGauche2();
    void incrementalDroite();
    bool tournerAngleGauche3(MakerLineRead lineReader);
    void tactactac();


};