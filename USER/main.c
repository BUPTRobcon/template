#include "spi.h"
#include "cmd.h"
#include "global.h"
#include "param.h"
#include "can.h"
#include "TIM.h"
#include "math.h"

extern u8 cmd,sticks[4];
extern u8 ptr;
extern int wait_cnt;

//全局变量定义区
bool g_stop_flag;
float g_vega_x;
float g_vega_y;

//--------------暂时用来草稿-------------

float d_pitch=0,dd_pitch=0,last_d_pitch=0;
float now_pitch,pur_pitch;
int pitch_cnt=0;

float d_roll=0,dd_roll=0,last_d_roll=0;
float now_roll,pur_roll;
int roll_cnt=0;

int pitch_flag=0,roll_flag=0;

extern int TIM3_round,TIM4_round;
typedef struct 
{
	bool ispressed;
	u8 cnt;
//  void * func;
}bottons;

bottons up,down,left,right;

bottons* b[4]={&up,&down,&left,&right};


void controller_check(void){
	switch(cmd){
		case 'A':
			up.cnt=10;
			up.ispressed=true;
			break;
		case 'B':
			right.cnt=10;
			right.ispressed=true;
			break;
		case 'C':
			down.cnt=10;
			down.ispressed=true;
			break;
		case 'D':
			left.cnt=10;
			left.ispressed=true;
			break;
	}
		
}

void TIM2_IRQHandler(void){
	u8 i;
	if( TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET ) 
		{
			if (wait_cnt>-1)
				if (++wait_cnt==50)
				{	USART_SendString(UART5,"Good to go\n");
					USART_ITConfig(UART5, USART_IT_RXNE, ENABLE);wait_cnt=-1;}
			TIM_ClearITPendingBit(TIM2,TIM_FLAG_Update);//必须清除中断标志位否则一直中断
			for (i=0;i<4;i++) 
			 {
				 b[i]->cnt--;
				 if (b[i]->cnt==0)
					 b[i]->ispressed=false;
			 }
			//USART_SendString(UART5,"%d\n",rounds*2000 + TIM4->CNT);
		}	
}
void pitch_move(float v){
	if (v - 600.f > 0.0000001)
		v = 600;
	else if (v + 600.f < 0.0000001)
		v=-600;
	USART_SendString(USART2,"3v%d\r",(int)v);
}

void roll_move(float v){
	if (v - 600.f > 0.0000001)
		v = 600;
	else if (v + 600.f < 0.0000001)
		v=-600;
	USART_SendString(USART2,"0v%d\r",-(int)v);
}



//-----------------草稿结束------------------------

void can_msg_rcv_callback(CanRxMsg *can_rx_msg);

int main(void)
{   //system_stm32f4xx.c #define PLL_M=8 PLL_N=336 HSE -> SYSCLK 168MHZ
	// RCC->CFGR |= RCC_CFGR_PPRE2_DIV2; APHB1_CLK 84MHZ
	//AHB 168MHZ  APB2 84MHZ  APB1 42MHZ
	int TIM3_display=0,TIM4_display=0;
	
	delay_init(168);  //初始化延时函数
	rcc_config();
	gpio_config();
	EXTI_config();
	nvic_config();
	TIM2_Init();
	TIM3_Init();
	TIM4_Init();
	TIM8_Init();
//	SPI2_Init();
	uart_init(115200);//初始化串口波特率为115200
	SPI2_Init();
	cmd_init();
	param_init();
//	can_add_callback();
    while(1) 
	{
		if (pitch_flag==1){
			now_pitch = (TIM4_round * 30000.f - TIM4->CNT)/10000.f;
			d_pitch = pur_pitch - now_pitch;
			dd_pitch = d_pitch - last_d_pitch;
			last_d_pitch = d_pitch;
			if (fabs(d_pitch)<0.01){
				if (pitch_cnt > 10){
					pitch_flag=0;
					pitch_move(0);
				}else{
					pitch_cnt++;
					pitch_move(d_pitch * 300 + dd_pitch * 240);
				}
			}else{
				pitch_cnt=0;
				pitch_move(d_pitch * 300 + dd_pitch * 240);
			}
		}
		if (roll_flag==1){
			now_roll = (TIM3_round * 30000.f - TIM3->CNT)/10000.f;
			d_roll = pur_roll - now_roll;
			dd_roll = d_roll - last_d_roll;
			last_d_roll = d_roll;
			if (fabs(d_roll)<0.01){
				if (roll_cnt > 10){
					roll_flag=0;
					roll_move(0);
				}else{
					roll_cnt++;
					roll_move(d_roll * 300 + dd_roll * 240);
				}
			}else{
				roll_cnt=0;
				roll_move(d_roll * 300 + dd_roll * 240);
			}
		}
		controller_check();
		if (TIM3_round*30000-TIM3->CNT!=TIM3_display){
			TIM3_display=TIM3_round*30000-TIM3->CNT;
			USART_SendString(UART5,"TIM3:%d\n",TIM3_display);}
		if (TIM4_round*30000-TIM4->CNT!=TIM4_display){
			TIM4_display=TIM4_round*30000-TIM4->CNT;
			USART_SendString(UART5,"TIM4:%d\n",TIM4_display);}		
//			USART_SendString(UART5,"TIM3:%d\n",TIM3_round*30000+TIM3->CNT);
//			USART_SendString(UART5,"TIM4:%d\n",TIM4_round*30000+TIM4->CNT);
	}
}

