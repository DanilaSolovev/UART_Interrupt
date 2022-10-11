
#include "stdint.h"
#include "stm32f101xb.h"
#include "math.h"
uint8_t data = 0;
uint32_t res_mas[10] = {0};
uint32_t i = 0;
uint8_t UARTResive(uint8_t usartnum, uint32_t n);
void UARTcnf(uint8_t usartnum, uint32_t brate, uint8_t prer);
void UARTSend(int8_t c);
void UARTSendMAS(uint8_t usartnum, uint32_t n);


/* Регистры контроллера прерываний*/
typedef struct
{
   uint32_t ISER[8U];               /*!< Offset: 0x000 (R/W)  Interrupt Set Enable Register */
        uint32_t RESERVED0[24U];
   uint32_t ICER[8U];               /*!< Offset: 0x080 (R/W)  Interrupt Clear Enable Register */
        uint32_t RESERVED1[24U];
   uint32_t ISPR[8U];               /*!< Offset: 0x100 (R/W)  Interrupt Set Pending Register */
        uint32_t RESERVED2[24U];
   uint32_t ICPR[8U];               /*!< Offset: 0x180 (R/W)  Interrupt Clear Pending Register */
        uint32_t RESERVED3[24U];
   uint32_t IABR[8U];               /*!< Offset: 0x200 (R/W)  Interrupt Active bit Register */
        uint32_t RESERVED4[56U];
   uint8_t  IP[240U];               /*!< Offset: 0x300 (R/W)  Interrupt Priority Register (8Bit wide) */
        uint32_t RESERVED5[644U];
    uint32_t STIR;                  /*!< Offset: 0xE00 ( /W)  Software Trigger Interrupt Register */
}  NVIC_Type;

#define NVIC_BASE		0xE000E100UL				/*!< NVIC Base Address */

#define NVIC	((NVIC_Type*)NVIC_BASE)   /*!< NVIC configuration struct */

typedef struct
{
    uint32_t SR;
    uint32_t DR;
    uint32_t BRR;
    uint32_t CR1;
    uint32_t CR2;
    uint32_t CR3;
    uint32_t GTPR;
}USART2_Type;

typedef struct
{
    uint32_t SR;
    uint32_t DR;
    uint32_t BRR;
    uint32_t CR1;
    uint32_t CR2;
    uint32_t CR3;
    uint32_t GTPR;
}USART1_Type;

#define USART2_BASE     0x40004400UL

#define USART2 ((USART2_Type*)USART2_BASE)

#define USART1_BASE     0x40013800UL

#define USART1 ((USART1_Type*)USART1_BASE)
//Инициализация USART
void UARTcnf(uint8_t usartnum, uint32_t brate, uint8_t prer)
{
    //Расчет baudrate
    float div = ((8000000*100)/(brate*16));
    div = div/100;
    volatile float frac;
    float mant;
    frac=16*modff(div,&mant);
    frac = roundf(frac);
    volatile uint32_t frac1 = (uint32_t) frac;
    volatile uint32_t mant1 = (uint32_t) mant;
    
    
    
    RCC->APB2ENR = RCC->APB2ENR | RCC_APB2ENR_IOPAEN_Msk;
    switch (usartnum)
    {
        case 0x02:
        RCC->APB1ENR |= (1<<17);
        GPIOA->CRL =0;
        GPIOA->CRL |= (3<<8);
        GPIOA->CRL |= (2<<10);
        
        GPIOA->CRL &= ~(3<<12);
        GPIOA->CRL |= (2<<14);
        
        GPIOA->ODR |= 1<<3;
        
        USART2->CR1 |= 0x00;
        USART2->CR1 |= (1<<13);
        USART2->CR1 &= ~(1<<12);
        USART2->BRR |=frac1;
        USART2->BRR |=(mant1<<4);
        
        
        USART2->CR1 |= (1<<2);
        USART2->CR1 |= (1<<3);
        if (prer==1)
            {
            /* Разрешение прерываний */
            NVIC->ISER[1] = (uint32_t)(1UL << (((uint32_t)USART2_IRQn ) & 0x1FUL));
            USART2->CR1 |= (1<<5); /*Прерывание при приеме данных*/
            }
        break;
        case 0x01:
        RCC->APB2ENR |= (1<<14);
        GPIOA->CRH = 0;
        GPIOA->CRH |= (3<<4);
        GPIOA->CRH |= (2<<6);
        
        GPIOA->CRH &= ~(3<<8);
        GPIOA->CRH |= (2<<10);
        
        GPIOA->ODR |= 1<<10;
        
        USART1->CR1 |= 0x00;
        USART1->CR1 |= (1<<13);
        USART1->CR1 &= ~(1<<12);
        USART1->BRR |=frac1;
        USART1->BRR |=(mant1<<4);
        
        USART1->CR1 |= (1<<2);
        USART1->CR1 |= (1<<3);
        if (prer==1)
        {
            /* Разрешение прерываний */
            NVIC->ISER[1] = (uint32_t)(1UL << (((uint32_t)USART1_IRQn ) & 0x1FUL));
            USART1->CR1 |= (1<<5); /*Прерывание при приеме данных*/
        }
        break;
        default:
        break;
    }
}
//Прием данных по USART
uint8_t UARTResive(uint8_t usartnum, uint32_t n)
{
    uint8_t Resive=0;
    volatile uint32_t a = 0;
    i = 0;
    memset(res_mas,0,sizeof(res_mas));
    if (usartnum==1)
    {
        while(a<1000000 && i!=n)
//        while(i!=n)
        {
            if (((USART1->SR)& (1<<5))!= 0x00)
            {
                Resive = USART1->DR;
                res_mas[i]=Resive;
                i++;
            }
        a++;
        }
    }
    if (usartnum==2)
    {
        while(a<1000000 && i!=n)
//        while(i!=n)
        {
            if (((USART2->SR)& (1<<5))!= 0x00)
            {
                Resive = USART2->DR;
                res_mas[i]=Resive;
                i++;
            }
        a++;
        }
    }
	return Resive;
}
//Отправка символа по USART
void UARTSend(int8_t c)
{
    USART2->DR = c;
	while (!(USART2->SR & (1<<6)));
}
//Отправка строки по USART
void UARTSendMAS(uint8_t usartnum, uint32_t n)
{
    i = 0;
    while (i!=n)
    {
        UARTSend(res_mas[i]);
        i++;
    }
}
//Обработчик прерываний USART
void USART2_IRQHandler (void)
{
        data = 0x00;
        data = UARTResive(2,5);
        if (data !=0){res_mas[i]=data;}
        i++;
        if (i==10) {i=0;}
}
void USART1_IRQHandler (void)
{
        data = 0x00;
        data = UARTResive(1,5);
        if (data !=0){res_mas[i]=data;}
        i++;
        if (i==10) {i=0;}
}
