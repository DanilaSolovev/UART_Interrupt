
#include "stdint.h"
#include "stm32f101xb.h"
#include "math.h"
#define GPIO_PIN_5                 ((uint16_t)0x0020)  /* Pin 5 selected    */
#define Perv                       ((uint8_t)0x00)
#define Posl                       ((uint8_t)0x00)
uint8_t data = 0;
uint8_t res_mas[10] = {0};
uint32_t i = 0;
void UARTResive_mas(uint8_t usartnum, uint32_t n);
uint8_t UARTResive(uint8_t usartnum);
uint8_t a=0;
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

typedef struct
{
  uint16_t CR1;
  uint16_t  RESERVED0;
  uint16_t CR2;
  uint16_t  RESERVED1;
  uint16_t SMCR;
  uint16_t  RESERVED2;
  uint16_t DIER;
  uint16_t  RESERVED3;
  uint16_t SR;
  uint16_t  RESERVED4;
  uint16_t EGR;
  uint16_t  RESERVED5;
  uint16_t CCMR1;
  uint16_t  RESERVED6;
  uint16_t CCMR2;
  uint16_t  RESERVED7;
  uint16_t CCER;
  uint16_t  RESERVED8;
  uint16_t CNT;
  uint16_t  RESERVED9;
  uint16_t PSC;
  uint16_t  RESERVED10;
  uint16_t ARR;
  uint16_t  RESERVED11;
  uint16_t RCR;
  uint16_t  RESERVED12;
  uint16_t CCR1;
  uint16_t  RESERVED13;
  uint16_t CCR2;
  uint16_t  RESERVED14;
  uint16_t CCR3;
  uint16_t  RESERVED15;
  uint16_t CCR4;
  uint16_t  RESERVED16;
  uint16_t BDTR;
  uint16_t  RESERVED17;
  uint16_t DCR;
  uint16_t  RESERVED18;
  uint16_t DMAR;
  uint16_t  RESERVED19;
} TIM1_TypeDef;

#define TIM1_BASE       0x40012C00UL

#define TIM1 ((TIM1_TypeDef*)TIM1_BASE)

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
//Прием массива по USART
void UARTResive_mas(uint8_t usartnum, uint32_t n)
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
}
uint8_t UARTResive(uint8_t usartnum)
{
    uint8_t Resive=0;
    if (usartnum==1)
    {
        if (((USART1->SR)& (1<<5))!= 0x00)
        {
            Resive = USART1->DR;
        }
    }
    if (usartnum==2)
    {
        if (((USART2->SR)& (1<<5))!= 0x00)
        {
            Resive = USART2->DR;
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
        static int8_t state=Perv;
        if (state==Perv){TIM1->CR1 |= 0x01; state=Posl;} 
        res_mas[i]=UARTResive(2);
        i++;
        if (i==10) {i=0;TIM1->CR1 &= ~0x01;}
}
void USART1_IRQHandler (void)
{
        data = 0x00;
        data = UARTResive(1);
        if (data !=0){res_mas[i]=data;}
        i++;
        if (i==10) {i=0;}
}
void TIM1_UP_IRQHandler (void)
    {
        if (TIM1->SR & 0x01)
        {
            if (a==0){
                a=1;
                GPIOA->BSRR = GPIOA->BSRR | GPIO_PIN_5;
            }
            else 
            {
                a=0;
                GPIOA->BRR = GPIOA->BRR | GPIO_PIN_5;
            }
            
        }
            memset(res_mas,0,sizeof(res_mas));
            i=0;
            TIM1->CR1 &= ~0x01;
            TIM1->SR = ~0x01;
        
    }
