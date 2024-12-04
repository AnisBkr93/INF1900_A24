
#pragma once
#define F_CPU 8000000
#include <avr/io.h>
/*
Auteurs: Anis Boukerdoune, Emmanuel Berberi, Buntep Chea, Fadi Mousa
Date: 2024-12-03
Description: Classe Son permettant de contrôler une sonnerie via un signal
PWM. Elle permet de jouer une fréquence spécifique ou d'arrêter le son.
Identifications matérielles: Broches PD7 pour le signal PWM.
*/


class Son 
{
public :  
    Son();

    ~Son();

    void runSong(double frequency);

    void stopSong();

};