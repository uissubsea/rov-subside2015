#include <stm32f10x_conf.h>

TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
TIM_OCInitTypeDef  TIM_OCInitStructure;

uint16_t CCR1_Val = 333;
uint16_t CCR2_Val = 249;
uint16_t CCR3_Val = 166;
uint16_t CCR4_Val = 83;

void RCC_Configuration(void)
{
  /* TIM3 clock enable */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

  /* GPIOA and GPIOB clock enable */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB |
                         RCC_APB2Periph_GPIOC | RCC_APB2Periph_AFIO, ENABLE);
}

void GPIO_Configuration(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  /*GPIOC Configuration: TIM3 channel1, 2, 3 and 4 */
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

  GPIO_Init(GPIOC, &GPIO_InitStructure);

  GPIO_PinRemapConfig(GPIO_FullRemap_TIM3, ENABLE);
}

int main(void)
{

  RCC_Configuration();
  GPIO_Configuration();

   TIM_TimeBaseStructure.TIM_Period = 665;
   TIM_TimeBaseStructure.TIM_Prescaler = 1;
   TIM_TimeBaseStructure.TIM_ClockDivision = 0;
   TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

   TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

   /* PWM1 Mode configuration: Channel1 */
     TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
     TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
     TIM_OCInitStructure.TIM_Pulse = CCR1_Val;
     TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;

     TIM_OC1Init(TIM3, &TIM_OCInitStructure);

     TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);

     /* PWM1 Mode configuration: Channel2 */
     TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
     TIM_OCInitStructure.TIM_Pulse = CCR2_Val;

     TIM_OC2Init(TIM3, &TIM_OCInitStructure);

     TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);

     /* PWM1 Mode configuration: Channel3 */
     TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
     TIM_OCInitStructure.TIM_Pulse = CCR3_Val;

     TIM_OC3Init(TIM3, &TIM_OCInitStructure);

     TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);

     /* PWM1 Mode configuration: Channel4 */
     TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
     TIM_OCInitStructure.TIM_Pulse = CCR4_Val;

     TIM_OC4Init(TIM3, &TIM_OCInitStructure);

     TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);

     TIM_ARRPreloadConfig(TIM3, ENABLE);

     /* TIM3 enable counter */
     TIM_Cmd(TIM3, ENABLE);

     while(1)
       {

       }


}
