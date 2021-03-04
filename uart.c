
#include "uart.h"

#include "round.h"
#include "time.h"
#include <xc.h>
#include <stdbool.h>
#include <string.h>


void initUART(unsigned long int desiredBaudRate) {
    
    RPINR18bits.U1RXR = 7;
    RPOR3bits.RP6R = 3;

    U1MODEbits.STSEL = 1;
    U1MODEbits.PDSEL = 0;
    U1MODEbits.ABAUD = 0;
    U1MODEbits.BRGH = 0;
    
    U1BRG = roundFloat((float) getMachineFrequency() / desiredBaudRate / 16 - 1);

    U1STAbits.URXISEL = 0;
    
    IFS0bits.U1RXIF = 0;
    IEC0bits.U1RXIE = 1;
    
    U1MODEbits.UARTEN = 1;
    U1STAbits.UTXEN = 1;
    
    delayMs(1);
    
}


typedef union {
	int i;
    long int li;
    float f;
	unsigned char bytes[4];
} ConvUnion;

void writeChar(char c) {
    while(U1STAbits.TRMT == 0);
    U1TXREG = c;
}

void writeString(char *string) {
    unsigned char stringLength = strlen(string);
    for(unsigned char i = 0; i < stringLength; i++) {
        while(U1STAbits.TRMT == 0);
        U1TXREG = string[i];
    }
    while(U1STAbits.TRMT == 0);
    U1TXREG = '\n';
}

void writeInt(int num) {
    ConvUnion myUnion;
	myUnion.i = num;
    
    while(U1STAbits.TRMT == 0);
    U1TXREG = myUnion.bytes[0];
    while(U1STAbits.TRMT == 0);
    U1TXREG = myUnion.bytes[1];
}

void writeLongInt(long int num) {
    ConvUnion myUnion;
	myUnion.li = num;
    
    while(U1STAbits.TRMT == 0);
    U1TXREG = myUnion.bytes[0];
    while(U1STAbits.TRMT == 0);
    U1TXREG = myUnion.bytes[1];
    while(U1STAbits.TRMT == 0);
    U1TXREG = myUnion.bytes[2];
    while(U1STAbits.TRMT == 0);
    U1TXREG = myUnion.bytes[3];
}

void writeFloat(float num) {
	ConvUnion myUnion;
	myUnion.f = num;
    
    while(U1STAbits.TRMT == 0);
    U1TXREG = myUnion.bytes[0];
    while(U1STAbits.TRMT == 0);
    U1TXREG = myUnion.bytes[1];
    while(U1STAbits.TRMT == 0);
    U1TXREG = myUnion.bytes[2];
    while(U1STAbits.TRMT == 0);
    U1TXREG = myUnion.bytes[3];
}


static volatile unsigned char startRecieveBuffer = 0;
static volatile unsigned char endRecieveBuffer = 0;
static volatile char recieveBuffer[32];

void __attribute__((__interrupt__, no_auto_psv)) _U1RXInterrupt() {
    recieveBuffer[(endRecieveBuffer++) % 32] = U1RXREG;
    IFS0bits.U1RXIF = 0;
}

char readChar() {
    while(endRecieveBuffer == startRecieveBuffer);
    return recieveBuffer[(startRecieveBuffer++) % 32];
}

void readString(char *string) {
    for(char i = 0; true; i++) {
        while(endRecieveBuffer == startRecieveBuffer);
        char c = recieveBuffer[(startRecieveBuffer++) % 32];
        if(c == '\n') {
            string[i] = 0;
            break;
        }
        string[i] = c;
    }
}

int readInt() {
    ConvUnion myUnion;
    
    while(endRecieveBuffer == startRecieveBuffer);
    myUnion.bytes[0] = recieveBuffer[(startRecieveBuffer++) % 32];
    while(endRecieveBuffer == startRecieveBuffer);
    myUnion.bytes[1] = recieveBuffer[(startRecieveBuffer++) % 32];
    
    return myUnion.i;
}

long int readLongInt() {
    ConvUnion myUnion;
    
    while(endRecieveBuffer == startRecieveBuffer);
    myUnion.bytes[0] = recieveBuffer[(startRecieveBuffer++) % 32];
    while(endRecieveBuffer == startRecieveBuffer);
    myUnion.bytes[1] = recieveBuffer[(startRecieveBuffer++) % 32];
    while(endRecieveBuffer == startRecieveBuffer);
    myUnion.bytes[2] = recieveBuffer[(startRecieveBuffer++) % 32];
    while(endRecieveBuffer == startRecieveBuffer);
    myUnion.bytes[3] = recieveBuffer[(startRecieveBuffer++) % 32];
    
    return myUnion.li;
}

float readFloat() {
    ConvUnion myUnion;
    
    while(endRecieveBuffer == startRecieveBuffer);
    myUnion.bytes[0] = recieveBuffer[(startRecieveBuffer++) % 32];
    while(endRecieveBuffer == startRecieveBuffer);
    myUnion.bytes[1] = recieveBuffer[(startRecieveBuffer++) % 32];
    while(endRecieveBuffer == startRecieveBuffer);
    myUnion.bytes[2] = recieveBuffer[(startRecieveBuffer++) % 32];
    while(endRecieveBuffer == startRecieveBuffer);
    myUnion.bytes[3] = recieveBuffer[(startRecieveBuffer++) % 32];
    
    return myUnion.f;
}
