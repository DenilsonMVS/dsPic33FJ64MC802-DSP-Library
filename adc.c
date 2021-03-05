
#include "adc.h"

#include "time.h"


void ADCInitForOneReading(char pin) {
	AD1CON1 = 0;
    AD1CON1bits.AD12B = 1;
    AD1CON1bits.SSRC = 7;
    AD1CON1bits.ASAM = 1;
    AD1CON2 = 0;
    AD1CON3 = 0;
    AD1CON3bits.SAMC = 31;
    AD1CON3bits.ADCS = 2;
    AD1CHS0 = 0;
    AD1CHS0bits.CH0SA = pin;
    
    AD1CON1bits.ADON = 1;
    delayMs(1);
}

void ADCInitForMultipleReadings() {
    AD1CON1 = 0;
    AD1CON1bits.AD12B = 1;
    AD1CON2 = 0;
    AD1CON3 = 0;
    AD1CON3bits.ADCS = 2;
    AD1CHS0 = 0;
    
    AD1CON1bits.ADON = 1;
    delayMs(1);
}

int readADC(char pin, unsigned samplingCycles) {
    AD1CHS0bits.CH0SA = pin;
    AD1CON1bits.SAMP = 1;
    __delay32(samplingCycles);
    AD1CON1bits.SAMP = 0;
    while(!AD1CON1bits.DONE);
    return ADC1BUF0;
}
