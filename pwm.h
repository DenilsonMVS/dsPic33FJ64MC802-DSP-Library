
#ifndef PWM_H
#define PWM_H

// Habilita pwm no módulo do PWM2. 
// Parâmetro "value" deve ser de 0 a 1000. 
// Parâmetro pin deve ser 1, 2 ou 3. Referindo-se ao módulo 1, 2 e 3 do PWM1.
// No caso, o sinal do pwm será enviado para o pino alto (H). O pino baixo (L) estará em 0V.
extern void pwm(int value, char pin);

#endif
