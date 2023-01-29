#include "lib/include.h"

int main(void)
{
  
    char c='5';
    //char b='a';
    Configurar_PLL();  //Confiuracion de velocidad de reloj
    Configurar_UART3();//Configuracion de UART 3 con los valores del experimeento 1

    //Experimento 1
    //  Configurar_UART3(); //Jesus,Yesica,Carlos,Dulce,Rodolfo,Leonardo -fclk 80MHZ Baud-rate 19200
   
    Configurar_GPIO();
 
    while(1)
    {
       int i= 0;
       char string[20];
       char string2[20];
       i=readString('%', string,string2);
       printString(string2);
    }
}
