#pragma once
#include <avr/io.h>
#include <util/delay.h>
/*
Auteurs: Anis Boukerdoune, Emmanuel Berberi, Buntep Chea, Fadi Mousa
Date: 2024-12-03
Description: Classe MakerLineRead pour lire l'état des capteurs de suivi de
ligne et déterminer la position du robot. Elle détecte les intersections,
les virages, et indique si le robot est sur une ligne ou à une sortie.
Identifications matérielles: Capteurs connectés aux broches PC2 à PC7.
*/


class MakerLineRead {
public:
    MakerLineRead();
    ~MakerLineRead();

    // Méthodes pour lire et analyser les capteurs
    void readSensors();
    uint8_t getSensorCount();
    char getLinePosition();

    // Vérifications spécifiques
    bool isOnLine();
    bool isAtExit();
    bool isCenter();
    bool isRightEdge();
    bool isLeftEdge();

    // Détection avancée
    bool detectIntersection();
    bool detectRightTurn();
    bool detectLeftTurn();
    bool isReallyCenter();


private:
    // États des capteurs individuels
    bool sensorLeftEdge, sensorLeft, sensorCenter, sensorRight, sensorRightEdge;
    bool isRight, isLeft, isMiddle;
};

