// #pragma once
#define F_CPU 8000000
#include <avr/io.h>
#include <util/delay.h>
#include "memoire_24.h" 
#include "Transmission.h"
using namespace std;


void lireDeMemoire(char* chaineLue, uint16_t taille,uint8_t addresseDebut) {
    Memoire24CXXX mem;
    for (uint16_t i = 0; i < taille ; i++) {
        mem.lecture(addresseDebut + i, (uint8_t*)&chaineLue[i]);
        _delay_ms(5); 
    }
}

void afficherExtremite(char* donnee,Transmission transmissionUART){
    transmissionUART.transmissionUART( '\n' );
    char mots[35] = "Identification de l’extrémité\n";
    
    uint8_t j;
    for ( j=0; j < 34; j++ ) {
        transmissionUART.transmissionUART( mots[j] );
    }
    char motss[31] = "-----------------------------\n";
    

    for ( j=0; j < 30; j++ ) {
        transmissionUART.transmissionUART( motss[j] );
    }


    char motsss[20] = "Point de départ : ";
  
    for ( j=0; j < 19; j++ ) {
        transmissionUART.transmissionUART( motsss[j] );
    }
     transmissionUART.transmissionUART( donnee[0] );

     char motssss[27] = "\nOrientation de départ : ";
   

    for ( j=0; j < 26; j++ ) {
        transmissionUART.transmissionUART( motssss[j] );
    }

     transmissionUART.transmissionUART( donnee[2] );
     transmissionUART.transmissionUART( donnee[3] );

      char motsssss[25] = "\nExtrémité trouvée : ";



    for ( j=0; j < 24; j++ ) {
        transmissionUART.transmissionUART( motsssss[j] );
    }
    
     transmissionUART.transmissionUART( donnee[4] );
     transmissionUART.transmissionUART( '\n' );
     transmissionUART.transmissionUART( '\n' );

}

void afficherParcours(char* donnee, Transmission transmissionUART){
    
      char mots[24] = "Traversée du parcours\n";
  
    uint8_t j;
    for ( j=0; j < 23; j++ ) {
        transmissionUART.transmissionUART( mots[j] );
    }

     char motss[23] = "---------------------\n";

    for ( j=0; j < 22; j++ ) {
        transmissionUART.transmissionUART( motss[j] );
    }

     char motsss[20] = "Point de départ : ";


    for ( j=0; j < 19; j++ ) {
        transmissionUART.transmissionUART( motsss[j] );
    }
     transmissionUART.transmissionUART( donnee[0] );

     char motssss[22] = "\nPoint de poteau 1 : ";

    for ( j=0; j < 21; j++ ) {
        transmissionUART.transmissionUART (motssss[j] );
    }
    
     transmissionUART.transmissionUART( donnee[1] );

    char motsssss[22] = "\nPoint de poteau 2 : ";



    for ( j=0; j < 21; j++ ) {
        transmissionUART.transmissionUART( motsssss[j] );
    }
    
     transmissionUART.transmissionUART( donnee[2] );

        char motssssss[22] = "\nPoint de poteau 3 : ";


    for ( j=0; j < 21; j++ ) {
        transmissionUART.transmissionUART( motssssss[j] );
    }

     transmissionUART.transmissionUART( donnee[3] );

}


void afficherEquipe(Transmission transmissionUART){
    char equipe[14] ="3137 - Franky";

    transmissionUART.transmissionUART( '\n' );
    transmissionUART.transmissionUART( '\n' );

    for(int i = 0; i<13;i++){
        transmissionUART.transmissionUART(equipe[i]);
    }
}

void afficherRapport(){
    Transmission usart = Transmission();
    usart.initialisationUART();
    Memoire24CXXX mem = Memoire24CXXX();


    char donneeTrouverExtremite[6];
    mem.lecture(56, (uint8_t *)donneeTrouverExtremite, 5);
   // lireDeMemoire(donneeTrouverExtremite,5,56);



    char donneeTraverserParcours[5];
    //lireDeMemoire(donneeTraverserParcours,4,100);
    mem.lecture(100, (uint8_t *)donneeTraverserParcours, 4);

    afficherExtremite(donneeTrouverExtremite,usart);
    afficherParcours(donneeTraverserParcours,usart);
    _delay_ms(50);
    afficherEquipe(usart);


    mem.ecriture(0,'J');


}


