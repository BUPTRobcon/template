#include "usart.h"	
#include "math.h"


#define X_mid 125
#define Y_mid 125





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
	USART_ITConfig(UART5, USART_IT_RXNE, ENABLE);//开启相关中断
	USART_Init(USART3, &USART_InitStructure); //初始化串口3
    USART_Cmd(USART3, ENABLE);  //使能串口3
		
	USART_ITConfig(UART5, USART_IT_RXNE, ENABLE);//开启相关中断
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
	
	USART_Init(UART4, &USART_InitStructure); //初始化串口4
    USART_Cmd(UART4, ENABLE);  //使能串口4

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
			case 'A'://前进       行进
				Hand_flag = 5 ;
				direction_angle = -PI/2;
				break;
			case 'B'://后退
				Hand_flag = 5 ;
				direction_angle = PI/2;
				break;
			case 'D'://左
				Hand_flag = 5 ;
				direction_angle = PI;
				break;
			case 'C'://有
				Hand_flag = 5 ;
				direction_angle = 0;
				break;
			case 'F'://L2         变速
				WANTSPEED-=50;
				if(WANTSPEED <0)
					WANTSPEED = 0;
				//USART_SendString(UART5,"%d  \n",WANTSPEED);
				break;
			case 'N'://R2
				WANTSPEED+=50;
			   // USART_SendString(UART5,"%d  \n",WANTSPEED);
				break;
			
			case 'M'://L2         旋转 左
				TURN_Flag = 5;
				Turn_R_Flag = 1;
				break;
			case 'E'://R2
				TURN_Flag = 5;
				Turn_L_Flag = 1;
				break;
			
			case 'I'://Y         开手柄控制
				OPEN_Hander = 1;
				break;
			case 'J'://R2        关手柄控制
				OPEN_Hander = 0;
				break;
			
//			case 'W'://前后
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
//			case 'P'://左右
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






 
