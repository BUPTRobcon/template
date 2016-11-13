#include "usart.h"	

//初始化IO 串口1 
//bound:波特率
void uart_init(u32 bound){
   //GPIO端口设置
	USART_InitTypeDef USART_InitStructure;
	
//////////////////////////////////////////////////////////////////////////////////////////	
	
	USART_InitStructure.USART_BaudRate = bound;//波特率设置
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
   
	USART_Init(USART1, &USART_InitStructure); //初始化串口1
    USART_Cmd(USART1, ENABLE);  //使能串口1
	USART_Init(UART5, &USART_InitStructure); //初始化串口5
    USART_Cmd(UART5, ENABLE);  //使能串口5
//	USART_Init(USART3, &USART_InitStructure); //初始化串口3
//    USART_Cmd(USART3, ENABLE);  //使能串口3
	//USART_ClearFlag(USART1, USART_FLAG_TC);  	TC发送完成  1<<6
		
	USART_ITConfig(UART5, USART_IT_RXNE, ENABLE);//开启相关中断
//	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	
//	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;		
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;

}



//void UART5_IRQHandler(void)                	//串口5中断服务程序
//{
//	u8 Res; u8 len;
//	uint32_t ccr1,ccr2;
//#if SYSTEM_SUPPORT_OS 		//如果SYSTEM_SUPPORT_OS为真，则需要支持OS.
//	OSIntEnter();    
//#endif
//	if(USART_GetITStatus(UART5, USART_IT_RXNE) != RESET)  //接收中断(接收到的数据必须是0x0d 0x0a结尾)  
//       // RXNE准备好读取接收到的数据 1<<5
//	{
//		Res =USART_ReceiveData(UART5);//(USART1->DR);	//读取接收到的数据   中断标志已清除
////		ccr2=TIM3->CCR2+1;
////		ccr1=TIM3->CCR1+1;
////		len=USART_RX_STA&0X3FFF;
////		if (len==0) {
////		 if (Res=='I')  {TIM_SetCompare2(TIM3,18800-1);return;}
////		 else if (Res=='J') {TIM_SetCompare2(TIM3,19300-1);return;}
////		 else if (Res=='K') {TIM_SetCompare1(TIM3,18800-1);return;}
////		 else if (Res=='L') {TIM_SetCompare1(TIM3,19200-1);return;}
////	    }
//		len=USART_RX_STA&0x3fff;
//		if(len==1){
//		if (USART_RX_BUF[0]=='I')  {TIM_SetCompare2(TIM3,18800-1);USART_RX_STA=0;return;}
//		else if (USART_RX_BUF[0]=='J') {TIM_SetCompare2(TIM3,19420-1);USART_RX_STA=0;return;}
//		else if (USART_RX_BUF[0]=='K') {TIM_SetCompare1(TIM3,18800-1);USART_RX_STA=0;return;}
//		else if (USART_RX_BUF[0]=='L') {TIM_SetCompare1(TIM3,19200-1);USART_RX_STA=0;return;} }
//		if((USART_RX_STA&0x8000)==0)//接收未完成   0x8000=1<<15
//		{
//			if(USART_RX_STA&0x4000)//接收到了0x0d  0x4000=1<<14
//			{
//				if(Res!=0x0a)USART_RX_STA=0;//接收错误,重新开始
//				else { //   USART_RX_STA|=0x8000;
////					      len=USART_RX_STA&0x3fff;
//					      controller(len);
//			              USART_RX_STA=0;
//				     }	//接收完成了     
//					 //USART_ClearITPendingBit(USART1,USART_IT_RXNE)  清除中断标志位？？
////       *	@note   RXNE pending bit can be also cleared by a read to the USART_DR register 
////            (USART_ReceiveData()).
////				     {
////				           len=USART_RX_STA&0x3fff;
////				           if (len>3) check(len); else rFail();
////					   }
//			}
//			else //还没收到0X0D
//			{	
//				if(Res==0x0d)USART_RX_STA|=0x4000;
//				else
//				{
//					USART_RX_BUF[USART_RX_STA&0X3FFF]=Res ;
//					USART_RX_STA++;
//					if(USART_RX_STA>(USART_REC_LEN-1))USART_RX_STA=0;//接收数据错误,重新开始接收	  
//				}		 
//			}
//		}   		 
//     } 
//#if SYSTEM_SUPPORT_OS 	//如果SYSTEM_SUPPORT_OS为真，则需要支持OS.
//	OSIntExit();  											 
//#endif
//} 
void UART5_IRQHandler(void)                	//串口5中断服务程序
{
	u8 Res; 
	if(USART_GetITStatus(UART5, USART_IT_RXNE) != RESET)  //接收中断(接收到的数据必须是0x0d 0x0a结尾)  
       // RXNE准备好读取接收到的数据 1<<5
	{
		Res =USART_ReceiveData(UART5);//(USART1->DR);	//读取接收到的数据   中断标志已清除
		
		if((UART5_RX_STA&0x8000)==0)//接收未完成   0x8000=1<<15
		{
			if(UART5_RX_STA&0x4000)//接收到了0x0d  0x4000=1<<14
			{
				if(Res!=0x0a)UART5_RX_STA=0;//接收错误,重新开始
				else UART5_RX_STA|=0x8000;	//接收完成了 	   
			}
			else //还没收到0X0D
			{	
				if(Res==0x0d)UART5_RX_STA|=0x4000;
				else
				{
					UART5_RX_BUF[UART5_RX_STA&0X3FFF]=Res ;
					UART5_RX_STA++;
					if(UART5_RX_STA>(USART_REC_LEN-1))UART5_RX_STA=0;//接收数据错误,重新开始接收	  
				}		 
			}
		}   		 
     } 
} 


//void USART1_IRQHandler(void)                	//串口5中断服务程序
//{
//	u8 Res; u8 len;
//	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //接收中断(接收到的数据必须是0x0d 0x0a结尾)  
//       // RXNE准备好读取接收到的数据 1<<5
//	{
//		Res =USART_ReceiveData(USART1);//(USART1->DR);	//读取接收到的数据   中断标志已清除
//		
//		if((USART1_RX_STA&0x8000)==0)//接收未完成   0x8000=1<<15
//		{
//			if(USART1_RX_STA&0x4000)//接收到了0x0d  0x4000=1<<14
//			{
//				if(Res!=0x0a)USART1_RX_STA=0;//接收错误,重新开始
//				else USART1_RX_STA|=0x8000;	//接收完成了 	   
//			}
//			else //还没收到0X0D
//			{	
//				if(Res==0x0d)USART1_RX_STA|=0x4000;
//				else
//				{
//					USART1_RX_BUF[USART1_RX_STA&0X3FFF]=Res ;
//					USART1_RX_STA++;
//					if(USART1_RX_STA>(USART_REC_LEN-1))USART1_RX_STA=0;//接收数据错误,重新开始接收	  
//				}		 
//			}
//		}   		 
//     } 
//}



 
