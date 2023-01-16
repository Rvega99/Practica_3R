#include "lib/include.h"

extern void Configurar_UART1(void)
{
    SYSCTL->RCGCUART  = (1<<1);   //Paso 1 (RCGCUART) pag.388 0->Disable 1->Enable
    //                    A         B       C         D        E       F 
    SYSCTL->RCGCGPIO |= (0<<0) | (1<<1) | (0<<2) | (0<<3) | (0<<4) | (0<<5);     //Paso 2 (RCGCGPIO) pag.340 Enable clock port A
    //(GPIOAFSEL) pag.815 Enable alternate function
    GPIOB_AHB->AFSEL = (1<<1) | (1<<0);
    //GPIO Port Control (GPIOPCTL) PA0-> U0Rx PA1-> U0Tx pag.1806
    GPIOB_AHB->PCTL = (GPIOB_AHB->PCTL&0xFFFFFF00) | 0x00000011;// (1<<0) | (1<<4);//0x00000011
    // GPIO Digital Enable (GPIODEN) pag.682
    GPIOB_AHB->DEN = (1<<0) | (1<<1);//PB1 PB0
    //UART0 UART Control (UARTCTL) pag.918 DISABLE!!
    UART1->CTL = (0<<9) | (0<<8) | (0<<0)| (0<<4);

    // UART Integer Baud-Rate Divisor (UARTIBRD) pag.914
    /*
    BRD = 20,000,000 / (16 * 9600) = 130.2
    UARTFBRD[DIVFRAC] = integer(0.2 * 64 + 0.5) = 14
    */
    UART1->IBRD = 260;
    // UART Fractional Baud-Rate Divisor (UARTFBRD) pag.915
    UART1->FBRD = 27;
    //  UART Line Control (UARTLCRH) pag.916
    UART1->LCRH = (0x3<<5)|(1<<4);
    //  UART Clock Configuration(UARTCC) pag.939
    UART1->CC =(0<<0);
    //Disable UART0 UART Control (UARTCTL) pag.918
    UART1->CTL = (1<<0) | (1<<8) | (1<<9)| (1<<4);



}

extern char readChar(void)
{
    //UART FR flag pag 911
    //UART DR data 906
    int v;
    char c;
    while((UART1->FR & (1<<4)) != 0 );
    v = UART1->DR & 0xFF;
    c = v;
    return c;
}
extern void printChar(char c)
{
    while((UART1->FR & (1<<5)) != 0 );
    UART1->DR = c;
}
extern void printString(char* string2)
{
    while(*string2)
    {
        printChar(*(string2++));
    }
}

extern int readString(char delimitadorF,char *string,char *string2)
{
    int s=0;
int j=0;
   int i=0;
   char c = readChar();
   while(c != delimitadorF)
   {
       string[i]=c;
       i++;
       c=readChar();
   }
   s=i*2;
   int h=0;
   for(j=0;j<s;j++)
   {
       
       string2[h]=string[i-1];
       h++;
       string2[h]=j+1;
       h++;
       i--;
   }

   return i;

}
extern char invertirString(char *string)
{
    int i=0;
int J=0;
    int longitud = sizeof(string);
  char temp;
  for (int i= 0, j = longitud - 1; i < (longitud / 2); i++, J--) 
  {
    temp= string[i];
    string[i] = string[J];
    string[J] = temp;
  }
  return string;
}
//Experimento 2

//El envio es su nombre  (rave) 

// invertirlo y regresarlo con numeros consecutivos
// entre letras (e1v2a3r) 

