# dsPic33FJ64MC802-DSP-Library
Funções úteis para se programar um dsPIC33FJ64MC802 no compilador XC16.
Contém funções para UART, QEI, PWM, PLL, ADC e Timer.

Iniciação científica pelo CEFET-MG campus Divinópolis
Título: Desenvolvimento de um DSP para sistemas de controle inteligentes
Aluno: Denilson Martins Vieira Santos
orientador: Dr. Lucas Silva de Oliveira

!!!!!
Como o dsPIC33FJ64MC802 é um microcontrolador de 16 bits, deve-se atentar ao tamanho de suas variáveis.

int: 16 bits -> (-32768 / 32767)
long int: 32 bits -> (-2147483648 / 2147483647)
long long int: 64 bits -> (-9223372036854775808 / 9223372036854775807)
!!!!!


!!!!! 
Não é recomendado a utilização de macros

Ocorreram problemas ao realizar operações aritiméticas com macros. Exemplo:

    #define A 50                                -> (Funciona) A possui o valor de 50
    #define B 20                                -> (Funciona) B possui o valor de 20
    #define C ((A) * (B))                       -> (Não funciona) C acaba por não possui valor de 1000
!!!!!


!!!!!
As funções foram testadas com as seguintes configurações:

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


Caso ocorra algum tipo de alteração nessas configurações, é possível que as funções não operem da forma esperada.

Configurações mais prováveis de alterar o comportamento das funções:

FNOSC
IESO
LPOL
HPOL
PWMPIN

!!!!!
