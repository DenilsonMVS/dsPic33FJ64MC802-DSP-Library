
#include "pwm.h"

void pwm(int value, char pin) {
    
    P1TCONbits.PTOPS = 1;
    P1TCONbits.PTCKPS = 0;
    P1TCONbits.PTMOD = 2;
    
    P1TMR = 0;                  
    P1TPER = 500;               
    
    if(pin == 1) {
        PWM1CON1bits.PMOD1 = 1;
        PWM1CON1bits.PEN1H = 1;
        PWM1CON1bits.PEN1L = 0;
    } else if(pin == 2) {
        PWM1CON1bits.PMOD2 = 1;
        PWM1CON1bits.PEN2H = 1;
        PWM1CON1bits.PEN2L = 0;
    } else {    
        PWM1CON1bits.PMOD3 = 1;
        PWM1CON1bits.PEN3H = 1;
        PWM1CON1bits.PEN3L = 0; 
    }
    
    DTCON1bits.DTAPS = 0;
    DTCON1bits.DTA = 59;
    
    
    if(pin == 1) 
    	P1DC1 = value % 1000;
    else if(pin == 2) 
    	P1DC2 = value % 1000;
    else 
    	P1DC3 = value % 1000;
    
    P1TCONbits.PTEN = 1;
    
}
