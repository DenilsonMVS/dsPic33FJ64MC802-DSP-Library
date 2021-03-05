
#ifndef QEI_H
#define QEI_H

/*
**	Habilita o módulo QEI.
**
**	Parâmetros número dos pinos do encoder. (Os pinos precisam estar conectados a um pino do PORTB.)
**
**	Para realizar a leitura do encoder, basta ler os registradores POS1CNT e o POS2CNT.
**
**	!!!!!
**		Na leitura do registrador, é preciso estar atento à possibilidade de underflow e overflow.
**
**		O registrador inicia com 0. Normalmente, caso ocorra um giro na direção positiva, não ocorrerá problemas.
**		No entanto caso esteja em zero, e ocorra um giro na direção negativa, o registrador retornará valores aproximados de 65536.
**		Isso acontece por causa de underflow.
**
**		E novamente caso volte a girar na direção positiva e estiver próximo de 65536, haverá grandes chance de ocorrer um overflow,
**		e o valor guardado que antes era próximo de 65536, de repente estará próximo de 0.
**	!!!!!
*/

extern void enableQEI1(unsigned char pin1, unsigned char pin2);
extern void enableQEI2(unsigned char pin1, unsigned char pin2);

#endif
