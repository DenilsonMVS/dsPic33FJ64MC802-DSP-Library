
#include "qei.h"

void enableQEI1(unsigned char pin1, unsigned char pin2) {
	TRISB &= ~(1 << pin1);
	TRISB &= ~(1 << pin2);
	RPINR14bits.QEA1R = pin1;
	RPINR14bits.QEB1R = pin2;

	RPINR15bits.INDX1R = 0b11111;

	QEI1CONbits.CNTERR = 0b0;    
    QEI1CONbits.QEISIDL = 0b0;    
    QEI1CONbits.SWPAB = 0b0;    
    QEI1CONbits.PCDOUT = 0b1; 
    
    DFLT1CONbits.QEOUT = 0b0;  
    DFLT1CONbits.CEID = 0b1;

    POS1CNT = 0;
    MAX1CNT = 0xFFFF;
    
    QEI1CONbits.QEIM = 0b111;
}

void enableQEI2(unsigned char pin1, unsigned char pin2) {
	TRISB &= ~(1 << pin1);
	TRISB &= ~(1 << pin2);
	RPINR16bits.QEA1R = pin1;
	RPINR16bits.QEB1R = pin2;

	RPINR17bits.INDX1R = 0b11111;

	QEI2CONbits.CNTERR = 0b0;    
    QEI2CONbits.QEISIDL = 0b0;    
    QEI2CONbits.SWPAB = 0b0;    
    QEI2CONbits.PCDOUT = 0b1; 
    
    DFLT2CONbits.QEOUT = 0b0;  
    DFLT2CONbits.CEID = 0b1;

    POS2CNT = 0;
    MAX2CNT = 0xFFFF;
    
    QEI2CONbits.QEIM = 0b111;
}

