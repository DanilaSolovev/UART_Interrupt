
#include "main.h"
#define GPIO_PIN_5                 ((uint16_t)0x0020)  /* Pin 5 selected    */

/*Определение прототипов функциий*/
void delay (uint32_t ticks);

// Определение переменных
uint32_t spd = 0;

int main(void)
{
    RCC->APB2ENR = RCC->APB2ENR | RCC_APB2ENR_IOPAEN_Msk;
	
	GPIOA->CRL = (GPIOA->CRL) | ( GPIO_CRL_MODE5_0 | GPIO_CRL_MODE5_1);
	
	/* Установка регистра CRL битов CNF5[1:0] на тип выхода push-pull */
	GPIOA->CRL = (GPIOA->CRL) & ( ~( GPIO_CRL_CNF5_0 | GPIO_CRL_CNF5_1));
	/*Инициализация UART*/
    /*UARTcnf(Номер UART(1-2), Baudrate, Прерывания(0-1))*/
	//UARTcnf(2,14400,0);
    
    NVIC->ISER[(((uint32_t)TIM1_UP_IRQn) >> 5UL)] = (uint32_t)(1UL << (((uint32_t)TIM1_UP_IRQn ) & 0x1FUL));
    
    RCC->APB2ENR = RCC->APB2ENR | (1<<11);
    TIM1->DIER|=0x01;
    TIM1->SR = 0;
    TIM1->PSC=8000-1;
    TIM1->ARR = 1000-1;
    TIM1->CR1 |= 0x01; 
    //while (!(TIM1->SR & 0x01));
    //TIM1->SR = 0;
    
    while(1)
    {
       //uint8_t data = UARTResive(2,10);
//       spd = res_mas[0]+res_mas[1]+res_mas[2];
//    GPIOA->BSRR = GPIOA->BSRR | GPIO_PIN_5;     
//    while (!(TIM1->SR & 0x01));
//    TIM1->SR = 0;
//    GPIOA->BRR = GPIOA->BRR | GPIO_PIN_5;
//    while (!(TIM1->SR & 0x01));
//    TIM1->SR = 0;
        
       //UARTSendMAS(2,10);
       //delay(100000);
    }
    





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
