#include <stm32f4xx_conf.h>
#include <stm32f4xx.h>
#include <time.h>
#include <Delay.h>
#include <adc_setup.h>

/*
 *
 * License Open
 * Author Per Bjerkreim & Tor Monstercawk Finnesand
 */

void RCC_Configuration(void){
  //RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOC, ENABLE);
}


int main(void)
{

	RCC_Configuration();
	GPIO_InitTypeDef gpioStruct;

	gpioStruct.GPIO_Mode = GPIO_Mode_OUT;
	gpioStruct.GPIO_OType = GPIO_OType_PP;
	gpioStruct.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10;
	gpioStruct.GPIO_Speed = GPIO_Speed_100MHz;

	GPIO_Init(GPIOC, &gpioStruct);

	ADC_Config();

	GPIO_WriteBit(GPIOC, GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10, Bit_SET);

	GPIO_WriteBit(GPIOC, GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10, Bit_RESET);
	while(1)
    {

		if(adc_convert() < 500)
		{
			GPIO_WriteBit(GPIOC, GPIO_Pin_10, Bit_SET);
		}else
		{
			GPIO_WriteBit(GPIOC, GPIO_Pin_10, Bit_RESET);
			GPIO_WriteBit(GPIOC, GPIO_Pin_8, Bit_RESET);
			GPIO_WriteBit(GPIOC, GPIO_Pin_9, Bit_RESET);

		}

		/* Pin_x = 0 betyr at kretsen er åpen. Pin_x = 1 betyr at kretsen er lukket */
		/* Ingen strøm på ROV */
    	//GPIO_WriteBit(GPIOC, GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10, Bit_RESET); /* Isolering */
    							    //GPIO_Pin_9 = 0; /* Oppladning */
    	//GPIO_Pin_10 = 0; /* Utladning */

    	Delay_Ms(2000);
    	/* Lader opp kondensatorer */
    	GPIO_WriteBit(GPIOC, GPIO_Pin_9, Bit_SET);
    		Delay_Ms(2000);

    	/* ROV kjører */

    		GPIO_WriteBit(GPIOC, GPIO_Pin_9, Bit_RESET);
    		GPIO_WriteBit(GPIOC, GPIO_Pin_8, Bit_SET);


    	/* Lader ut */
    	if(GPIOC->ODR == GPIO_Pin_8 & GPIOC->ODR != GPIO_Pin_9 & GPIOC->ODR != GPIO_Pin_10) {
    		Delay_Ms(1000);
    		GPIO_WriteBit(GPIOC, GPIO_Pin_10, Bit_SET);
    	}

    	/* Utladet*/
    	Delay_Ms(5000);
    	GPIO_WriteBit(GPIOC, GPIO_Pin_10, Bit_RESET);
    }
}
