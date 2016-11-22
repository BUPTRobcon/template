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
	USART_Init(USART2, &USART_InitStructure); //��ʼ������1
    USART_Cmd(USART2, ENABLE);  //ʹ�ܴ���1
	USART_Init(UART5, &USART_InitStructure); //��ʼ������5
    USART_Cmd(UART5, ENABLE);  //ʹ�ܴ���5
	USART_Init(USART3, &USART_InitStructure); //��ʼ������3
    USART_Cmd(USART3, ENABLE);  //ʹ�ܴ���3
	//USART_ClearFlag(USART1, USART_FLAG_TC);  	TC�������  1<<6
		
	USART_ITConfig(UART5, USART_IT_RXNE, ENABLE);//��������ж�
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);

}

/******uart5����ָ���������������¶������жϷ�����
void UART5_IRQHandler(void)                	//����5�жϷ������
{
	u8 Res; 
	extern int pitch_flag,roll_flag;
	extern float pur_pitch,pur_roll;
	if(USART_GetITStatus(UART5, USART_IT_RXNE) != RESET)  //�����ж�(���յ������ݱ�����0x0d 0x0a��β)  
       // RXNE׼���ö�ȡ���յ������� 1<<5
	{
		Res =USART_ReceiveData(UART5);//(USART1->DR);	//��ȡ���յ�������   �жϱ�־�����
		if (Res=='8') {pur_pitch=10.f;pitch_flag=1;}
		else if(Res=='2') {pur_pitch=5.f;pitch_flag=1;}
//		else if(Res=='6') USART_SendString(USART2,"0v-200\r");
//		else if(Res=='4') USART_SendString(USART2,"0v200\r");
		else {USART_SendString(USART2,"0v0\r");USART_SendString(USART2,"3v0\r");pitch_flag=0;}
//		if((UART5_RX_STA&0x8000)==0)//����δ���   0x8000=1<<15
//		{
//			if(UART5_RX_STA&0x4000)//���յ���0x0d  0x4000=1<<14
//			{
//				if(Res!=0x0a)UART5_RX_STA=0;//���մ���,���¿�ʼ
//				else UART5_RX_STA|=0x8000;	//��������� 	   
//			}
//			else //��û�յ�0X0D
//			{	
//				if(Res==0x0d)UART5_RX_STA|=0x4000;
//				else
//				{
//					UART5_RX_BUF[UART5_RX_STA&0X3FFF]=Res ;
//					UART5_RX_STA++;
//					if(UART5_RX_STA>(USART_REC_LEN-1))UART5_RX_STA=0;//�������ݴ���,���¿�ʼ����	  
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






 
