
#ifndef TIME_H
#define TIME_H

#include <stdbool.h>




/* Define a frequência de máquina para o valor mais próximo desejado.
** 
** !!!!! Não é recomendado chamar essa função mais de uma vez no código !!!!!
** !!!!! Caso a freuência de máquina seja alterada quando timing ou thread estiver habilitado, é possível ocorrer imprecisão na medição do tempo !!!!!
** !!!!! Recomenda-se chamar-la no início da função main() !!!!!
**
** Retorna a frequência de máquina verdadeira
*/
extern long setClock(long desiredFrequency);

// Retorna a frequência de máquina verdadeira					
extern long getMachineFrequency();





extern void delay(float time); 		// Delay em segundos
extern void delayMs(float time);	// Delay em milisegundos
extern void delayUs(float time);	// Delay em microsegundos

// Retorna o número de ciclos passados desde a última chamada de resetTiming() ou initiateTiming()
extern long long cycles();

// Retorna o tempo em segundos passado desde a última chamada de resetTiming() ou initiateTiming()
extern float seconds();

// Retorna o tempo em milisegundos passado desde a última chamada de resetTiming() ou initiateTiming()
extern float milliSeconds();

// Retorna o tempo em microsegundos passado desde a última chamada de resetTiming() ou initiateTiming()
extern float microSeconds();





/* Inicia a contagem de tempo
**
** !!!!! Utiliza o Timer1 !!!!!
** !!!!! Habilita interrupção de Timer1 !!!!
**
*/
extern void initiateTiming();

// Faz com que a contagem de tempo retorne a 0
extern void resetTiming();

/*
** Desabilita a contagem de tempo
** 
** !!!!! Desliga o Timer1 !!!!!
** !!!!! Desabilita interrupção de Timer1 !!!!!
*/
extern void endTiming();







/* Habilita a utilização de uma "thread"
**
** !!!!! Utiliza o Timer2 !!!!!
** !!!!! Habilita interrupção de Timer2 !!!!!
** !!!!! Somente chama uma função repetidamente após um número de ciclos !!!!!
** !!!!! Problemas podem ocorrer caso o tempo de execução de uma função for maior que o período em que é chamada !!!!!
*/
extern void initiateThread();

/* Desabilita a utilização da "thread"
** 
** !!!!! Desliga o Timer2 !!!!!
** !!!!! Desabilita interrupção de Timer2 !!!!!
** !!!!! A função para de ser repetidamente chamada !!!!!
*/
extern void endThread();

// Chama a função passada após um número de ciclos
// Retorna verdadeiro se foi possível habilitar a thread
extern bool timer(void (*function)(), long time);

// Chama a função passada após tempo em segundos
// Retorna verdadeiro se foi possível habilitar a thread
extern bool timerSeconds(void (*function)(), float timeF);

// Chama a função passada após tempo em milisegundos
// Retorna verdadeiro se foi possível habilitar a thread
extern bool timerMilliSeconds(void (*function)(), float timeF);

// Chama a função passada após tempo em microsegundos
// Retorna verdadeiro se foi possível habilitar a thread
extern bool timerMicroSeconds(void (*function)(), float timeF);

#endif
