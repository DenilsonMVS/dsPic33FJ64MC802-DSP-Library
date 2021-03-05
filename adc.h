
#ifndef ADC_H
#define ADC_H

/*
**	Ativa conversão analógica automática de um único pino.
**	Para realizar a leitura da última amostra, basta ler o registrador:
**	ADC1BUF0 (Retorna inteiro positivo de 12 bits)
*/
extern void ADCInitForOneReading(char pin);



/*
**	Ativa conversão analógica manual, em que é possível escolher a cada leitura o pino a ser lido.
**	Para realizar a leitura da última amostra, basta chamar a função:
**	readADC() (Retorna inteiro positivo de 12 bits)
*/
extern void ADCInitForMultipleReadings();

/* 	Função para realizar leitura analógica de pino especificado
	
	!!!!!

		É necessário fornecer o número de ciclos de máquina para a amostragem. Esse tempo de amostragem deve ser maior que 150 nanosegundos.
	
		Somente deve ser chamada se antes for chamada a função ADCInitForMultipleReadings().

	!!!!!

	Retorna inteiro positivo de 12 bits
*/
extern int readADC(char pin, unsigned samplingCycles);


#endif
