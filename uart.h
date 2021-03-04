
#ifndef UART_H
#define UART_H


// Inicia o módulo UART
extern void initUART(unsigned long int desiredBaudRate);



// Escreve um caractere na UART
extern void writeChar(char c);

/* Escreve uma string na UART
** 
** !!!!! Por causa da função readline() utilizada no Raspberry Pi 3,
** a convenção do caractere delimitador do fim da string foi alterada. Em programas normais,
** o caractere delimitador é o 0 ('\0'). No entanto, para comunicar-se com o raspberry,
** foi utilizado o caractere 32 ('\n'). Não é necessário realizar essa alteração manualmente,
** a função automaticamente envia o último byte sendo o caractere '\n'. !!!!!
**
** !!!!! Por causa da convenção descrita acima, não é possível transmitir uma string com algum caracete de quebra de linha !!!!!
*/
extern void writeString(char *string);

// Escreve um inteiro de 16 bits na UART
extern void writeInt(int num);

// Escreve um inteiro de 32 bits na UART
extern void writeLongInt(long int num);

// Escreve uma variável de ponto flutuante de 32 bits na UART
extern void writeFloat(float num);




// Fica em loop infinito até que lê um caractere
extern char readChar();

/* Fica em loop infinito até que lê uma string
** 
** !!!!! Por causa da função readline() utilizada no Raspberry Pi 3,
** a convenção do caractere delimitador do fim da string foi alterada. Em programas normais,
** o caractere delimitador é o 0 ('\0'). No entanto, para comunicar-se com o raspberry,
** foi utilizado o caractere 32 ('\n'). Não é necessário realizar essa alteração manualmente,
** a função automaticamente troca o caractere 32 ('\n') pelo 0 ('\0').
**
** !!!!! Por causa da convenção descrita acima, não é possível ler uma string com algum caracete de quebra de linha !!!!!
*/
extern void readString(char *string);

// Fica em loop infinito até que lê um inteiro de 16 bits
extern int readInt();

// Fica em loop infinito até que lê um inteiro de 32 bits
extern long int readLongInt();

// Fica em loop infinito até que lê uma variável de ponto flutuante de 32 bits na UART
extern float readFloat();

#endif
