#include "spi.h"
#include "global.h"
#include "can.h"

//--------------��ʱ�����ݸ�-------------
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
			TIM_ClearITPendingBit(TIM2,TIM_FLAG_Update);//��������жϱ�־λ����һֱ�ж�
			for (i=0;i<4;i++) 
			 {
				 b[i]->cnt--;
				 if (b[i]->cnt==0)
					 b[i]->ispressed=false;
			 }
			//USART_SendString(UART5,"%d\n",rounds*2000 + TIM4->CNT);
		}	
}
//-----------------�ݸ����------------------------

void can_msg_rcv_callback(CanRxMsg *can_rx_msg);

int main(void)
{   //system_stm32f4xx.c #define PLL_M=8 PLL_N=336 HSE -> SYSCLK 168MHZ
	// RCC->CFGR |= RCC_CFGR_PPRE2_DIV2; APHB1_CLK 84MHZ
	//AHB 168MHZ  APB2 84MHZ  APB1 42MHZ
	int i;	

	delay_init(168);  //��ʼ����ʱ����
	rcc_config();
    gpio_config();	
	nvic_config();
	uart_init(115200);//��ʼ�����ڲ�����Ϊ9600

	SPI2_Init();
//	can_add_callback();
    while(1) 
	{
		controller_check();
		if (USART1_RX_STA&0x8000){
			USART_SendString(UART5,"->%s\n",USART1_RX_BUF);
			for (i=0;i<(USART1_RX_STA&0x3fff);i++)
				USART1_RX_BUF[i]=0;
			USART1_RX_STA=0;
		}
		if (UART5_RX_STA&0x8000){
			USART_SendString(UART5,"\n:%s\n",UART5_RX_BUF);
			USART_SendString(USART1,"%s\r",UART5_RX_BUF);
			for (i=0;i<(UART5_RX_STA&0x3fff);i++)
				UART5_RX_BUF[i]=0;
			UART5_RX_STA=0;
		}
	}
}

