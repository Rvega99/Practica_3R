 #include "lib/include.h"

extern void Configurar_UART3(void)
{
    SYSCTL->RCGCUART  = (1<<3); //El registro RCGCUART proporciona al software la capacidad de habilitar
                                //y deshabilitar los módulos UART en modo Ejecutar UART 3 0->Apagado 1->Encendido
    
    SYSCTL->RCGCGPIO |= (1<<0);      //El registro RCGCGPIO proporciona al software la capacidad
                                     // de habilitar y deshabilitar módulos GPIO en modo de ejecución
    
    GPIOA_AHB->AFSEL = (1<<5) | (1<<4);     //(GPIOAFSEL) Habilitar funcion alterna 

    //El registro GPIOPCTL se utiliza junto con el registro GPIOAFSEL y selecciona el registro específico
    //señal periférica para cada pin GPIO cuando se utiliza el modo de función alternativa
    GPIOA_AHB->PCTL = (GPIOA_AHB->PCTL&0xFF00FFFF) | 0x00110000;// (1<<0) | (1<<4);//0x00000011

    // El registro GPIODEN es el registro de habilitación digital
    GPIOA_AHB->DEN = (1<<4) | (1<<5);//PA4 PA5

    //UART0 UART Control (UARTCTL) pag.918 Apagar para configurar primero todo y luego encender hasta abajo 
    //El registro UARTCTL es el registro de control. Todos los bits se borran al restablecer,
    //excepto los bits Transmit Enable (TXE) y Receive Enable (RXE),
    UART3->CTL = (0<<9) | (0<<8) | (0<<0)|(0<<4);

    // UART Integer Baud-Rate Divisor (UARTIBRD) pag.914
   //  Configurar_UART3(); //Jesus,Yesica,Carlos,Dulce,Rodolfo,Leonardo -fclk 80MHZ Baud-rate 19200
        //  BRD = 80,000,000 / (16*19200) =260.4166
        //  UARTFBRD[DIVFRAC] = integer(.4166 * 64 + 0.5)= 27

    //El registro UARTIBRD es la parte entera del valor divisor de velocidad en baudios. 
    //Todos los bits se borran al reiniciar.
    UART3->IBRD = 130;

    // Al cambiar el registro UARTFBRD, el nuevo valor no surte efecto hasta
    //La transmisión/recepción del personaje actual se ha completado
    UART3->FBRD = 14;

    //  El registro UARTLCRH es el registro de control de línea. 
    //Parámetros de serie como longitud de datos, paridad,  
    //y la selección de bits de parada se implementan en este registro.
    UART3->LCRH = (0x3<<5)|(1<<4);

    //  El registro UARTCC controla la fuente del reloj en baudios para el módulo UART
    UART3->CC =(0<<0);
    
    //Encender UART Control (UARTCTL) 
    UART3->CTL = (1<<0) | (1<<8) | (1<<9)|(1<<4);


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

