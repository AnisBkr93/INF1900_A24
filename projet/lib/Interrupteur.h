#pragma once
#define F_CPU 8000000
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
/*
Auteurs: Anis Boukerdoune, Emmanuel Berberi, Buntep Chea, Fadi Mousa
Date: 2024-12-03
Description: Classe Interrupteur permettant de gérer une interruption externe
et de détecter un appui stable sur un interrupteur avec un anti-rebond.
Identifications matérielles: Broches PD2 et PA3.
*/

class Interrupteur
{
public :
    Interrupteur();
    
    ~Interrupteur();

    void initialisation();

    bool antiRebound();

};
