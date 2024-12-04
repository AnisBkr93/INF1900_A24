#pragma once
#define F_CPU 8000000
#ifndef MOTEUR_H
#define MOTEUR_H
#include "Moteur.h"



Moteur::Moteur(){
    // compteur a 0
    DDRD |= (1 << PD5) | (1 << PD4) | (1 << PD2) | (1 << PD3); // PD5 et PD4 pour les PWM et PD6 et PD7 pour la direction des roues
    
    TCCR1A |= (1 << WGM10) | (1 << COM1A1) | (1 << COM1B1); // 8-bit FAST PWM sur PD5 et PD4
    TCCR1B |= (1 << CS11); // Prescaler à 8

    OCR1A = 0; // PWM à 0% (roue ne tourne pas), valeur max 255 pour 100%
    OCR1B = 0;
    
}

Moteur::~Moteur(){
}

void Moteur::changerVitesse(uint8_t pwmA , uint8_t pwmB){
    OCR1A = pwmA;
    OCR1B = pwmB;
}

void Moteur::avancerMoteur(uint8_t pwmA , uint8_t pwmB) {
    PORTD &= ~(1 << PD2);
    PORTD &= ~(1 << PD3);
    changerVitesse(pwmA , pwmB);
}

// permet de garger la meme vitesse que derniere fonctionnement
void Moteur::avancerMoteur() {
    PORTD &= ~(1 << PD2);
    PORTD &= ~(1 << PD3);
}

void Moteur::reculerMoteur(uint8_t pwmA , uint8_t pwmB){
    PORTD |= (1 << PD2); 
    PORTD |= (1 << PD3);
    changerVitesse(pwmA , pwmB);
}

void Moteur::reculerMoteur(){
    PORTD |= (1 << PD2); 
    PORTD |= (1 << PD3);
}

// 90 deg
void Moteur::tournerGauche(){
    PORTD &= ~(1 << PD2);
    PORTD &= ~(1 << PD3);
    changerVitesse(100, 40); // a tester
}

void Moteur::tournerGauche(uint8_t pwmA , uint8_t pwmB){
    PORTD &= ~(1 << PD2);
    PORTD &= ~(1 << PD3);
    changerVitesse(pwmA , pwmB);
}

bool Moteur::tournerAngleGauche3(MakerLineRead lineReader){
  
    PORTD |= (1 << PD2);
    PORTD &= ~(1 << PD3);
    changerVitesse(150, 150);
    _delay_ms(1280);
    arreterMoteur();
    _delay_ms(250);
   return lineReader.getSensorCount()==0;
}

// 90 deg
void Moteur::tournerDroite(){ 
    PORTD &= ~(1 << PD2);
    PORTD &= ~(1 << PD3);
    changerVitesse(40, 100); // a tester
}

void Moteur::tournerDroite(uint8_t pwmA , uint8_t pwmB){
    PORTD &= ~(1 << PD2);
    PORTD &= ~(1 << PD3);
    changerVitesse(pwmA , pwmB);
}

void Moteur::arreterMoteur(){
    changerVitesse(0,0);
}

bool Moteur::tournerAngleGauche(MakerLineRead lineReader){
    bool middleDeadEndFound = false;
    PORTD |= (1 << PD2);
    PORTD &= ~(1 << PD3);
    changerVitesse(0 , 150);
    for(int i = 0;i < 90; i++){
         middleDeadEndFound = middleDeadEndFound | (lineReader.getSensorCount()!=0);
         _delay_ms(12);
    }
    _delay_ms(200);
    arreterMoteur();
    _delay_ms(250);
    return middleDeadEndFound;
}

bool Moteur::tournerAngleDroite(MakerLineRead lineReader){
    PORTD &= ~(1 << PD2);
    PORTD |= (1 << PD3);
    changerVitesse(150 , 150);
    _delay_ms(1200);
    arreterMoteur();
    _delay_ms(250);
    return lineReader.getSensorCount()==0;
}

bool Moteur::avancerPetiteLigne(MakerLineRead lineReader){
    bool deadEndTrouver = false;
    PORTD &= ~(1 << PD2);
    PORTD &= ~(1 << PD3);
    changerVitesse(150 , 113);
    _delay_ms(1300);
    arreterMoteur();
    _delay_ms(250);
    return true;
}

void Moteur::reculerPetiteLigne(){
    PORTD |= (1 << PD2);
    PORTD |= (1 << PD3);
    changerVitesse(105 , 80);
    _delay_ms(1750);
    arreterMoteur();
    _delay_ms(250);
}

bool Moteur::avancerPetiteLigne2(MakerLineRead lineReader){
    bool deadEndTrouver = false;
    PORTD &= ~(1 << PD2);
    PORTD &= ~(1 << PD3);
    changerVitesse(150 , 150);
     for(int i = 0;i<180;i++){
        
        _delay_ms(5);
        deadEndTrouver = deadEndTrouver | (lineReader.getSensorCount()==0);
    }
    arreterMoteur();
     _delay_ms(50);
     return deadEndTrouver;
}

void Moteur::reculerPetiteLigne2(){
    PORTD |= (1 << PD2);
    PORTD |= (1 << PD3);
    changerVitesse(200 , 150);
    _delay_ms(500);
    arreterMoteur();
    _delay_ms(50);

}

void Moteur::incrementalGauche(){
      PORTD |= (1 << PD2);
    PORTD &= ~(1 << PD3);
    changerVitesse(235,255);
    _delay_ms(50);
    arreterMoteur();
    _delay_ms(100);
}

void Moteur::incrementalDroite(){
     PORTD &= ~(1 << PD2);
    PORTD |= (1 << PD3);
    changerVitesse(255,255);
    _delay_ms(50);
    arreterMoteur();
    _delay_ms(100);

}
void Moteur::tournerAngleGauche2(){
      
    PORTD |= (1 << PD2);
    PORTD &= ~(1 << PD3);
    changerVitesse(160 , 150);
   
    _delay_ms(1100);
    arreterMoteur();
    _delay_ms(250);
    
}

void Moteur::tactactac(){
    PORTD |= (1 << PD2);
    PORTD &= ~(1 << PD3);
    changerVitesse(255,255);
    _delay_ms(100);
    arreterMoteur();


}
#endif /* MOTEUR_H */