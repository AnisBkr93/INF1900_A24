#include "Sonnerie.h"

uint16_t prescalar = 256 ;

Son::Son()
{
    DDRD |= (1<<PD7) | (1 << PD6);
    PORTB &= ~(1<<PB6);
    TCCR2A |= (1<< WGM21) | (1<<COM2A0);//mode CTC
    TCCR2B |= (1<<CS21) | (1<<CS22) ;// prescalaire  256
    TCNT2 =0 ;

}

Son::~Son()
{}


void Son::runSong(double frequency)
    {
        TCCR2A |= (1<< WGM21) | (1<<COM2A0);//mode CTC
        OCR2A= ((8000000 / (2* prescalar * frequency))-1);
    }

void Son::stopSong()
{
    OCR2A = 0 ;
    TCCR2A = 0;
}