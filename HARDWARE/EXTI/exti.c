#include "exti.h"


int wait_cnt=-1;

void EXTI15_10_IRQHandler(void)
{
	delay_ms(10);
	if(EXTI_GetITStatus(EXTI_Line11)!=RESET){//判断某个线上的中断是否发生
		if (GPIO_ReadInputDataBit(GPIOF,GPIO_Pin_11)==0){ 
		
			wait_cnt=0;
			USART_SendString(UART5,"Pitch STOP!");
			USART_SendString(USART2,"0v0\r");USART_SendString(USART2,"3v0\r");
			USART_ITConfig(UART5, USART_IT_RXNE, DISABLE);
			EXTI_ClearITPendingBit(EXTI_Line11); //清除 LINE 上的中断标志位
		}
	}else
	if(EXTI_GetITStatus(EXTI_Line14)!=RESET){//判断某个线上的中断是否发生
		if (GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_14)==0)
		{ 
		
			wait_cnt=0;
			USART_SendString(UART5,"Roll STOP!");
			USART_SendString(USART2,"0v0\r");USART_SendString(USART2,"3v0\r");
			USART_ITConfig(UART5, USART_IT_RXNE, DISABLE);
			EXTI_ClearITPendingBit(EXTI_Line14); //清除 LINE 上的中断标志位
		}
	} 
}
