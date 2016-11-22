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
	USART_Init(USART2, &USART_InitStructure); //初始化串口1
    USART_Cmd(USART2, ENABLE);  //使能串口1
	USART_Init(UART5, &USART_InitStructure); //初始化串口5
    USART_Cmd(UART5, ENABLE);  //使能串口5
	USART_Init(USART3, &USART_InitStructure); //初始化串口3
    USART_Cmd(USART3, ENABLE);  //使能串口3
	//USART_ClearFlag(USART1, USART_FLAG_TC);  	TC发送完成  1<<6
		
	USART_ITConfig(UART5, USART_IT_RXNE, ENABLE);//开启相关中断
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);

}

/******uart5用于指令接收与解析，重新定义了中断服务函数
void UART5_IRQHandler(void)                	//串口5中断服务程序
{
	u8 Res; 
	extern int pitch_flag,roll_flag;
	extern float pur_pitch,pur_roll;
	if(USART_GetITStatus(UART5, USART_IT_RXNE) != RESET)  //接收中断(接收到的数据必须是0x0d 0x0a结尾)  
       // RXNE准备好读取接收到的数据 1<<5
	{
		Res =USART_ReceiveData(UART5);//(USART1->DR);	//读取接收到的数据   中断标志已清除
		if (Res=='8') {pur_pitch=10.f;pitch_flag=1;}
		else if(Res=='2') {pur_pitch=5.f;pitch_flag=1;}
//		else if(Res=='6') USART_SendString(USART2,"0v-200\r");
//		else if(Res=='4') USART_SendString(USART2,"0v200\r");
		else {USART_SendString(USART2,"0v0\r");USART_SendString(USART2,"3v0\r");pitch_flag=0;}
//		if((UART5_RX_STA&0x8000)==0)//接收未完成   0x8000=1<<15
//		{
//			if(UART5_RX_STA&0x4000)//接收到了0x0d  0x4000=1<<14
//			{
//				if(Res!=0x0a)UART5_RX_STA=0;//接收错误,重新开始
//				else UART5_RX_STA|=0x8000;	//接收完成了 	   
//			}
//			else //还没收到0X0D
//			{	
//				if(Res==0x0d)UART5_RX_STA|=0x4000;
//				else
//				{
//					UART5_RX_BUF[UART5_RX_STA&0X3FFF]=Res ;
//					UART5_RX_STA++;
//					if(UART5_RX_STA>(USART_REC_LEN-1))UART5_RX_STA=0;//接收数据错误,重新开始接收	  
//				}		 
//			}
//		}   		 
     } 
} */

void USART3_IRQHandler(void)                	
{
	u8 Res;	
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET) 
	{
		Res =USART_ReceiveData(USART3);
		if (Res=='I'||Res=='J'||Res=='K'||Res=='L'||
			Res=='A'||Res=='B'||Res=='C'||Res=='D'||
		    Res=='F'||Res=='N')  
		{       
         cmd=Res;
		 return;}   
		if (Res=='P') {ptr=0;return;}
		else if (Res=='W') {ptr=1;return;}
		else if (Res=='S') {ptr=2;return;}
		else if (Res=='Q') {ptr=3;return;}
		if (Res==0x0a||Res==0x0d) return;
		if (sticks[ptr]==0)
			sticks[ptr]=sticks[ptr]*10+Res-32;
//		if((USART3_RX_STA&0x8000)==0
//		{
//			if(USART3_RX_STA&0x4000)
//			{
//				if(Res!=0x0a)USART3_RX_STA=0;
//				else {   
//					     
//				     }	
//			}
//			else
//			{	
//				if(Res==0x0d)USART3_RX_STA|=0x4000;
//				else
//				{
//					USART3_RX_BUF[USART3_RX_STA&0X3FFF]=Res ;
//					USART3_RX_STA++;
//					if(USART3_RX_STA>(USART_REC_LEN-1))USART3_RX_STA=0;
//				}		 
//			}
//		}   		 
     } 
} 






 
