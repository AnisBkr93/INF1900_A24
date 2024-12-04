#define F_CPU 8000000
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "Del.h"
#include "memoire_24.h" 
#include "Chercher.h"
#include "TRaverse.h"
#include "affciherRapport.h"
#include "Sonnerie.h"


/*
Auteurs: Anis Boukerdoune, Emmanuel Berberi, Buntep Chea, Fadi Mousa
Date: 2024-12-03
Description: Programme principal pour contrôler un robot avec trois modes:
Recherche d'extrémité (CHERCHE_EXTREMITER).
Traversée du parcours (TRAVERSE_PARCOURS).
Affichage du rapport (AFFICHE_RAPPORT).
Le mode courant est sauvegardé et lu dans l'EEPROM pour une reprise ultérieure.
Identifications matérielles: LED (PA0, PA1), EEPROM, Sonnerie.
*/
const uint16_t EEPROM_MODE_ADRESSE = 0x00; //adresse ou on sauvegarde le current mode dans EEPROM

enum Mode {
    CHERCHE_EXTREMITER = 0,
    TRAVERSE_PARCOURS = 1,
    AFFICHE_RAPPORT =2
};

int main(){
    Del led(PA0, PA1);
    Son sono;
    sono.stopSong();
    led.initialiser(&DDRA, &PORTA); //initialiser(volatile uint8_t* ddrPort, volatile uint8_t* port) 

    Memoire24CXXX eeprom;

    //lire derniere valeur sauvegarder de EEPROM
    uint8_t savedMode = 0;
    eeprom.lecture(EEPROM_MODE_ADRESSE, &savedMode);
    
    
        if((uint8_t)savedMode ==0){
            chercherExtremite();
        }
        else if((uint8_t)savedMode == 1){
            TraverserParcours();
        }
        else if ((uint8_t)savedMode == 2){
            _delay_ms(5000);
            afficherRapport();
        }
        else{
            Mode currentMode = Mode::CHERCHE_EXTREMITER; //si on a saved mode = 5 CAD on est dans mode 2
    
        while (true) {
            switch (currentMode) {
                case CHERCHE_EXTREMITER: 
                        eeprom.ecriture(EEPROM_MODE_ADRESSE, (uint8_t)currentMode);
                        led.vert(); 
                        _delay_ms(2000);
                            led.eteint();
                        _delay_ms(100);
                        currentMode = Mode((currentMode + 1) % 3);
                        break;
                
                case TRAVERSE_PARCOURS: 
                        eeprom.ecriture(EEPROM_MODE_ADRESSE, (uint8_t)currentMode);
                        led.rouge(); 
                        _delay_ms(2000); 
                            led.eteint();
                        _delay_ms(100);
                        currentMode = Mode((currentMode + 1) % 3);
                        break;
                
                case AFFICHE_RAPPORT: 
                eeprom.ecriture(EEPROM_MODE_ADRESSE, (uint8_t)currentMode);
                for(int i = 0;i<100;i++){
                    led.ambre();
                } 
                    led.eteint();
                _delay_ms(100);
                currentMode = Mode((currentMode + 1) % 3);
                break;
                
            }

            
            

        }
    }   

    //commence la boucle
}