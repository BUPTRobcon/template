#include "TIM.h"

TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
TIM_OCInitTypeDef  TIM_OCInitStructure;
TIM_ICInitTypeDef   TIM_ICInitStructure;

int TIM3_round,TIM4_round;

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
	TIM_ICInitStructure.TIM_ICFilter = 10;
	TIM_ICInit(TIM4, &TIM_ICInitStructure);

	TIM4->CNT=0;
	TIM4_round=0;

	TIM_ClearFlag(TIM4, TIM_FLAG_Update);
	TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);
	TIM_Cmd(TIM4, ENABLE);	
}

void TIM3_Init(){
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

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

	TIM3->CNT=0;
	TIM3_round=0;

	TIM_ClearFlag(TIM3, TIM_FLAG_Update);
	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
	TIM_Cmd(TIM3, ENABLE);	
}

void TIM2_Init()        //0.005ms 定时
{
	/*TIM2*/
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
}

void TIM8_Init()  //无刷PWM输出
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8, ENABLE);
	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
	TIM_TimeBaseStructure.TIM_Prescaler = 168 - 1;  //    84M/84 =1M  1us/count
	TIM_TimeBaseStructure.TIM_Period = 20000 - 1;  
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;  //设置了时钟分割
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;// 向上  
	TIM_TimeBaseInit(TIM8, &TIM_TimeBaseStructure);
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //选择定时器模式:TIM脉冲宽度调制模式1
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputState_Disable;
	TIM_OCInitStructure.TIM_Pulse = 7.7/100*1000000/50 - 1;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性
	TIM_OC1Init(TIM8, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM8 OC1
    TIM_OC1PreloadConfig(TIM8, TIM_OCPreload_Enable);  //使能TIM3在CCR1上的预装载寄存器
    TIM_ARRPreloadConfig(TIM8,ENABLE);//ARPE使能 
	
	TIM_Cmd(TIM8, ENABLE);
	TIM_CtrlPWMOutputs(TIM8, ENABLE);
}

void TIM3_IRQHandler(){
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET){
		if (TIM3->CNT==0) TIM3_round-=1;
		else if (TIM3->CNT==TIM3->ARR) TIM3_round+=1;
		TIM_ClearITPendingBit(TIM3,TIM_FLAG_Update);
	}
}
void TIM4_IRQHandler(){
	if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET){
		if (TIM4->CNT==0) TIM4_round-=1;
		else if (TIM4->CNT==TIM4->ARR) TIM4_round+=1;
		TIM_ClearITPendingBit(TIM4,TIM_FLAG_Update);
	}
}


