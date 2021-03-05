
# dsPic33FJ64MC802-DSP-Library
Funções úteis para se programar um dsPIC33FJ64MC802 no compilador XC16.
Contém funções para UART, QEI, PWM, PLL, ADC e Timer.

Iniciação científica pelo CEFET-MG campus Divinópolis  
Título: Desenvolvimento de um DSP para sistemas de controle inteligentes  
Aluno: Denilson Martins Vieira Santos
Orientador: Dr. Lucas Silva de Oliveira


Como o dsPIC33FJ64MC802 é um microcontrolador de 16 bits, deve-se atentar ao tamanho de suas variáveis:

    int: 16 bits -> (-32768 / 32767)
    long int: 32 bits -> (-2147483648 / 2147483647)
    long long int: 64 bits -> (-9223372036854775808 / 9223372036854775807)



Não é recomendado a utilização de macros.
Ocorreram problemas ao realizar operações aritiméticas com macros. Exemplo:

    #define A 50                                -> (Funciona) A possui o valor de 50
    #define B 20                                -> (Funciona) B possui o valor de 20
    #define C ((A) * (B))                       -> (Não funciona) C acaba por não possui valor de 1000



As funções foram testadas com as seguintes configurações:

	#pragma config BWRP = WRPROTECT_OFF     //Desligadas todas as proteções
	#pragma config BSS = NO_FLASH
	#pragma config RBS = NO_RAM
	#pragma config SWRP = WRPROTECT_OFF
	#pragma config SSS = NO_FLASH
	#pragma config RSS = NO_RAM
	#pragma config GWRP = OFF
	#pragma config GSS = OFF

	// Configuração de oscilador
	#pragma config FNOSC = FRCPLL           // Utilizado oscilador FRC interno com PLL
	                                        // Com o PLL é possível ajustar a frequência 
	                                        // de oscilação durante a execução do programa
	                                        // podendo aumenta-la ou reduzi-la

	/*  FNOSC pode ser configurado como:
	** 
	**  LPRCDIVN    :   Oscilador interno com frequência dividida por N (N pode ser configurado depois por meio de CLKDIVbits.DOZE)
	**  FRCDIV16    :   Oscilador interno com frequência dividida por 16    7,37MHz / 16 = 461kHz
	**  LPRC        :   Oscilador interno com baixa frequência e baixa potência
	**  SOSC        :   Oscilador secundário
	**  PRIPLL      :   Oscilador primário com PLL
	**  PRI         :   Oscilador primário
	**  FRC         :   Oscilador interno RC    7,37MHz
	*/

	#pragma config IESO = OFF               // Troca de oscilador a partir de um tempo, iniciamente é usado o FRC interno, depois é utilizado o oscilador primário.

	#pragma config POSCMD = NONE            // Fonte do oscilador primário (não é usado)

	#pragma config OSCIOFNC = ON            // Função dos pinos OSC2 
	                                        // 0: trabalham como entradas de oscilador
	                                        // 1: trabalham como portas digitais de entrada ou saída

	#pragma config IOL1WAY = OFF            // Configuração de seleção de pinos
	#pragma config FCKSM = CSECMD           // Troca de clock

	#pragma config WDTPOST = PS32768
	#pragma config WDTPRE = PR128
	#pragma config WINDIS = OFF
	#pragma config FWDTEN = OFF             // Watchdog Timer desligado

	#pragma config FPWRT = PWR1             // POR Timer Value (desabilitado)
	#pragma config ALTI2C = OFF             // Trocar pinos I2C (É possível trocar os pinos usados pelo módulo I2C)
	#pragma config LPOL = ON                // Motor Control PWM Low Side Polarity bit (PWM module low side output pins have active-high output polarity)
	#pragma config HPOL = ON                // Motor Control PWM High Side Polarity bit (PWM module high side output pins have active-high output polarity)

	#pragma config PWMPIN = OFF             // Controle dos pinos de PWM 
	                                        // ON: controlados pelo PORT
	                                        // OFF: controlados pelo módulo PWM

	#pragma config ICS = PGD1               // Seleção de canal de comunicação (PGD1)
	#pragma config JTAGEN = OFF             // Habilitação de JTAG (desabilitado)


Caso ocorra algum tipo de alteração nessas configurações, é possível que as funções não operem da forma esperada.
Configurações mais prováveis de alterar o comportamento das funções:

    FNOSC
    IESO
    LPOL
    HPOL
    PWMPIN
