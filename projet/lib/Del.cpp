
#define F_CPU 8000000
#include "Del.h"

Del::Del(uint8_t pinA, uint8_t pinB) : pinA(pinA), pinB(pinB), port(nullptr)
{}

Del::~Del()
{}

void Del::initialiser(volatile uint8_t* ddrPort, volatile uint8_t* port) {
    this->port = port;
    *ddrPort |= (1 << pinA) | (1 << pinB);  
}

void Del::rouge()
{
    if (port) {
        *port &= ~(1 << pinA);  
        *port |= (1 << pinB);   
    }
}

void Del::vert()
{
    if (port) {
        *port &= ~(1 << pinB);  
        *port |= (1 << pinA);   
    }
}

void Del::eteint()
{
    if (port) {
        *port &= ~(1 << pinA);  
        *port &= ~(1 << pinB);  
    }
}

void Del::ambre()
{
    rouge();
    _delay_ms(10);
    vert();
    _delay_ms(10);
}
