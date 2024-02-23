#include <avr/io.h> 
#include <stdbool.h> 
#include <util/delay.h>

int main(void) 
{ 
 bool X_in = 0; 
 bool Q_A=0,Q_B=0;
 
 DDRD |= (1<<PIND3) | (1<<PIND4); // declare pins 3 and 4 as output
 DDRD &= ~(1<<PIND2); // declare pin 2 as input

 while(1) 
 { 
    X_in = (PIND & (1<<PIND2)) == (1<<PIND2);
    bool nand = !(Q_A & X_in);
    bool xor = (Q_A & !Q_B) | (!Q_A & Q_B);
    Q_A = xor;
    Q_B = nand;

    if(Q_A)
        PORTD |= (1<<PIND3); // set pin 3 high
    else
        PORTD &= ~(1<<PIND3); // set pin 3 low

    if(Q_B)
        PORTD |= (1<<PIND4); // set pin 4 high
    else
        PORTD &= ~(1<<PIND4); // set pin 4 low

    _delay_ms(1000); // Delay for 1 second
 } 
 return 0; 
}
