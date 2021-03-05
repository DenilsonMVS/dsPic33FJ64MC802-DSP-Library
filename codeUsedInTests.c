
#pragma config BWRP = WRPROTECT_OFF
#pragma config BSS = NO_FLASH
#pragma config RBS = NO_RAM

#pragma config SWRP = WRPROTECT_OFF
#pragma config SSS = NO_FLASH
#pragma config RSS = NO_RAM

#pragma config GWRP = OFF
#pragma config GSS = OFF

#pragma config FNOSC = FRCPLL
#pragma config IESO = OFF 

#pragma config POSCMD = NONE
#pragma config OSCIOFNC = ON
#pragma config IOL1WAY = OFF
#pragma config FCKSM = CSECMD

#pragma config WDTPOST = PS32768
#pragma config WDTPRE = PR128
#pragma config WINDIS = OFF
#pragma config FWDTEN = OFF

#pragma config FPWRT = PWR128
#pragma config ALTI2C = OFF
#pragma config LPOL = ON
#pragma config HPOL = ON
#pragma config PWMPIN = ON

#pragma config ICS = PGD1
#pragma config JTAGEN = OFF


#include <xc.h>                         
#include <stdbool.h>
#include <string.h>
#include <libpic30.h>

#include "time.h"
#include "uart.h"

#define UART_SPEED 2500000

static float samplingTime;
static float endTime;

static float proportional;
static float integrative;
static float derivative;

static float currentTime;
static float lastTime;
static float readValue;
static int readValueInt;
static float error;
static float controlSignal;
static float area = 0;
static float lastError = 0;

static void (*runController)();

static int currentValue;
static int analogInputValue;

static void (*readExtraData)();
static void (*transmitExtraData)();

void initiateQEI() {
    TRISBbits.TRISB4 = 1;
    TRISBbits.TRISB5 = 1;
    
    RPINR14bits.QEA1R = 4;
    RPINR14bits.QEB1R = 5;
    RPINR15bits.INDX1R = 0b11111;
    
    QEICONbits.QEIM = 0;
    QEICONbits.CNTERR = 0;
    QEICONbits.QEISIDL = 0;
    QEICONbits.SWPAB = 0;
    QEICONbits.PCDOUT = 0;
    QEICONbits.POSRES = 1;
    DFLTCONbits.CEID = 1;
    DFLTCONbits.QEOUT = 1;
    DFLTCONbits.QECK = 5;
    POSCNT = 0;
    QEICONbits.QEIM = 6;
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

int readADC(char pin) {
    AD1CHS0bits.CH0SA = pin;
    AD1CON1bits.SAMP = 1;
    __delay32(32);
    AD1CON1bits.SAMP = 0;
    while(!AD1CON1bits.DONE);
    return ADC1BUF0;
}


void runPController() {
    readValueInt = POS1CNT % 24;
    readValue = readValueInt * 0.2618f;
    controlSignal = (3.142f - readValue) * proportional;
    lastTime = currentTime;
}

void runPIController() {
    readValueInt = POS1CNT % 24;
    readValue = readValueInt * 0.2618f;
    error = 3.142f - readValue;
    area += error / (currentTime - lastTime);
    controlSignal = error * proportional + area * integrative;
    lastTime = currentTime;
}

void runPDController() {
    readValueInt = POS1CNT % 24;
    readValue = readValueInt * 0.2618f;
    error = 3.142f - readValue;
    controlSignal = error * proportional + (error - lastError) * derivative / (currentTime - lastTime); 
    lastTime = currentTime;
    lastError = error;
}

void runPIDController() {
    readValueInt = POS1CNT % 24;
    readValue = readValueInt * 0.2618f;
    error = 3.142f - readValue;
    
    area += error / (currentTime - lastTime);
    
    controlSignal = error * proportional + area * integrative + (error - lastError) * derivative / (currentTime - lastTime);
    
    lastTime = currentTime;
    lastError = error;
}

void transmitData() {
    writeFloat(currentTime);
    writeInt(readValueInt);
    writeFloat(controlSignal);
}

void mainRoutine() {
    runController();
    readExtraData();
    transmitData();
    transmitExtraData();
    
    LATBbits.LATB3 = ~LATBbits.LATB3;
}


void runOpenController() {
    readValueInt = POS1CNT % 24;
    readValue = readValueInt * 0.2618f;
    lastTime = currentTime;
}

void transmitOpenData() {
    writeFloat(currentTime);
    writeInt(readValueInt);
}

void openMainRoutine() {
    runOpenController();
    readExtraData();
    transmitOpenData();
    transmitExtraData();
    LATBbits.LATB3 = ~LATBbits.LATB3;
}


void nothing() {}

void readOnlyCurrentSensor() {
    currentValue = ADC1BUF0;
}
void transmitOnlyCurrentSensor() {
    writeInt(currentValue);
}

void readOnlyAnalogInput() {
    analogInputValue = ADC1BUF0;
}
void transmitOnlyAnalogInput() {
    writeInt(analogInputValue);
}

void readCurrentSensorAndAnalogInput() {
    currentValue = readADC(1);
    analogInputValue = readADC(2);
}
void transmitCurrentSensorAndAnalogInput() {
    writeInt(currentValue);
    writeInt(analogInputValue);
}


void configureExtraReadings(char extraReadings) {
    if(extraReadings == '0') {
        readExtraData = nothing;
        transmitExtraData = nothing;
    } else if(extraReadings == '1') {
        readExtraData = readOnlyCurrentSensor;
        transmitExtraData = transmitOnlyCurrentSensor;
        ADCInitForOneReading(1);
    } else if(extraReadings == '2') {
        readExtraData = readOnlyAnalogInput;
        transmitExtraData = transmitOnlyAnalogInput;
        ADCInitForOneReading(2);
    } else {
        readExtraData = readCurrentSensorAndAnalogInput;
        transmitExtraData = transmitCurrentSensorAndAnalogInput;
        ADCInitForMultipleReadings();
    }
}


void runFirstRoutine() {
    resetTiming();
        currentTime = 0.0f;
        
        if(integrative == 0.0f && derivative == 0.0f) {
            readValueInt = POS1CNT % 24;
            readValue = readValueInt * 0.2618f;
            controlSignal = (3.142f - readValue) * proportional;
            lastTime = currentTime;
            
            runController = runPController;
        } else if(derivative == 0.0f) {
            readValueInt = POS1CNT % 24;
            readValue = readValueInt * 0.2618f;
            error = 3.142f - readValue;

            area = 0.0f;

            controlSignal = error * proportional;

            lastTime = currentTime;
            
            runController = runPIController;
        } else if(integrative == 0.0f) {
            readValueInt = POS1CNT % 24;
            readValue = readValueInt * 0.2618f;
            error = 3.142f - readValue;

            controlSignal = error * proportional;

            lastTime = currentTime;
            lastError = error;
            
            runController = runPDController;
        } else {
            readValueInt = POS1CNT % 24;
            readValue = readValueInt * 0.2618f;
            error = 3.142f - readValue;

            area = 0.0f;

            controlSignal = error * proportional;

            lastTime = currentTime;
            lastError = error;
            
            runController = runPIDController;
        }
        
        readExtraData();
        transmitData();
        transmitExtraData();

        LATBbits.LATB3 = ~LATBbits.LATB3;
}

void initiateMainMode() {
    LATBbits.LATB2 = 1;
    LATBbits.LATB3 = 0;
    
    if(readChar() == 'c') {
        
        samplingTime = readFloat();
        endTime = readFloat();
        proportional = readFloat();
        integrative = readFloat();
        derivative = readFloat();
        
        configureExtraReadings(readChar());
        
        delayMs(1);
        LATBbits.LATB2 = 0;
        
        runFirstRoutine();        
        while(true) {
            currentTime = seconds();
            if(currentTime < endTime) { 
                if(currentTime - lastTime >= samplingTime)
                    mainRoutine();
            } else {
                mainRoutine();
                break;
            }
        }
        
    } else {
        
        samplingTime = readFloat();
        endTime = readFloat();
        controlSignal = readFloat();
        
        configureExtraReadings(readChar());
        
        delayMs(1);
        LATBbits.LATB2 = 0;
        
        resetTiming();
        lastTime = 0.0f;
        currentTime = 0.0f;
        
        openMainRoutine();
        while(true) {
            currentTime = seconds();
            if(currentTime < endTime) { 
                if(currentTime - lastTime >= samplingTime)
                    openMainRoutine();
            } else {
                openMainRoutine();
                break;
            }
        }
    }
}



int main() {
    
    setClock(40000000);
    initiateTiming();
    initUART(UART_SPEED);
    
    TRISB = 0;
    TRISA = 0;
    TRISAbits.TRISA1 = 1;
    TRISAbits.TRISA2 = 1;
    
    AD1PCFGL = ~TRISA;
    
    initiateQEI();
    
    while(true)
        initiateMainMode();
    
    return 1;
    
}
