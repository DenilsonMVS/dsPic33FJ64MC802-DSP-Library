
#include <xc.h>
#include <math.h>
#include <libpic30.h>

#include "time.h"
#include "round.h"

static const long oscillatorFrequency = 7370000;
static const long cyclesTimerOverflow = 0x10000;

static volatile long long timer1OverflowCount = 0;
static volatile long machineFrequency = 23031250;

void __attribute__((__interrupt__,auto_psv)) _T1Interrupt() {
    timer1OverflowCount++;
    IFS0bits.T1IF = 0;
}

long setClock(long desiredFrequency) {
    
    const long baseFrequency = oscillatorFrequency / 36;
    
    CLKDIVbits.DOZEN = 0;
    CLKDIVbits.PLLPRE = 7;
	CLKDIVbits.FRCDIV = 0;
	CLKDIVbits.PLLPOST = 0;
	OSCTUN = 0;
    
	PLLFBDbits.PLLDIV = roundFloat((float) desiredFrequency / baseFrequency - 2);
    
	machineFrequency = baseFrequency * (PLLFBDbits.PLLDIV + 2);
    
    __builtin_write_OSCCONH(0x01);
    __builtin_write_OSCCONL(OSCCON | 0x01);
    
    while(OSCCONbits.OSWEN != 0);
    
    while(OSCCONbits.LOCK!= 1);
    
    return machineFrequency;
}

long getMachineFrequency() {
    return machineFrequency;
}

void delay(float time) {
    __delay32(machineFrequency * time);
}

void delayMs(float time) {
    __delay32(machineFrequency * time / 1000);
}

void delayUs(float time) {
    __delay32(machineFrequency * time / 1000000);
}


void initiateTiming() {
    T1CONbits.TCKPS = 0;
    IFS0bits.T1IF = 1;
    IEC0bits.T1IE = 1;
    PR1 = 0xFFFF;
    TMR1 = 0x0;
    T1CONbits.TON = 1;
}

void endTiming() {
    IEC0bits.T1IE = 0;
    T1CONbits.TON = 0;
}

long long cycles() {
    return TMR1 + cyclesTimerOverflow * timer1OverflowCount;
}

float seconds() {
    float tmp = TMR1 + cyclesTimerOverflow * timer1OverflowCount;
    tmp /= machineFrequency;
    return tmp;
}

float milliSeconds() {
    float tmp = TMR1 + cyclesTimerOverflow * timer1OverflowCount;
    tmp *= 1000.0f;
    tmp /= machineFrequency;
    return tmp;
}

float microSeconds() {
    float tmp = TMR1 + cyclesTimerOverflow * timer1OverflowCount;
    tmp *= 1000000.0f;
    tmp /= machineFrequency;
    return tmp;
}

void resetTiming() {
    timer1OverflowCount = 0;
    TMR1 = 0;
}


static void (*volatile func)();
static volatile long long timeToCall;

void initiateThread() {
    T2CONbits.TCKPS = 0;
    IFS0bits.T2IF = 0;
    IEC0bits.T2IE = 0;
    PR2 = 0xFFFF;
    TMR2 = 0;
    T2CONbits.TON = 1;
}

void endThread() {
    IEC0bits.T2IE = 0;
    T2CONbits.TON = 0;
}

void __attribute__((__interrupt__,auto_psv)) _T2Interrupt() {
    if(timeToCall > 0x10000) {
        timeToCall -= 0x10000;
        if(timeToCall < 0x10000) {
            PR2 = timeToCall;
            timeToCall = 0;
        }
    } else if(timeToCall == 0) {
        IEC0bits.T2IE = 0;
        PR2 = 0xFFFF;
        func();
    }
    
    IFS0bits.T2IF = 0;    
}

bool timer(void (*function)(), long time) {
    if(!IEC0bits.T2IE) {
        func = function;
        if(time < 0xFFFF) {
            timeToCall = 0;
            TMR2 = 0;
            PR2 = time;
        } else
            timeToCall = time; 
        IEC0bits.T2IE = true;
        return true;
    } else
        return false;
}

bool timerSeconds(void (*function)(), float timeF) {
    long time = timeF * machineFrequency;
    if(!IEC0bits.T2IE) {
        func = function;
        if(time < 0xFFFF) {
            timeToCall = 0;
            TMR2 = 0;
            PR2 = time;
        } else
            timeToCall = time; 
        IEC0bits.T2IE = true;
        return true;
    } else
        return false;
}

bool timerMilliSeconds(void (*function)(), float timeF) {
    long time = timeF * machineFrequency / 1000;
    if(!IEC0bits.T2IE) {
        func = function;
        if(time < 0xFFFF) {
            timeToCall = 0;
            TMR2 = 0;
            PR2 = time;
        } else
            timeToCall = time; 
        IEC0bits.T2IE = true;
        return true;
    } else
        return false;
}

bool timerMicroSeconds(void (*function)(), float timeF) {
    long time = timeF * machineFrequency / 1000000;
    if(!IEC0bits.T2IE) {
        func = function;
        if(time < 0xFFFF) {
            timeToCall = 0;
            TMR2 = 0;
            PR2 = time;
        } else
            timeToCall = time; 
        IEC0bits.T2IE = true;
        return true;
    } else
        return false;
}

