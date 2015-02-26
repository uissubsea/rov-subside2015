/*
 * main.c
 *
 *  Created on: Feb 10, 2015
 *      Author: tmf
 */


#include "stm32f4xx_conf.h"
#include "stm32f4xx.h"

GPIO_InitTypeDef  GPIO_InitStructure;


int main(void) {
  /* GPIOD Periph clock enable */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

  /* Configure PD12, 13, 14 and PD15 in output pushpull mode */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOD, &GPIO_InitStructure);

  if (SysTick_Config(SystemCoreClock / 1000))
    {
      /* Capture error */
      while (1);
    }

  while (1) {
    ;
  }
}

#ifdef USE_FULL_ASSERT
void assert_failed ( uint8_t * file , uint32_t line)
{
/* Infinite loop */
/* Use GDB to find out why we're here */
while (1);
}
#endif
