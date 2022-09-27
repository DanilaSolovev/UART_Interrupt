
#include "main.h"


/*Определение прототипов функциий*/
void delay (uint32_t ticks);
void UARTSend(int8_t c);

void UARTSendSTR(char *string);
// Определение переменных
uint32_t spd = 0;

int main(void)
{

	/*Инициализация UART*/
	UARTcnf(2,14400);
    
    while(1)
    {
//       uint8_t data = UARTResive();
//        if (data !=0){spd=data;}
//        UARTSend('a');
        delay(100000);
    }
    





}

void UARTSend(int8_t c)
{
    USART2->DR = c;
	while (!(USART2->SR & (1<<6)));
}
void UARTSendSTR(char *string)
{
    while (*string) UARTSend (*string++);
}



void delay (uint32_t ticks)
{
	for (uint32_t i = 0; i < ticks; i++)
	{
	}
}




void HardFault_Handler (void)
{
	while(1)
	{}
}
