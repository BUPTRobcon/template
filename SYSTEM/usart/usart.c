#include "usart.h"	

//��ʼ��IO ����1 
//bound:������
void uart_init(u32 bound){
   //GPIO�˿�����
	USART_InitTypeDef USART_InitStructure;
	
//////////////////////////////////////////////////////////////////////////////////////////	
	
	USART_InitStructure.USART_BaudRate = bound;//����������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
   
	USART_Init(USART1, &USART_InitStructure); //��ʼ������1
    USART_Cmd(USART1, ENABLE);  //ʹ�ܴ���1
	USART_Init(UART5, &USART_InitStructure); //��ʼ������5
    USART_Cmd(UART5, ENABLE);  //ʹ�ܴ���5
//	USART_Init(USART3, &USART_InitStructure); //��ʼ������3
//    USART_Cmd(USART3, ENABLE);  //ʹ�ܴ���3
	//USART_ClearFlag(USART1, USART_FLAG_TC);  	TC�������  1<<6
		
	USART_ITConfig(UART5, USART_IT_RXNE, ENABLE);//��������ж�
//	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	
//	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;		
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;

}



//void UART5_IRQHandler(void)                	//����5�жϷ������
//{
//	u8 Res; u8 len;
//	uint32_t ccr1,ccr2;
//#if SYSTEM_SUPPORT_OS 		//���SYSTEM_SUPPORT_OSΪ�棬����Ҫ֧��OS.
//	OSIntEnter();    
//#endif
//	if(USART_GetITStatus(UART5, USART_IT_RXNE) != RESET)  //�����ж�(���յ������ݱ�����0x0d 0x0a��β)  
//       // RXNE׼���ö�ȡ���յ������� 1<<5
//	{
//		Res =USART_ReceiveData(UART5);//(USART1->DR);	//��ȡ���յ�������   �жϱ�־�����
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
//		if((USART_RX_STA&0x8000)==0)//����δ���   0x8000=1<<15
//		{
//			if(USART_RX_STA&0x4000)//���յ���0x0d  0x4000=1<<14
//			{
//				if(Res!=0x0a)USART_RX_STA=0;//���մ���,���¿�ʼ
//				else { //   USART_RX_STA|=0x8000;
////					      len=USART_RX_STA&0x3fff;
//					      controller(len);
//			              USART_RX_STA=0;
//				     }	//���������     
//					 //USART_ClearITPendingBit(USART1,USART_IT_RXNE)  ����жϱ�־λ����
////       *	@note   RXNE pending bit can be also cleared by a read to the USART_DR register 
////            (USART_ReceiveData()).
////				     {
////				           len=USART_RX_STA&0x3fff;
////				           if (len>3) check(len); else rFail();
////					   }
//			}
//			else //��û�յ�0X0D
//			{	
//				if(Res==0x0d)USART_RX_STA|=0x4000;
//				else
//				{
//					USART_RX_BUF[USART_RX_STA&0X3FFF]=Res ;
//					USART_RX_STA++;
//					if(USART_RX_STA>(USART_REC_LEN-1))USART_RX_STA=0;//�������ݴ���,���¿�ʼ����	  
//				}		 
//			}
//		}   		 
//     } 
//#if SYSTEM_SUPPORT_OS 	//���SYSTEM_SUPPORT_OSΪ�棬����Ҫ֧��OS.
//	OSIntExit();  											 
//#endif
//} 
void UART5_IRQHandler(void)                	//����5�жϷ������
{
	u8 Res; 
	if(USART_GetITStatus(UART5, USART_IT_RXNE) != RESET)  //�����ж�(���յ������ݱ�����0x0d 0x0a��β)  
       // RXNE׼���ö�ȡ���յ������� 1<<5
	{
		Res =USART_ReceiveData(UART5);//(USART1->DR);	//��ȡ���յ�������   �жϱ�־�����
		
		if((UART5_RX_STA&0x8000)==0)//����δ���   0x8000=1<<15
		{
			if(UART5_RX_STA&0x4000)//���յ���0x0d  0x4000=1<<14
			{
				if(Res!=0x0a)UART5_RX_STA=0;//���մ���,���¿�ʼ
				else UART5_RX_STA|=0x8000;	//��������� 	   
			}
			else //��û�յ�0X0D
			{	
				if(Res==0x0d)UART5_RX_STA|=0x4000;
				else
				{
					UART5_RX_BUF[UART5_RX_STA&0X3FFF]=Res ;
					UART5_RX_STA++;
					if(UART5_RX_STA>(USART_REC_LEN-1))UART5_RX_STA=0;//�������ݴ���,���¿�ʼ����	  
				}		 
			}
		}   		 
     } 
} 


//void USART1_IRQHandler(void)                	//����5�жϷ������
//{
//	u8 Res; u8 len;
//	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //�����ж�(���յ������ݱ�����0x0d 0x0a��β)  
//       // RXNE׼���ö�ȡ���յ������� 1<<5
//	{
//		Res =USART_ReceiveData(USART1);//(USART1->DR);	//��ȡ���յ�������   �жϱ�־�����
//		
//		if((USART1_RX_STA&0x8000)==0)//����δ���   0x8000=1<<15
//		{
//			if(USART1_RX_STA&0x4000)//���յ���0x0d  0x4000=1<<14
//			{
//				if(Res!=0x0a)USART1_RX_STA=0;//���մ���,���¿�ʼ
//				else USART1_RX_STA|=0x8000;	//��������� 	   
//			}
//			else //��û�յ�0X0D
//			{	
//				if(Res==0x0d)USART1_RX_STA|=0x4000;
//				else
//				{
//					USART1_RX_BUF[USART1_RX_STA&0X3FFF]=Res ;
//					USART1_RX_STA++;
//					if(USART1_RX_STA>(USART_REC_LEN-1))USART1_RX_STA=0;//�������ݴ���,���¿�ʼ����	  
//				}		 
//			}
//		}   		 
//     } 
//}



 
