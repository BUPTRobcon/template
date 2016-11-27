#include "TIM.h"

TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
TIM_OCInitTypeDef  TIM_OCInitStructure;
TIM_ICInitTypeDef   TIM_ICInitStructure;

void TIM4_Init(){
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);

	TIM_DeInit(TIM4);
   
	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
	TIM_TimeBaseStructure.TIM_Prescaler = 0x0;  // No prescaling 42M
	TIM_TimeBaseStructure.TIM_Period = 29999;  
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;  //设置了时钟分割
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;// 向上  
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
 
	TIM_EncoderInterfaceConfig(TIM4,TIM_EncoderMode_TI12,TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
	TIM_ICStructInit(&TIM_ICInitStructure);
	TIM_ICInitStructure.TIM_ICFilter = 6;
	TIM_ICInit(TIM4, &TIM_ICInitStructure);

//  TIM4->CNT=4000;

	TIM_ClearFlag(TIM4, TIM_FLAG_Update);
	TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);
	TIM_Cmd(TIM4, ENABLE);	
}

void TIM3_Init(){
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);

	TIM_DeInit(TIM3);
   
	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
	TIM_TimeBaseStructure.TIM_Prescaler = 0x0;  // No prescaling 42M
	TIM_TimeBaseStructure.TIM_Period = 29999;  
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;  //设置了时钟分割
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;// 向上  
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
 
	TIM_EncoderInterfaceConfig(TIM3,TIM_EncoderMode_TI12,TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
	TIM_ICStructInit(&TIM_ICInitStructure);
	TIM_ICInitStructure.TIM_ICFilter = 6;
	TIM_ICInit(TIM3, &TIM_ICInitStructure);

//  TIM4->CNT=4000;

	TIM_ClearFlag(TIM3, TIM_FLAG_Update);
	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
	TIM_Cmd(TIM3, ENABLE);	
}
void TIM2_Configuration()        //0.005ms 定时
{
	/*TIM2*/
	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	TIM_DeInit(TIM2);
	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
	TIM_TimeBaseStructure.TIM_Prescaler = 42000 - 1;  //    84M/42000 =2000  0.5ms/count
	TIM_TimeBaseStructure.TIM_Period = 9;  
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;  //设置了时钟分割
    //计时器的时钟周期为 (4+1)*(71+1)/(72*10^6)=0.005ms   ?
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;// 向上  
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
	TIM_ClearFlag(TIM2, TIM_FLAG_Update);
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
	TIM_Cmd(TIM2, ENABLE);	
	NVIC_InitStructure.NVIC_IRQChannel=TIM2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority =1;//1
    NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;//0
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}





