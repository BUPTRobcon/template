#include "global.h"
#include "usart.h"


extern int wait_cnt;
extern s8 ptrS,ptrB;
extern bottons* b[];
extern sticks* s[];


void pitch_move(float v){
	if (v - 1200.f > 0.0000001)
		v = 1200;
	else if (v + 1200.f < 0.0000001)
		v=-1200;
	USART_SendString(USART2,"3v%d\r",(int)v);
}

void roll_move(float v){
	if (v - 1200.f > 0.0000001)
		v = 1200;
	else if (v + 1200.f < 0.0000001)
		v=-1200;
	USART_SendString(USART2,"0v%d\r",-(int)v);
}


void bottons_check(){
	if (ptrB>-1){
		b[ptrB]->cnt=20;
		b[ptrB]->ispressed=true;
		ptrB=-1;
	}
}

void sticks_check(int Hx,int Hy){
	if (ptrS>0){
		if (Hx>=0&&Hx<256&&Hy>=0&&Hy<256){
			s[ptrS-1]->x=Hx-127;
			s[ptrS-1]->y=128-Hy;}
		ptrS=0;
	}
}

//void TIM2_IRQHandler(void){
//	int i;
//	if( TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET ) 
//		{
//			if (wait_cnt>-1)
//				if (++wait_cnt==50)
//				{	USART_SendString(UART5,"Good to go\n");
//					USART_ITConfig(UART5, USART_IT_RXNE, ENABLE);wait_cnt=-1;}
//			TIM_ClearITPendingBit(TIM2,TIM_FLAG_Update);//必须清除中断标志位否则一直中断
//			for (i=0;i<12;i++) 
//			 {
//				 if (b[i]->cnt>0) b[i]->cnt--;
//				 if (b[i]->cnt==0)
//					 b[i]->ispressed=false;
//			 }
//			//USART_SendString(UART5,"%d\n",rounds*2000 + TIM4->CNT);
//		}	
//}





