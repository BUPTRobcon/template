#include "spi.h"
#include "cmd.h"
#include "global.h"
#include "param.h"
#include "can.h"

//全局变量定义区
bool g_stop_flag;
float g_vega_x;
float g_vega_y;

//--------------暂时用来草稿-------------
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
//-----------------草稿结束------------------------

void can_msg_rcv_callback(CanRxMsg *can_rx_msg);

int main(void)
{   //system_stm32f4xx.c #define PLL_M=8 PLL_N=336 HSE -> SYSCLK 168MHZ
	// RCC->CFGR |= RCC_CFGR_PPRE2_DIV2; APHB1_CLK 84MHZ
	//AHB 168MHZ  APB2 84MHZ  APB1 42MHZ
	
	delay_init(168);  //初始化延时函数
	rcc_config();
	gpio_config();
	uart_init(115200);//初始化串口波特率为115200
	SPI2_Init();
	nvic_config();
	cmd_init();
	param_init();
//	can_add_callback();
    while(1) 
	{
		controller_check();
	}
}

