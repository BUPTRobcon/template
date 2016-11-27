#include "usart.h"	
#include "math.h"


#define X_mid 125
#define Y_mid 125





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
	USART_ITConfig(UART5, USART_IT_RXNE, ENABLE);//��������ж�
	USART_Init(USART3, &USART_InitStructure); //��ʼ������3
    USART_Cmd(USART3, ENABLE);  //ʹ�ܴ���3
		
	USART_ITConfig(UART5, USART_IT_RXNE, ENABLE);//��������ж�
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
	
	USART_Init(UART4, &USART_InitStructure); //��ʼ������4
    USART_Cmd(UART4, ENABLE);  //ʹ�ܴ���4

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
	static u8 rec_buf [4];
	static u8 index =0;
	static s8 WPQS_Mark=0;
	static u16 W_number = 0 ,P_number = 0 ,Q_number = 0 , S_number = 0;
	int ten = 1;
	int i=0;
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET) 
	{
		//USART_ClearITPendingBit(USART3,USART_IT_RXNE);
		Res =USART_ReceiveData(USART3);
		//USART_SendString(UART5,&Res);
		switch (Res){
			case 'A'://ǰ��       �н�
				Hand_flag = 5 ;
				direction_angle = -PI/2;
				break;
			case 'B'://����
				Hand_flag = 5 ;
				direction_angle = PI/2;
				break;
			case 'D'://��
				Hand_flag = 5 ;
				direction_angle = PI;
				break;
			case 'C'://��
				Hand_flag = 5 ;
				direction_angle = 0;
				break;
			case 'F'://L2         ����
				WANTSPEED-=50;
				if(WANTSPEED <0)
					WANTSPEED = 0;
				//USART_SendString(UART5,"%d  \n",WANTSPEED);
				break;
			case 'N'://R2
				WANTSPEED+=50;
			   // USART_SendString(UART5,"%d  \n",WANTSPEED);
				break;
			
			case 'M'://L2         ��ת ��
				TURN_Flag = 5;
				Turn_R_Flag = 1;
				break;
			case 'E'://R2
				TURN_Flag = 5;
				Turn_L_Flag = 1;
				break;
			
			case 'I'://Y         ���ֱ�����
				OPEN_Hander = 1;
				break;
			case 'J'://R2        ���ֱ�����
				OPEN_Hander = 0;
				break;
			
//			case 'W'://ǰ��
//				if(WPQS_Mark == 2){
//					for(i = index - 1;i>=0 ;i--){
//						if(i>=0){
//							P_number += (rec_buf[i]-48)*ten;
//							ten = ten*10;
//						}
//					}
//					USART_SendString(UART5,"P=%d",P_number);
//				}
//				if(P_number>90&&P_number<140){
//					P_number = 125;
//				}
//				WPQS_Mark = 1;
//				index = 0;
//				
//				break;
//			case 'P'://����
//				Hand_flag = true ;
//				if(WPQS_Mark == 1){
//					for(i = index - 1;i>=0 ;i--){
//						if(i>=0){
//							W_number += (rec_buf[i]-48)*ten;
//							ten = ten*10;
//						}
//					
//					}
//					USART_SendString(UART5,"W=%d \n",W_number);
//				}
//				if(W_number>90&&W_number<140){
//					W_number = 125;
//				}
//				direction_angle = atan2 ((W_number-125), (P_number-125));
//				WPQS_Mark = 2;
//				index = 0;
//				break;

			default :
//				if(Res >=48 &&Res <=57&&(WPQS_Mark == 2||WPQS_Mark ==1)){
//					rec_buf[index] = Res;
//					//USART_SendString(UART5,"N=%d \n",rec_buf[index]);
//					index ++;
//				}else{
//					
//				}
				OPEN_Hander = 10;
				break;
		}
		//USART_SendString(UART5,"D=%f \n",direction_angle);
  		 
     }		
} 






 
