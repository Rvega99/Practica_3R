#include "lib/include.h"

extern void Configurar_UART3(void)
{
    SYSCTL->RCGCUART  = (1<<3);   //Paso 1 (RCGCUART) pag.388 UART/modulo0 0->Disable 1->Enable
    SYSCTL->RCGCGPIO |= (1<<0);     //Paso 2 (RCGCGPIO) pag.382 Enable clock port A
    //(GPIOAFSEL) pag.1770 Enable alternate function
    GPIOA_AHB->AFSEL = (1<<5) | (1<<4);
    //GPIO Port Control (GPIOPCTL) PA4-> U3Rx PA5-> U3Tx pag.741
    GPIOA_AHB->PCTL = (GPIOA_AHB->PCTL&0xFFFFFF00) | 0x00000011;// (1<<0) | (1<<4);//0x00000011
    // GPIO Digital Enable (GPIODEN) pag.682
    GPIOA_AHB->DEN = (1<<0) | (1<<1);//PA4 PA5
    //UART0 UART Control (UARTCTL) pag.918 Apagar para configurar primero todo y luego encender hasta abajo 
    UART3->CTL = (0<<9) | (0<<8) | (0<<0);

    // UART Integer Baud-Rate Divisor (UARTIBRD) pag.914
   //  Configurar_UART3(); //Jesus,Yesica,Carlos,Dulce,Rodolfo,Leonardo -fclk 80MHZ Baud-rate 19200
        //  BRD = 80,000,000 / (16*19200) =260.4166
        //  UARTFBRD[DIVFRAC] = integer(.4166 * 64 + 0.5)= 27

    UART3->IBRD = 260;
    // UART Fractional Baud-Rate Divisor (UARTFBRD) pag.915
    UART3->FBRD = 27;
    //  UART Line Control (UARTLCRH) pag.916
    UART3->LCRH = (0x3<<5)|(1<<4);
    //  UART Clock Configuration(UARTCC) pag.939
    UART3->CC =(0<<0);
    //Disable UART3 UART Control (UARTCTL) pag.918
    UART3->CTL = (1<<0) | (1<<8) | (1<<9);



}

extern char readChar(void)
{
    //UART FR flag pag 911
    //UART DR data 906
    int v;
    char c;
    while((UART3->FR & (1<<4)) != 0 );
    v = UART3->DR & 0xFF;
    c = v;
    return c;
}
extern void printChar(char c)
{
    while((UART3->FR & (1<<5)) != 0 );
    UART3->DR = c;
}
extern void printString(char* string)
{
    while(*string)
    {
        printChar(*(string++));
    }
}

extern int readString(char delimitador,char *string)
{

   int i=0;
   char c = readChar();
   while(c != delimitador)
   {
       *(string+i) = c;
       i++;
       c = readChar();
   }

   return i;

}
//Experimento 2
//El envio es su nombre  (rave) 
// invertirlo y regresarlo con numeros consecutivos
// entre letras (e1v2a3r) 