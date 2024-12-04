#include "MakerLineRead.h"

MakerLineRead::MakerLineRead() {
    DDRC &= ~((1 << PC5) | (1 << PC7) | (1 << PC2) | (1 << PC3) | (1 << PC4) ); // Configure les broches comme entrées
}

MakerLineRead::~MakerLineRead() {}

// Lit l'état des capteurs
void MakerLineRead::readSensors() {
    uint8_t sensorState = PINC;// Stocker l'entiereter de l'etat des sensors

    sensorLeftEdge = (PINC & (1 << PC5));
    sensorLeft = (PINC & (1 << PC7));
    sensorCenter = (PINC & (1 << PC2));
    sensorRight = (PINC & (1 << PC3));
    sensorRightEdge = (PINC & (1 << PC4));
}

// Retourne le nombre de capteurs activés
uint8_t MakerLineRead::getSensorCount() {
    readSensors();
    uint8_t count = 0;
    if(sensorLeftEdge) { count++; }
    if(sensorLeft) { count++; }
    if(sensorCenter) { count++; }
    if(sensorRight) { count++; }
    if(sensorRightEdge) { count++; }
    return count;
}

// Détermine la position du robot par rapport à la ligne
char MakerLineRead::getLinePosition() {

    readSensors();
    uint8_t countRight = 0;
    uint8_t countLeft = 0;
    if(sensorLeftEdge) { countLeft++; }
    if(sensorLeft) { countLeft++; }
    if(sensorRight) { countRight++; }
    if(sensorRightEdge) { countRight++; }
    
     if (getSensorCount() == 0) {
        return 'n'; // 'n' stands for "no line detected"
    }
    
   
     if(countLeft > countRight) {
        isRight = false;
        isLeft = true;
        isMiddle = false;
        return 'g';
    }
    else if(countLeft < countRight) {
        isLeft = false;
        isRight = true;
        isMiddle = false;
        return 'd'; // Droite
    }

        isMiddle = true;
        isLeft = false;
        isRight = false;
        return 'm'; // Milieu
}

// Vérifie si le robot est sur une ligne (capteur central activé)
bool MakerLineRead::isOnLine() {
    readSensors();
    return sensorCenter; //&& sensorRightEdge;
}

// Vérifie si le robot a atteint une sortie
bool MakerLineRead::isAtExit() {
    readSensors();
    return sensorCenter && sensorLeftEdge;
}

// Vérifie si le capteur central détecte la ligne - does same thing as is online (we should probs remove one of them)
bool MakerLineRead::isCenter() {
    readSensors();
    return sensorCenter;
}

// Vérifie si le capteur de droite est activé
bool MakerLineRead::isRightEdge() {
    readSensors();
    return sensorRightEdge;
}

// Vérifie si le capteur de gauche est activé
// bool MakerLineRead::isLeftEdge() {
//     readSensors();
//     return sensorLeftEdge;
// }

// Détecte une intersection (tous les capteurs activés)
bool MakerLineRead::detectIntersection() {
    readSensors();
    return sensorLeftEdge && sensorLeft && sensorCenter && sensorRight && sensorRightEdge;
}

// Détecte un virage à angle droit vers la droite
bool MakerLineRead::detectRightTurn() {
    readSensors();
    return !sensorLeftEdge && !sensorLeft && sensorCenter && sensorRight && sensorRightEdge;
}

// Détecte un virage à angle droit vers la gauche
bool MakerLineRead::detectLeftTurn() {
    readSensors();
    return sensorLeftEdge && sensorLeft && sensorCenter && !sensorRight && !sensorRightEdge;
}

bool MakerLineRead::isLeftEdge() {
    readSensors();
    return sensorLeftEdge && sensorLeft && sensorCenter ;
}

bool MakerLineRead::isReallyCenter() {
    readSensors();
    return sensorLeftEdge && sensorCenter && sensorRightEdge && !sensorLeft && !sensorRight;
}