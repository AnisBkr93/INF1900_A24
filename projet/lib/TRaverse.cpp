#define F_CPU 8000000
#include <avr/io.h>
#include <util/delay.h>
#include "Moteur.h"
#include "MakerLineRead.h"
#include "Del.h"
#include "Transmission.h"
#include "memoire_24.h"
#include "Sonnerie.h"
#include "can.h"
#include <math.h>
#include "Interrupteur.h"
#include <stdlib.h>

#define DEBOUNCE_DELAY_TIME_MS 10

can convertisseurAnalogique;
Memoire24CXXX mem;
Moteur moteur;
MakerLineRead lineReader;
Son sonnerie;
Transmission t;

char debut;
bool stop1 = false;
bool stop2 = false;
bool stop3 = false;
bool stop4 = false;
bool stop5 = false;
bool stop6 = false;
bool firstRunDOne = false;
bool secondRunDone = false;
bool thirdRunDone = false;

uint8_t mode = 0; // Initial mode

uint8_t readButton(void) {
    uint8_t state1 = PINA & (1 << PA2);
    _delay_ms(DEBOUNCE_DELAY_TIME_MS);
    uint8_t state2 = PINA & (1 << PA2);

    if (state1 == state2)
        return state1;
    else
        return 0;
}

float convertirADCenDistance(uint16_t adcValue) {
    float voltage = (adcValue * 5.0) / 1023.0;
    float distance = 27.86 / pow(voltage, 1.15);
    return distance;
}

int moyenneDist() {
    int somme = 0;
    for (int i = 0; i < 3; i++) {
        float distance = convertirADCenDistance(convertisseurAnalogique.lecture(0x04));
        somme += distance;
    }
    return int(somme / 3);
}
void part0() {
    if (readButton()) {
        if(!firstRunDOne){
            mode = 1;
        }
        else if(!secondRunDone){
            mode = 2;
        }
         else if(!thirdRunDone){
             mode = 3;
         }
         else{
             mode = 4;
         }
    }


}



void part1() {
    int adcValue = 0;
    moteur.avancerMoteur(255,255);
        while(lineReader.getSensorCount()!=5){
            char position = lineReader.getLinePosition();
            // Ajuster les moteurs en fonction de la position détectée
            if (position == 'g') { // Ligne détectée à gauche
                moteur.tournerGauche(140, 90); // Ajuste vers la gauche
            } else if (position == 'd') { // Ligne détectée à droite
                moteur.tournerDroite(90, 120); // Ajuste vers la droite
            } else if (position == 'm') { // Ligne détectée au centre
                moteur.avancerMoteur(120, 110); // Avance tout droit
            } 
        }   
        moteur.arreterMoteur();
        _delay_ms(1000);
        moteur.avancerMoteur(255,255); //possible problem
        _delay_ms(150);
        for(int i = 0; i<115;i++){
            char position = lineReader.getLinePosition();

            // Ajuster les moteurs en fonction de la position détectée
            if (position == 'g') { // Ligne détectée à gauche
                moteur.tournerGauche(140, 90); // Ajuste vers la gauche

            } else if (position == 'd') { // Ligne détectée à droite
                moteur.tournerDroite(90, 120); // Ajuste vers la droite

            } else if (position == 'm') { // Ligne détectée au centre
                moteur.avancerMoteur(120, 110); // Avance tout droit
            } 
            _delay_ms(15);
        }

        moteur.arreterMoteur();

         _delay_ms(500);
        adcValue = moyenneDist();
        
       
        if(adcValue >=20 && adcValue <=40){
            while(adcValue > 10 ){
                char position = lineReader.getLinePosition();
                // Ajuster les moteurs en fonction de la position détectée
                if (position == 'g') { // Ligne détectée à gauche
                            
                    adcValue = moyenneDist();
 
                    moteur.tournerGauche(140, 90); // Ajuste vers la gauche
                                
                } else if (position == 'd') { // Ligne détectée à droite
                    adcValue = moyenneDist();
                
                    moteur.tournerDroite(90, 120); // Ajuste vers la droite

                } else if (position == 'm') { // Ligne détectée au centre
                    adcValue = moyenneDist();
                
                    moteur.avancerMoteur(120, 110); // Avance tout droit
                } 
            }
            moteur.arreterMoteur();
            for (int i = 0; i < 8; i++) {
                sonnerie.runSong(64); 
                _delay_ms(150);
                sonnerie.stopSong();
                _delay_ms(50);
            }
            firstRunDOne = true;
            mode = 0;// switch to mode 2
        }   
        else{
            moteur.incrementalDroite();
            moteur.incrementalDroite();
            moteur.incrementalDroite();
            if(!lineReader.isCenter()){
                while(!lineReader.isCenter()){
                    moteur.incrementalDroite();
                }
            }
            adcValue = moyenneDist();

            if(adcValue >=15 && adcValue <=40){
                while(adcValue > 10 ){
                    char position = lineReader.getLinePosition();
                    // Ajuster les moteurs en fonction de la position détectée
                    if (position == 'g') { // Ligne détectée à gauche
                            
                        adcValue = moyenneDist();
                        moteur.tournerGauche(140, 90); // Ajuste vers la gauche
                                
                    } else if (position == 'd') { // Ligne détectée à droite
                        adcValue = moyenneDist();
                        moteur.tournerDroite(90, 120); // Ajuste vers la droite

                    } else if (position == 'm') { // Ligne détectée au centre
                        adcValue = moyenneDist();
                        moteur.avancerMoteur(120, 110); // Avance tout droit
                    } 
                }
                moteur.arreterMoteur();
                for (int i = 0; i < 8; i++) {
                    sonnerie.runSong(64); 
                    _delay_ms(150);
                    sonnerie.stopSong();
                    _delay_ms(50);
                }  
                debut = 'D';
                mem.ecriture(100, 'D');
                mem.ecriture(101, '2');
                stop2 = true;
                firstRunDOne = true;
                mode = 0;// switch to mode 2
            }
            else{
                moteur.incrementalGauche();
                moteur.incrementalGauche();
                moteur.incrementalGauche();
                 while(!lineReader.isCenter()){
                     moteur.incrementalGauche(); 
                 }
                moteur.incrementalGauche();
                moteur.incrementalGauche();
                moteur.incrementalGauche();
                 moteur.incrementalGauche();
                moteur.incrementalGauche();
                moteur.incrementalGauche();
                // moteur.tournerAngleGauche(lineReader);
                // moteur.tournerAngleGauche(lineReader);
                // moteur.tournerAngleGauche(lineReader);
                while(!lineReader.isCenter()){
                    moteur.incrementalGauche(); 
                }
                adcValue = moyenneDist();
                while(adcValue > 10 ){
                    char position = lineReader.getLinePosition();
                    // Ajuster les moteurs en fonction de la position détectée
                    if (position == 'g') { // Ligne détectée à gauche
                    
                        adcValue = moyenneDist();
            
        
                        moteur.tournerGauche(140, 90); // Ajuste vers la gauche
                        
                    } else if (position == 'd') { // Ligne détectée à droite
                        adcValue = moyenneDist();
        
                        moteur.tournerDroite(90, 120); // Ajuste vers la droite

                    } else if (position == 'm') { // Ligne détectée au centre
                        adcValue = moyenneDist();
            
                        moteur.avancerMoteur(120, 110); // Avance tout droit
                    } 
                }
                moteur.arreterMoteur();
                for (int i = 0; i < 8; i++) {
                    sonnerie.runSong(64); 
                    _delay_ms(150);
                    sonnerie.stopSong();
                    _delay_ms(50);
                }  
                debut='C';
                mem.ecriture(100, 'C');
                mem.ecriture(101, '1');
                stop1 = true;
                firstRunDOne = true;
                mode = 0;// switch to mode 2
            }
        }
}

void part2() {

    int adcValue = 0;

    _delay_ms(2000);
        moteur.avancerMoteur(255,255);
        _delay_ms(100);
        while(lineReader.getSensorCount()!=0){
        char position = lineReader.getLinePosition();

        if(lineReader.getSensorCount() == 5){
                        mem.ecriture(101, '2');
                stop2 = true;
                 
                if(debut!='D'){
                    debut = 'C';
                    mem.ecriture(100,'C'); // correcte
                }

            }
            else if(lineReader.getSensorCount() == 3){
            mem.ecriture(101, '1');
            stop1 = true;
               
                if(debut!='C'){
                    debut = 'D';
                    mem.ecriture(100,'D');
                }
            }
             else if (position == 'g') { // Ligne détectée à gauche
                            
                        adcValue = moyenneDist();
                        moteur.tournerGauche(140, 90); // Ajuste vers la gauche
                                
                    } else if (position == 'd') { // Ligne détectée à droite
                        adcValue = moyenneDist();
                        moteur.tournerDroite(90, 120); // Ajuste vers la droite

                    } else if (position == 'm') { // Ligne détectée au centre
                        adcValue = moyenneDist();
                        moteur.avancerMoteur(120, 110); // Avance tout droit
                    } 
    }
    if(stop2){
        moteur.avancerPetiteLigne(lineReader);
        moteur.incrementalGauche();
        moteur.incrementalGauche();  
        moteur.incrementalGauche();  
        moteur.incrementalGauche(); 
        if(!lineReader.isCenter()){
            while(!lineReader.isCenter()){
                moteur.incrementalGauche();  
            }
        }

          moteur.avancerMoteur(255,255);
        while(lineReader.getSensorCount()!=5){
            char position = lineReader.getLinePosition();
            // Ajuster les moteurs en fonction de la position détectée
            if (position == 'g') { // Ligne détectée à gauche
                moteur.tournerGauche(140, 90); // Ajuste vers la gauche
            } else if (position == 'd') { // Ligne détectée à droite
                moteur.tournerDroite(90, 120); // Ajuste vers la droite
            } else if (position == 'm') { // Ligne détectée au centre
                moteur.avancerMoteur(120, 110); // Avance tout droit
            } 
        } 
        moteur.arreterMoteur();
        _delay_ms(1000);

           moteur.avancerMoteur(255,255); //possible problem
        _delay_ms(100);
        for(int i = 0; i<115;i++){
            char position = lineReader.getLinePosition();

            // Ajuster les moteurs en fonction de la position détectée
            if (position == 'g') { // Ligne détectée à gauche
                moteur.tournerGauche(140, 90); // Ajuste vers la gauche

            } else if (position == 'd') { // Ligne détectée à droite
                moteur.tournerDroite(90, 120); // Ajuste vers la droite

            } else if (position == 'm') { // Ligne détectée au centre
                moteur.avancerMoteur(120, 110); // Avance tout droit
            } 
            _delay_ms(15);
        }
        moteur.arreterMoteur();

        adcValue = moyenneDist();

        if(adcValue >=20 && adcValue <=40){
                while(adcValue > 10 ){
                    char position = lineReader.getLinePosition();
                    // Ajuster les moteurs en fonction de la position détectée
                    if (position == 'g') { // Ligne détectée à gauche
                            
                        adcValue = moyenneDist();
                        moteur.tournerGauche(140, 90); // Ajuste vers la gauche
                                
                    } else if (position == 'd') { // Ligne détectée à droite
                        adcValue = moyenneDist();
                        moteur.tournerDroite(90, 120); // Ajuste vers la droite

                    } else if (position == 'm') { // Ligne détectée au centre
                        adcValue = moyenneDist();
                        moteur.avancerMoteur(120, 110); // Avance tout droit
                    } 
                }
                moteur.arreterMoteur();
                for (int i = 0; i < 8; i++) {
                    sonnerie.runSong(64); 
                    _delay_ms(150);
                    sonnerie.stopSong();
                    _delay_ms(50);
                }  
                mem.ecriture(102, '3');
                stop3 = true;
                secondRunDone = true;
                mode = 0;
                        }
        else {
            moteur.incrementalDroite();
            moteur.incrementalDroite();
            moteur.incrementalDroite();
            if(!lineReader.isCenter()){
                while(!lineReader.isCenter()){
                    moteur.incrementalDroite();
                }
            }
            adcValue = moyenneDist();
                while(adcValue > 10 ){
                    char position = lineReader.getLinePosition();
                    // Ajuster les moteurs en fonction de la position détectée
                    if (position == 'g') { // Ligne détectée à gauche
                            
                        adcValue = moyenneDist();
                        moteur.tournerGauche(140, 90); // Ajuste vers la gauche
                                
                    } else if (position == 'd') { // Ligne détectée à droite
                        adcValue = moyenneDist();
                        moteur.tournerDroite(90, 120); // Ajuste vers la droite

                    } else if (position == 'm') { // Ligne détectée au centre
                        adcValue = moyenneDist();
                        moteur.avancerMoteur(120, 110); // Avance tout droit
                    } 
                }
                moteur.arreterMoteur();
                for (int i = 0; i < 8; i++) {
                    sonnerie.runSong(64); 
                    _delay_ms(150);
                    sonnerie.stopSong();
                    _delay_ms(50);
                }  
                mem.ecriture(102, '4');
                stop4 = true;
                secondRunDone = true;
                mode = 0;
            }
    } else {
         moteur.incrementalDroite();
        moteur.incrementalDroite();
        moteur.incrementalDroite();
        moteur.incrementalDroite();
        if(!lineReader.isCenter()){
            while(!lineReader.isCenter()){
                moteur.incrementalDroite();
            }
        }

         moteur.avancerMoteur(255,255);
        while(lineReader.getSensorCount()!=5){
            char position = lineReader.getLinePosition();
            // Ajuster les moteurs en fonction de la position détectée
            if (position == 'g') { // Ligne détectée à gauche
                moteur.tournerGauche(140, 90); // Ajuste vers la gauche
            } else if (position == 'd') { // Ligne détectée à droite
                moteur.tournerDroite(90, 120); // Ajuste vers la droite
            } else if (position == 'm') { // Ligne détectée au centre
                moteur.avancerMoteur(120, 110); // Avance tout droit
            } 
        }  

         moteur.arreterMoteur();
        _delay_ms(1000);
        moteur.avancerMoteur(255,255); //possible problem
        _delay_ms(100);
        for(int i = 0; i<115;i++){
            char position = lineReader.getLinePosition();

            // Ajuster les moteurs en fonction de la position détectée
            if (position == 'g') { // Ligne détectée à gauche
                moteur.tournerGauche(140, 90); // Ajuste vers la gauche

            } else if (position == 'd') { // Ligne détectée à droite
                moteur.tournerDroite(90, 120); // Ajuste vers la droite

            } else if (position == 'm') { // Ligne détectée au centre
                moteur.avancerMoteur(120, 110); // Avance tout droit
            } 
            _delay_ms(15);
        }
        moteur.arreterMoteur();

         adcValue = moyenneDist();

        if(adcValue >=15 && adcValue <=40){
                while(adcValue > 10 ){
                    char position = lineReader.getLinePosition();
                    // Ajuster les moteurs en fonction de la position détectée
                    if (position == 'g') { // Ligne détectée à gauche
                            
                        adcValue = moyenneDist();
                        moteur.tournerGauche(140, 90); // Ajuste vers la gauche
                                
                    } else if (position == 'd') { // Ligne détectée à droite
                        adcValue = moyenneDist();
                        moteur.tournerDroite(90, 120); // Ajuste vers la droite

                    } else if (position == 'm') { // Ligne détectée au centre
                        adcValue = moyenneDist();
                        moteur.avancerMoteur(120, 110); // Avance tout droit
                    } 
                }
                moteur.arreterMoteur();
                for (int i = 0; i < 8; i++) {
                    sonnerie.runSong(64); 
                    _delay_ms(150);
                    sonnerie.stopSong();
                    _delay_ms(50);
                }  
                mem.ecriture(102, '4');
                stop4 = true;
                secondRunDone = true;
                mode = 0;
            }
    else {
                moteur.incrementalGauche();
                moteur.incrementalGauche();  
                moteur.incrementalGauche();  
                moteur.incrementalGauche(); 
                if(!lineReader.isCenter()){
                    while(!lineReader.isCenter()){
                        moteur.incrementalGauche();  
                    }
                }
                 adcValue = moyenneDist();
                while(adcValue > 10 ){
                    char position = lineReader.getLinePosition();
                    // Ajuster les moteurs en fonction de la position détectée
                    if (position == 'g') { // Ligne détectée à gauche
                            
                        adcValue = moyenneDist();
                        moteur.tournerGauche(140, 90); // Ajuste vers la gauche
                                
                    } else if (position == 'd') { // Ligne détectée à droite
                        adcValue = moyenneDist();
                        moteur.tournerDroite(90, 120); // Ajuste vers la droite

                    } else if (position == 'm') { // Ligne détectée au centre
                        adcValue = moyenneDist();
                        moteur.avancerMoteur(120, 110); // Avance tout droit
                    } 
                }
                moteur.arreterMoteur();
                for (int i = 0; i < 8; i++) {
                    sonnerie.runSong(64); 
                    _delay_ms(150);
                    sonnerie.stopSong();
                    _delay_ms(50);
                }  
                mem.ecriture(102, '3');
                stop3 = true;
                secondRunDone = true;
                mode = 0;
            }

    }







}

void part3() {
int adcValue = 0;

    _delay_ms(2000);
        moteur.avancerMoteur(255,255);
        _delay_ms(100);
        while(lineReader.getSensorCount()!=0){
        char position = lineReader.getLinePosition();


              if (position == 'g') { // Ligne détectée à gauche
                            
                        adcValue = moyenneDist();
                        moteur.tournerGauche(140, 90); // Ajuste vers la gauche
                                
                    } else if (position == 'd') { // Ligne détectée à droite
                        adcValue = moyenneDist();
                        moteur.tournerDroite(90, 120); // Ajuste vers la droite

                    } else if (position == 'm') { // Ligne détectée au centre
                        adcValue = moyenneDist();
                        moteur.avancerMoteur(120, 110); // Avance tout droit
                    } 
    }
    if(stop3){
        moteur.avancerPetiteLigne(lineReader);
        moteur.tournerAngleDroite(lineReader);
        while(!lineReader.isCenter()){
            moteur.incrementalDroite();
        }

           while(lineReader.getSensorCount()!=5){
        char position = lineReader.getLinePosition();


              if (position == 'g') { // Ligne détectée à gauche
                            
                        adcValue = moyenneDist();
                        moteur.tournerGauche(140, 90); // Ajuste vers la gauche
                                
                    } else if (position == 'd') { // Ligne détectée à droite
                        adcValue = moyenneDist();
                        moteur.tournerDroite(90, 120); // Ajuste vers la droite

                    } else if (position == 'm') { // Ligne détectée au centre
                        adcValue = moyenneDist();
                        moteur.avancerMoteur(120, 110); // Avance tout droit
                    } 
    }
       moteur.arreterMoteur();
        _delay_ms(1000);
        moteur.avancerMoteur(255,255); //possible problem
        _delay_ms(100);
        for(int i = 0; i<115;i++){
            char position = lineReader.getLinePosition();

            // Ajuster les moteurs en fonction de la position détectée
            if (position == 'g') { // Ligne détectée à gauche
                moteur.tournerGauche(140, 90); // Ajuste vers la gauche

            } else if (position == 'd') { // Ligne détectée à droite
                moteur.tournerDroite(90, 120); // Ajuste vers la droite

            } else if (position == 'm') { // Ligne détectée au centre
                moteur.avancerMoteur(120, 110); // Avance tout droit
            } 
            _delay_ms(15);
        }
        moteur.arreterMoteur();
        adcValue = moyenneDist();
             if(adcValue >=15 && adcValue <=35){
                while(adcValue > 10 ){
                    char position = lineReader.getLinePosition();
                    // Ajuster les moteurs en fonction de la position détectée
                    if (position == 'g') { // Ligne détectée à gauche
                            
                        adcValue = moyenneDist();
                        moteur.tournerGauche(140, 90); // Ajuste vers la gauche
                                
                    } else if (position == 'd') { // Ligne détectée à droite
                        adcValue = moyenneDist();
                        moteur.tournerDroite(90, 120); // Ajuste vers la droite

                    } else if (position == 'm') { // Ligne détectée au centre
                        adcValue = moyenneDist();
                        moteur.avancerMoteur(120, 110); // Avance tout droit
                    } 
                }
                moteur.arreterMoteur();
                for (int i = 0; i < 8; i++) {
                    sonnerie.runSong(64); 
                    _delay_ms(150);
                    sonnerie.stopSong();
                    _delay_ms(50);
                }  
                mem.ecriture(103, '6'); ///
                stop6 = true;
                thirdRunDone = true;
                mode = 0;
            }
            else{
                moteur.incrementalGauche();
                moteur.incrementalGauche();
                moteur.incrementalGauche();
                 moteur.incrementalGauche();
                //moteur.tournerAngleGauche(lineReader);
                while(!lineReader.isCenter()){
                    moteur.incrementalGauche();
                }
                adcValue = moyenneDist();
             
                while(adcValue > 10 ){
                    char position = lineReader.getLinePosition();
                    // Ajuster les moteurs en fonction de la position détectée
                    if (position == 'g') { // Ligne détectée à gauche
                            
                        adcValue = moyenneDist();
                        moteur.tournerGauche(140, 90); // Ajuste vers la gauche
                                
                    } else if (position == 'd') { // Ligne détectée à droite
                        adcValue = moyenneDist();
                        moteur.tournerDroite(90, 120); // Ajuste vers la droite

                    } else if (position == 'm') { // Ligne détectée au centre
                        adcValue = moyenneDist();
                        moteur.avancerMoteur(120, 110); // Avance tout droit
                    } 
                }
                moteur.arreterMoteur();
                for (int i = 0; i < 8; i++) {
                    sonnerie.runSong(64); 
                    _delay_ms(150);
                    sonnerie.stopSong();
                    _delay_ms(50);
                }  
                mem.ecriture(103, '5');
                stop5 = true;
                thirdRunDone = true;
                mode = 0;
            

            }
    }

    else{
        moteur.avancerPetiteLigne(lineReader);
        moteur.incrementalGauche();
        while(!lineReader.isCenter()){
            moteur.incrementalGauche();
        }

             while(lineReader.getSensorCount()!=5){
        char position = lineReader.getLinePosition();


              if (position == 'g') { // Ligne détectée à gauche
                            
                        adcValue = moyenneDist();
                        moteur.tournerGauche(140, 90); // Ajuste vers la gauche
                                
                    } else if (position == 'd') { // Ligne détectée à droite
                        adcValue = moyenneDist();
                        moteur.tournerDroite(90, 120); // Ajuste vers la droite

                    } else if (position == 'm') { // Ligne détectée au centre
                        adcValue = moyenneDist();
                        moteur.avancerMoteur(120, 110); // Avance tout droit
                    } 
    }
       moteur.arreterMoteur();
       _delay_ms(1000);

        moteur.avancerMoteur(255,255); //possible problem
        _delay_ms(100);
        for(int i = 0; i<115;i++){
            char position = lineReader.getLinePosition();

            // Ajuster les moteurs en fonction de la position détectée
            if (position == 'g') { // Ligne détectée à gauche
                moteur.tournerGauche(140, 90); // Ajuste vers la gauche

            } else if (position == 'd') { // Ligne détectée à droite
                moteur.tournerDroite(90, 120); // Ajuste vers la droite

            } else if (position == 'm') { // Ligne détectée au centre
                moteur.avancerMoteur(120, 110); // Avance tout droit
            } 
            _delay_ms(15);
        }
        moteur.arreterMoteur();
        _delay_ms(500);
        moteur.avancerMoteur(255,255);
        _delay_ms(100);
        adcValue = moyenneDist();
             if(adcValue >=15 && adcValue <=40){
                while(adcValue > 10 ){
                    char position = lineReader.getLinePosition();
                    // Ajuster les moteurs en fonction de la position détectée
                    if (position == 'g') { // Ligne détectée à gauche
                            
                        adcValue = moyenneDist();
                        moteur.tournerGauche(140, 90); // Ajuste vers la gauche
                                
                    } else if (position == 'd') { // Ligne détectée à droite
                        adcValue = moyenneDist();
                        moteur.tournerDroite(90, 120); // Ajuste vers la droite

                    } else if (position == 'm') { // Ligne détectée au centre
                        adcValue = moyenneDist();
                        moteur.avancerMoteur(120, 110); // Avance tout droit
                    } 
                }
                moteur.arreterMoteur();
                for (int i = 0; i < 8; i++) {
                    sonnerie.runSong(64); 
                    _delay_ms(150);
                    sonnerie.stopSong();
                    _delay_ms(50);
                }  
                mem.ecriture(103, '5');
                stop5 = true;
                thirdRunDone = true;
                mode = 0;
            }
            else{
                moteur.incrementalDroite();
                while(!lineReader.isCenter()){
                    moteur.incrementalDroite();
                }
            }
            moteur.arreterMoteur();
            _delay_ms(500);
            adcValue = moyenneDist();
                while(adcValue > 10 ){
                    char position = lineReader.getLinePosition();
                    // Ajuster les moteurs en fonction de la position détectée
                    if (position == 'g') { // Ligne détectée à gauche
                            
                        adcValue = moyenneDist();
                        moteur.tournerGauche(140, 90); // Ajuste vers la gauche
                                
                    } else if (position == 'd') { // Ligne détectée à droite
                        adcValue = moyenneDist();
                        moteur.tournerDroite(90, 120); // Ajuste vers la droite

                    } else if (position == 'm') { // Ligne détectée au centre
                        adcValue = moyenneDist();
                        moteur.avancerMoteur(120, 110); // Avance tout droit
                    } 
                }
                moteur.arreterMoteur();
                for (int i = 0; i < 8; i++) {
                    sonnerie.runSong(64); 
                    _delay_ms(150);
                    sonnerie.stopSong();
                    _delay_ms(50);
                }  
                mem.ecriture(103, '6');
                stop6 = true;
                thirdRunDone = true;
                mode = 0;
            

    }








}

void part4(){
    
    if(stop5){
        moteur.avancerMoteur(255,255);
        _delay_ms(100);
                   while(lineReader.getSensorCount()!=0){
        char position = lineReader.getLinePosition();


              if (position == 'g') { // Ligne détectée à gauche
                            
                      //  adcValue = moyenneDist();
                        moteur.tournerGauche(140, 90); // Ajuste vers la gauche
                                
                    } else if (position == 'd') { // Ligne détectée à droite
                       // adcValue = moyenneDist();
                        moteur.tournerDroite(90, 120); // Ajuste vers la droite

                    } else if (position == 'm') { // Ligne détectée au centre
                      //  adcValue = moyenneDist();
                        moteur.avancerMoteur(120, 110); // Avance tout droit
                    } 
            
    }
        moteur.avancerMoteur(255,255);
        _delay_ms(30);
        moteur.avancerPetiteLigne(lineReader);
        moteur.tournerAngleDroite(lineReader);

        while(!lineReader.isCenter()){
            moteur.incrementalDroite();
        }

             while(lineReader.getSensorCount()!=0){
        char position = lineReader.getLinePosition();


              if (position == 'g') { // Ligne détectée à gauche
                            
                     //  adcValue = moyenneDist();
                        moteur.tournerGauche(140, 90); // Ajuste vers la gauche
                                
                    } else if (position == 'd') { // Ligne détectée à droite
                       // adcValue = moyenneDist();
                        moteur.tournerDroite(90, 120); // Ajuste vers la droite

                    } else if (position == 'm') { // Ligne détectée au centre
                      //  adcValue = moyenneDist();
                        moteur.avancerMoteur(120, 110); // Avance tout droit
                    } 
            
    }
      moteur.arreterMoteur();
    _delay_ms(200);
    for (int i = 0; i < 2; i++) {
                    sonnerie.runSong(1); 
                    _delay_ms(750);
                    sonnerie.stopSong();
                    _delay_ms(50);
                }  

    mem.ecriture(0, '3');
    mode = 5;
    }
    else{
                  while(lineReader.getSensorCount()!=0){
        char position = lineReader.getLinePosition();


              if (position == 'g') { // Ligne détectée à gauche
                            
                        //adcValue = moyenneDist();
                        moteur.tournerGauche(140, 90); // Ajuste vers la gauche
                                
                    } else if (position == 'd') { // Ligne détectée à droite
                       // adcValue = moyenneDist();
                        moteur.tournerDroite(90, 120); // Ajuste vers la droite

                    } else if (position == 'm') { // Ligne détectée au centre
                       // adcValue = moyenneDist();
                        moteur.avancerMoteur(120, 110); // Avance tout droit
                    } 
    }
    moteur.incrementalGauche();
    while(!lineReader.isCenter()){
        moteur.incrementalGauche();
    }

                   while(lineReader.getSensorCount()!=0){
        char position = lineReader.getLinePosition();


              if (position == 'g') { // Ligne détectée à gauche
                            
                        //adcValue = moyenneDist();
                        moteur.tournerGauche(140, 90); // Ajuste vers la gauche
                                
                    } else if (position == 'd') { // Ligne détectée à droite
                       // adcValue = moyenneDist();
                        moteur.tournerDroite(90, 120); // Ajuste vers la droite

                    } else if (position == 'm') { // Ligne détectée au centre
                        //adcValue = moyenneDist();
                        moteur.avancerMoteur(120, 110); // Avance tout droit
                    } 
    }

    moteur.incrementalDroite();
    while(!lineReader.isCenter()){
        moteur.incrementalDroite();
    }

                   while(lineReader.getSensorCount()!=0){
        char position = lineReader.getLinePosition();


              if (position == 'g') { // Ligne détectée à gauche
                            
                       // adcValue = moyenneDist();
                        moteur.tournerGauche(140, 90); // Ajuste vers la gauche
                                
                    } else if (position == 'd') { // Ligne détectée à droite
                       // adcValue = moyenneDist();
                        moteur.tournerDroite(90, 120); // Ajuste vers la droite

                    } else if (position == 'm') { // Ligne détectée au centre
                       // adcValue = moyenneDist();
                        moteur.avancerMoteur(120, 110); // Avance tout droit
                    } 
    }
    moteur.arreterMoteur();
    _delay_ms(200);
    for (int i = 0; i < 2; i++) {
                    sonnerie.runSong(1); 
                    _delay_ms(750);
                    sonnerie.stopSong();
                    _delay_ms(50);
                }  

    }
    mem.ecriture(0, '3');
    mode = 5;

}

void part5(){
    moteur.arreterMoteur();
    _delay_ms(5000);
}

void TraverserParcours() {
    t.initialisationUART();
    DDRD &= ~(1 << PA2);
    sonnerie.stopSong();
    Del led(PA0, PA1);
    led.initialiser(&DDRA, &PORTA);
     for(int i =0 ; i<8;i++ ){
        led.rouge(); 
        _delay_ms(150);
        led.eteint();
        _delay_ms(100);
     }
    while (1) {
        switch (mode) {
            case 0:
                part0();
                break;
            case 1:
                part1();
                break;
            case 2:
                part2();
                break;
            case 3:
                part3();
                break;
            case 4:
                part4();
                break;
            case 5:
                part5();
                break;
            default:
                break;
        }
    }
}