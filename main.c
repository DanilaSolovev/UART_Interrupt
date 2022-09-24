
#include "main.h"


/*Определение прототипов функциий*/
void delay (uint32_t ticks);
void UART2cnf(void);
void UARTSend(int8_t c);

void UARTSendSTR(char *string);
// Определение переменных
uint32_t spd = 0;

int main(void)
{

	/*Инициализация прерываний*/
	UART2cnf();


    
    while(1)
    {
//        uint8_t data = UARTResive();
//        if (data !=0){spd=data;}
        UARTSend(0x01);
        delay(100000);
    }
    





}

void UART2cnf(void)
{
    RCC->APB1ENR |= (1<<17);
    RCC->APB2ENR = RCC->APB2ENR | RCC_APB2ENR_IOPAEN_Msk;
    GPIOA->CRL = 0;
    GPIOA->CRL &= ~(3<<8);
    GPIOA->CRL |= (2<<10);
	
    GPIOA->CRL &= ~(3<<12);
    GPIOA->CRL |= (2<<14);
	
    GPIOA->ODR |= 1<<3;
    
    USART2->CR1 |= 0x00;
    USART2->CR1 |= (1<<13);
    USART2->CR1 &= ~(1<<12);
    USART2->BRR =0x22C;
    
    USART2->CR1 |= (1<<2);
    USART2->CR1 |= (1<<3);
    /* Разрешение прерываний */
    NVIC->ISER[1] = (uint32_t)(1UL << (((uint32_t)USART2_IRQn ) & 0x1FUL));
    USART2->CR1 |= (1<<5); /*Прерывание при приеме данных*/
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
