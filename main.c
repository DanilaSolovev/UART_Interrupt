
#include "main.h"


/*Определение прототипов функциий*/
void delay (uint32_t ticks);

// Определение переменных
uint32_t spd = 0;

int main(void)
{

	/*Инициализация UART*/
    /*UARTcnf(Номер UART(1-2), Baudrate, Прерывания(0-1))*/
	UARTcnf(2,14400,0);
    
    while(1)
    {
       uint8_t data = UARTResive(2,5);
//       spd = res_mas[0]+res_mas[1]+res_mas[2];
        
        
        
       UARTSendMAS(2,5);
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
