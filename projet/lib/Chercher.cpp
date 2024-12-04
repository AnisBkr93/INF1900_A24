#define F_CPU 8000000
#include <avr/io.h>
#include <util/delay.h>
#include "Moteur.h"
#include "MakerLineRead.h"
#include "Del.h"
#include "Transmission.h"
#include "memoire_24.h"
#include "Sonnerie.h"

void getArretPossible(bool& leftDeadEnd, bool& rightDeadEnd, bool& middleDeadEnd,char* arretPossible ){ //fonction utilisé après la premiere itération de l'algo
     char arretCombo1[] = "A12DA43ENO";                                             // pour trouvé  pour trouvé l'arret possiblement prit au debut
     char arretCombo2[] = "B52EA33ENE";       
     char arretCombo3[] = "A22CB43DSO";
     char arretCombo4[] = "B62EB33CSE";
    

    bool leftCopy = leftDeadEnd;
    bool rightCopy = rightDeadEnd;
    bool middleCopy = middleDeadEnd;

    

    if(leftCopy && !rightCopy && !middleCopy){
      for(int i = 0; i<sizeof(arretCombo2);i++){
        arretPossible[i] = arretCombo2[i];
      }
    }

    if(!leftCopy && rightCopy  && !middleCopy){
        for(int i = 0; i<sizeof(arretCombo2);i++){
          arretPossible[i] = arretCombo1[i];
        }
    }

    if(leftCopy && !rightCopy && middleCopy){
        for(int i = 0; i<sizeof(arretCombo2);i++){
          arretPossible[i] = arretCombo3[i];
        }
    }

    if(!leftCopy && rightCopy && middleCopy){
        for(int i = 0; i<sizeof(arretCombo2);i++){
          arretPossible[i] = arretCombo4[i];
        }
    }
}


void verificationFin(bool& leftDeadEnd,bool& rightDeadEnd,bool& middleDeadEnd,bool& sortieFound, Moteur robot,MakerLineRead lineReader, uint8_t& nbLoop, bool& middleLeft, bool& middleRight , char* arretPossibleDebut,  bool& detectedFive){ 

  
    if(nbLoop +1 == 3){
        nbLoop++;
        robot.arreterMoteur();
        sortieFound = true;
        
    }

    else if(detectedFive){
     nbLoop++;
         robot.avancerMoteur(255,205);
        _delay_ms(800);
        robot.arreterMoteur();
       _delay_ms(760);
       leftDeadEnd = robot.tournerAngleGauche3(lineReader);
    
        
  
          if(lineReader.getSensorCount()==0){
      uint8_t timer = 0;
      while(!lineReader.isCenter() ){
        robot.incrementalGauche();
        timer++;
        if(timer == 3){
          break;
        }
      }
      leftDeadEnd = lineReader.getSensorCount()==0;
      
    }
    
    if(nbLoop ==1){
        if(leftDeadEnd){
            getArretPossible(leftDeadEnd,rightDeadEnd,middleDeadEnd,arretPossibleDebut);
            robot.tournerAngleDroite(lineReader);
        while(!lineReader.isCenter()){
            robot.incrementalDroite();
        }
        }

        else{
            rightDeadEnd = true;
            getArretPossible(leftDeadEnd,rightDeadEnd,middleDeadEnd,arretPossibleDebut);
        }
    }


     
       else if(nbLoop!=1)     {
        robot.tournerAngleDroite(lineReader);
            rightDeadEnd = robot.tournerAngleDroite(lineReader);
            if(lineReader.getSensorCount()==0){
                uint8_t timer = 0;
                while(!lineReader.isCenter()){
                    robot.incrementalDroite();
                    timer++;
                        if(timer==7){
                            break;
                        }
                }
            }
            rightDeadEnd = lineReader.getSensorCount()==0;

            if(leftDeadEnd && !rightDeadEnd){}

            if(!leftDeadEnd && rightDeadEnd){
                robot.tournerAngleGauche3(lineReader);
                while(!lineReader.isCenter()){
                        robot.incrementalGauche();
                }
               
            }


       }
           
    }
    else{
        if(middleLeft){
            rightDeadEnd = true;
            while(!lineReader.isCenter()){
                robot.incrementalGauche();
            }
            if(nbLoop==0){ 
                getArretPossible(leftDeadEnd,rightDeadEnd,middleDeadEnd,arretPossibleDebut);
                
            }
            nbLoop++;
        }
        else{
            leftDeadEnd = true;
            while(!lineReader.isCenter()){
                robot.incrementalDroite();
            }
            if(nbLoop==0){ 
                getArretPossible(leftDeadEnd,rightDeadEnd,middleDeadEnd,arretPossibleDebut);
            }
            nbLoop++;
        }
    }

    

    if(leftDeadEnd && rightDeadEnd && !middleDeadEnd){
        robot.tournerAngleGauche3(lineReader);
        
            robot.reculerMoteur(255,205);
       _delay_ms(800);
        robot.arreterMoteur();
        sortieFound = true;
    }
}

void determinerExactLocation(uint8_t nbLoop, char* possibleSegments,char* arretExact){ //fonction utiliser a la fin de l'algo pour determiné la position
                                                                                      //initiale, l'orientation et la destination                                                                        
    if(2 == nbLoop){
        arretExact[0] = possibleSegments[0];
        arretExact[1] = possibleSegments[1];
        arretExact[2] = possibleSegments[8];
        arretExact[3] = possibleSegments[9];
        arretExact[4] = possibleSegments[3];
        arretExact[5] =  '\0';
    }
    else{
        arretExact[0] = possibleSegments[4];
        arretExact[1] = possibleSegments[5];
        arretExact[2] = possibleSegments[8];
        arretExact[3] = possibleSegments[9];
        arretExact[4] = possibleSegments[7];
        arretExact[5] = '\0';

        if(arretExact[0] =='B' && arretExact[2] == '3'){
            arretExact[2] = 'N';
            arretExact[3] = 'O';
        }
    }
}

void mouvementSortieE(Moteur moteur, MakerLineRead lineReader){

  moteur.tournerAngleDroite(lineReader);
  if(lineReader.getSensorCount()==0){
    while(!lineReader.isCenter()){
      moteur.incrementalDroite();
    }
  }
    while(lineReader.getSensorCount()!=5){
        char position = lineReader.getLinePosition();

            // Ajuster les moteurs en fonction de la position détectée
        if (position == 'g') { // Ligne détectée à gauche
             moteur.tournerGauche(130, 90); // Ajuste vers la gauche
        } else if (position == 'd') { // Ligne détectée à droite
            moteur.tournerDroite(90, 120); // Ajuste vers la droite
        } else if (position == 'm') { // Ligne détectée au centre
            moteur.avancerMoteur(120, 110); // Avance tout droit
        } else if (position == 'n') { // Aucune ligne détectée
            moteur.arreterMoteur(); // Stop pour éviter les erreurs
        }
    }
    moteur.arreterMoteur();
}




int chercherExtremite(){    
    char arretPossibleDebut[11];
    char arretDebut[6];

    bool detectedFive = false;
   
    bool middleLeft = false;
    bool middleRight = false;

    bool sortieFound = false;
    
    MakerLineRead lineReader;
    Moteur moteur;
    
    bool noLine = false;
    bool detectedIntersectionOnce = false;
    bool ligneFiniE = false;
    bool sortieE = false;
    
    Del led(PA0, PA1);
    led.initialiser(&DDRA, &PORTA);
    
    uint8_t nbIteration = 0;
    bool leftDeadEnd = false;
    bool rightDeadEnd = false;
    bool middleDeadEnd = false;
    
    Transmission transmitter;
    transmitter.initialisationUART();
    
    Memoire24CXXX mem;
    
    uint8_t resetMode = 3;

    for(int i =0 ; i<8;i++ ){
        led.vert(); 
        _delay_ms(150);
        led.eteint();
        _delay_ms(100);
    }
  

    while(!sortieFound){

 
        while(!noLine){
              if(nbIteration == 0){
                detectedIntersectionOnce = true;
              }
             if(lineReader.detectIntersection()){
                if(detectedIntersectionOnce){
                }
                else{
                    _delay_ms(100);
                    for(int i = 0; i<115;i++){
                      char position = lineReader.getLinePosition();
        
            // Ajuster les moteurs en fonction de la position détectée
           if (position == 'g') { // Ligne détectée à gauche
                  moteur.tournerGauche(140, 90); // Ajuste vers la gauche
                  ligneFiniE = ligneFiniE | (lineReader.getSensorCount()==0);
            } else if (position == 'd') { // Ligne détectée à droite

                moteur.tournerDroite(90, 120); // Ajuste vers la droite
                ligneFiniE = ligneFiniE | (lineReader.getSensorCount()==0);
            } else if (position == 'm') { // Ligne détectée au centre
                moteur.avancerMoteur(120, 110); // Avance tout droit
                ligneFiniE = ligneFiniE | (lineReader.getSensorCount()==0);
            } else if (position == 'n') { // Aucune ligne détectée
                moteur.arreterMoteur(); // Stop pour éviter les erreurs
                ligneFiniE = true;
            }
              _delay_ms(30);
                     
                }
                    detectedIntersectionOnce = true;
                }
               
            }
            
            if(ligneFiniE){
                sortieE = true;
                nbIteration++;
                sortieFound = true;
                noLine=true;
                //rouler = true;
                break;
            }
            
            char position = lineReader.getLinePosition();
             if(lineReader.getSensorCount()==5 || lineReader.isReallyCenter()){
                if(detectedIntersectionOnce){
                      detectedFive = true;
                    middleDeadEnd = false;
                  middleLeft =  false;
                 middleRight =  false;
                    noLine = true;
                }
             }
           else if(lineReader.getSensorCount()==3){

            middleLeft =  lineReader.isLeftEdge();
            middleRight =  lineReader.isRightEdge();
            if(middleLeft || middleRight){
                middleDeadEnd = true;
            }
          }
            // Ajuster les moteurs en fonction de la position détectée
          else if (position == 'g') { // Ligne détectée à gauche
                  moteur.tournerGauche(140, 90); // Ajuste vers la gauche
            } else if (position == 'd') { // Ligne détectée à droite
                moteur.tournerDroite(90, 120); // Ajuste vers la droite
            } else if (position == 'm') { // Ligne détectée au centre
                moteur.avancerMoteur(110, 100); // Avance tout droit
               }
            else if (position == 'n' ) { // Aucune ligne détectée
                moteur.arreterMoteur(); // Stop pour éviter les erreurs
                noLine = true;
        }
        }
        if(ligneFiniE){
          break;
        }
        
        verificationFin(leftDeadEnd, rightDeadEnd, middleDeadEnd, sortieFound, moteur, lineReader,nbIteration,middleLeft,middleRight,arretPossibleDebut , detectedFive);


        detectedFive = false;
        detectedIntersectionOnce = false;
        noLine = false;
        leftDeadEnd = false;
        rightDeadEnd = false;
        middleDeadEnd = false;
        middleLeft = false;
        middleRight = false;
    }

    if(sortieE){
      mouvementSortieE(moteur,lineReader);
    } 
    moteur.arreterMoteur();
    determinerExactLocation(nbIteration,arretPossibleDebut,arretDebut);


    mem.ecriture(56, (uint8_t *)arretDebut, sizeof(arretDebut)-1);

    
    if (arretDebut[4] == 'C'){
             Son sono;
   sono.runSong(64);
   _delay_ms(750);
   sono.stopSong();
   _delay_ms(50);
   sono.runSong(64); //change to 1 for traverserParcours
   _delay_ms(750);
     sono.stopSong();
   _delay_ms(50);
   mem.ecriture(0, 'g');
        led.vert();
    }

    if (arretDebut[4] == 'D'){
           Son sono;
   sono.runSong(64);
   _delay_ms(750);
   sono.stopSong();
   _delay_ms(50);
   sono.runSong(64); //change to 1 for traverserParcours
   _delay_ms(750);
     sono.stopSong();
   _delay_ms(50);
   mem.ecriture(0, 'g');
        led.rouge();
    }

    if (arretDebut[4] == 'E'){
           Son sono;
   sono.runSong(64);
   _delay_ms(750);
   sono.stopSong();
   _delay_ms(50);
   sono.runSong(64); //change to 1 for traverserParcours
   _delay_ms(750);
     sono.stopSong();
   _delay_ms(50);
   mem.ecriture(0, 'g');
        while(true){
            led.ambre();
        }
    }

}