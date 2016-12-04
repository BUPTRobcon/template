#include "main.h" 

//extern u8 cmd,sticks[4];
//extern u8 ptr;



//全局变量定义区
bool g_stop_flag;
float g_vega_x;
float g_vega_y;



float pos_x,pos_y;
float angle; 
float Default_angle;
int Chassis_motor2 =0 , Chassis_motor3 =0 , Chassis_motor4 =0 ;
float CH_angle_M2 = -PI/2 - 2*PI/3 ,CH_angle_M3 = -PI/2 , CH_angle_M4 = -PI/2 + 2*PI/3;
static int TURN_speed = 0;
int ChassisSpeed = 1000;

typedef struct 
{
	float X;
	float Y;
	float ANG;
}POSITION;
POSITION END,START;

//--------------暂时用来草稿-------------







//-----------------草稿结束------------------------

int main(void)
{   //system_stm32f4xx.c #define PLL_M=8 PLL_N=336 HSE -> SYSCLK 168MHZ
	// RCC->CFGR |= RCC_CFGR_PPRE2_DIV2; APHB1_CLK 84MHZ
	//AHB 168MHZ  APB2 84MHZ  APB1 42MHZ
	extern int TIM3_round,TIM4_round;

	int TIM3_display=0,TIM4_display=0;
	int Hx,Hy;
	int g_vega_pos_x = 0,g_vega_pos_y= 0;   
	float g_vega_angle = 0; 
	
	float errorAngle ,error_X ,error_Y;
	float anglebuf =0;
	int Xianding = 0;
	u8 POS_Mark = 0;
	float Pre_ErrorY = 0;
	
	rcc_config();
	gpio_config();
	delay_init(168);  //初始化延时函数
	EXTI_config();
	nvic_config();
	TIM2_Init();
	TIM3_Init();
	TIM4_Init();
	TIM8_Init();
//	SPI2_Init();
	usart_init(&Hx,&Hy);
	cmd_init();
	param_init();
//	can_add_callback();

	can_init();
    vega_init(&g_vega_pos_x,&g_vega_pos_y,&g_vega_angle);
    vega_reset();
	delay_ms(2000);
	USART_SendString(UART5,"msg: Let's go!\n");
	
	direction_angle = PI/2;
	START.X = g_vega_pos_x* 0.0001 * 0.81;
	START.Y = g_vega_pos_y* 0.0001 * 0.81;
	START.ANG = (g_vega_angle/180.f)*PI;
	
	END.X = 0;
	END.Y = -2;
	END.ANG =PI/4;

    while(1) 
	{
		bottons_check();
		sticks_check(Hx,Hy);
		pos_x = g_vega_pos_x* 0.0001 * 0.81;
		pos_y = g_vega_pos_y* 0.0001 * 0.81;
		angle = (g_vega_angle/180.f)*PI;
		//USART_SendString(UART5,"\n\nX=%f   Y=%f  angle=%f\n",pos_x, pos_y , angle);
		ChassisSpeed= WANTSPEED;		
//		if(OPEN_Hander ==0){
//			/**-------------------------自动部分--------------------------------**/
//			
//		//	USART_SendString(UART5,"\n\n\n\nEND.Y=%f   X=%f   Y=%f  angle=%f\n",END.Y,pos_x, pos_y , angle);
//			//USART_SendString(UART5,"errorAngle=%f   CH_angle_M3 =%f   CH_angle_M2 =%f   CH_angle_M4 =%f\n",errorAngle,CH_angle_M3,CH_angle_M2,CH_angle_M4);

//			if(POS_Mark ==0){//去
//				
//				errorAngle = angle - END.ANG;
//				error_X = END.X - pos_x;
//				error_Y = END.Y - pos_y;

//				if(errorAngle >0.01){          //角度调整
//					TURN_speed =-300*errorAngle;
//				}else if(errorAngle <-0.01){
//					TURN_speed =-300*errorAngle;
//				}else{
//					TURN_speed= 0;
//				}
//				
//				if(error_X >0.001||error_X <-0.001){            //前进方向
//					direction_angle = atan2(error_Y,error_X);
//				}else{
//					if((Pre_ErrorY*error_Y)<=0.f&&error_Y<0.1&&error_Y>-0.1){//已经到达
//						POS_Mark = 1;
//						//USART_SendString(UART5,"***回\n");
//					}else{
//						direction_angle = atan2(error_Y,error_X);
//					}
//				}

//			//	USART_SendString(UART5,"***errorAngle===%f  ,error_X =%f ,error_Y =%f\n",errorAngle,error_X,error_Y);
//			//	USART_SendString(UART5,"***direction_angle===%f\n",direction_angle);
//				
//				Chassis_motor3 = -1* (ChassisSpeed*cos((CH_angle_M3 +angle -START.ANG) - direction_angle)+TURN_speed);//Y轴方向，这里direction_angle代表小车相对于场地坐标系的方向
//				Chassis_motor4 =      ChassisSpeed*cos((CH_angle_M4 +angle -START.ANG)- direction_angle)+TURN_speed;
//				Chassis_motor2 = -1* (ChassisSpeed*cos((CH_angle_M2 +angle -START.ANG) - direction_angle)+TURN_speed);
//				
//			}else if(POS_Mark ==1){//回
//				errorAngle = angle - START.ANG;
//				error_X = START.X - pos_x;
//				error_Y = START.Y - pos_y;
//				
//				if(errorAngle >0.01){              //角度调整
//					TURN_speed =-300*errorAngle;
//				}else if(errorAngle <-0.01){
//					TURN_speed =-300*errorAngle;
//				}else{
//					TURN_speed= 0;
//				}
//				
//				if(error_X >0.001||error_X <-0.001){                 //前进方向
//					direction_angle = atan2(error_Y,error_X);//返回值0--2PI
//				}else{
//					if((Pre_ErrorY*error_Y)<=0.f&&error_Y<0.1&&error_Y>-0.1){//已经到达
//						POS_Mark = 2;
//						//USART_SendString(UART5,"***qu\n");
//					}else{
//						direction_angle = atan2(error_Y,error_X);
//					}
//				}
//				
//				//USART_SendString(UART5,"errorAngle===%f  ,error_X =%f ,error_Y =%f\n",errorAngle,error_X,error_Y);
//				//USART_SendString(UART5,"direction_angle===%f\n",direction_angle);
//				
//				Chassis_motor3 = -1* (ChassisSpeed*cos((CH_angle_M3 +errorAngle) - direction_angle)+TURN_speed);//Y轴方向，这里direction_angle代表小车相对于场地坐标系的方向
//				Chassis_motor4 = ChassisSpeed*cos((CH_angle_M4+errorAngle)- direction_angle)+TURN_speed;
//				Chassis_motor2 = -1* (ChassisSpeed*cos((CH_angle_M2 +errorAngle) - direction_angle)+TURN_speed);
//			}else{
//				Chassis_motor3 = 0;
//				Chassis_motor2 = 0;
//				Chassis_motor4 = 0;

//			}
//			
//			//USART_SendString(UART5,"Chassis_motor3===%d  ,Chassis_motor2 =%d ,Chassis_motor4 =%d\n",Chassis_motor3,Chassis_motor2,Chassis_motor4);
//			//USART_SendString(MOTOR_USARTx,"2v%d\r3v%d\r4v%d\r",Chassis_motor2, Chassis_motor3, Chassis_motor4);
//			delay_ms(8);
//		
//		}else if(OPEN_Hander ==1){
//					
//			/*---------------------------------手柄部分-----------------------------------------*/			
//			errorAngle = angle - START.ANG;
//			
//			if(TURN_Flag >=1){
//				TURN_Flag--;
//				run_FLAG_turn = 0;
//				if(Turn_R_Flag == 1){
//					TURN_speed = 300;
//					
//				}
//				if(Turn_L_Flag == 1){
//					TURN_speed = -300;
//					
//				}
//				if(Hand_flag <=1){
//					USART_SendString(MOTOR_USARTx,"2v%d\r3v%d\r4v%d\r",(-1* TURN_speed ), (-1*TURN_speed) , TURN_speed);
//				}
//			}else{
//				Turn_R_Flag = 0;
//				Turn_L_Flag = 0;
//				if(Hand_flag <=1&&run_FLAG_turn ==0){
//					USART_SendString(MOTOR_USARTx,"2v%d\r3v%d\r4v%d\r",0 , 0 , 0);
//					USART_SendString(MOTOR_USARTx,"2v%d\r3v%d\r4v%d\r",0 , 0 , 0);
//					USART_SendString(MOTOR_USARTx,"2v%d\r3v%d\r4v%d\r",0 , 0 , 0);
//				}
//				TURN_speed = 0;
//				run_FLAG_turn=1;
//			}
//			if(Hand_flag >0){
//				Hand_flag--;
//				Chassis_motor3 =-1* (ChassisSpeed*cos((CH_angle_M3 +errorAngle) - direction_angle)+TURN_speed);
//				Chassis_motor4 = ChassisSpeed*cos((CH_angle_M4+errorAngle)- direction_angle)+TURN_speed;
//				Chassis_motor2 = -1* (ChassisSpeed*cos((CH_angle_M2 +errorAngle) - direction_angle)+TURN_speed);
//				USART_SendString(MOTOR_USARTx,"2v%d\r3v%d\r4v%d\r",Chassis_motor2 , Chassis_motor3 , Chassis_motor4);
//				run_FLAG = 0;
//				//USART_SendString(UART5,"timD=%d \n",TURN_speed);
//			}else{
//				if(run_FLAG ==0){
//					USART_SendString(MOTOR_USARTx,"2v%d\r3v%d\r4v%d\r",0 , 0 , 0);
//					USART_SendString(MOTOR_USARTx,"2v%d\r3v%d\r4v%d\r",0 , 0 , 0);
//					USART_SendString(MOTOR_USARTx,"2v%d\r3v%d\r4v%d\r",0 , 0 , 0);
//				}					
//				run_FLAG=1;
//			}
//			delay_ms(8);
//		}else {
//			USART_SendString(MOTOR_USARTx,"2v%d\r3v%d\r4v%d\r",0 , 0 , 0);
//			delay_ms(8);
//		}
		errorAngle = angle - START.ANG;
		TURN_speed=0;
		if (L1.ispressed) TURN_speed-=400;
		if (R1.ispressed) TURN_speed+=400;
		if (LU.ispressed) direction_angle = -PI/2;
		else if (LD.ispressed) direction_angle = PI/2;
		else if (LL.ispressed) direction_angle = PI;
		else if (LR.ispressed) direction_angle = 0;
		else ChassisSpeed = 0;
		Chassis_motor3 =-1* (ChassisSpeed*cos((CH_angle_M3 +errorAngle) - direction_angle)+TURN_speed);
		Chassis_motor4 = ChassisSpeed*cos((CH_angle_M4+errorAngle)- direction_angle)+TURN_speed;
		Chassis_motor2 = -1* (ChassisSpeed*cos((CH_angle_M2 +errorAngle) - direction_angle)+TURN_speed);
		USART_SendString(MOTOR_USARTx,"2v%d\r3v%d\r4v%d\r",Chassis_motor2 , Chassis_motor3 , Chassis_motor4);
		if (pitch_flag){
			pitch.now = (TIM4_round * 30000.f - TIM4->CNT)/10000.f;
			pitch.d = pur_pitch - pitch.now;
			pitch.dd = pitch.d - pitch.d_last;
			pitch.d_last = pitch.d;
			if (fabs(pitch.d)<0.01){
				if (pitch.cnt > 10){
					pitch_flag=false;
					pitch_move(0);
				}else{
					pitch.cnt++;
					pitch_move(pitch.d * 300 + pitch.dd * 240);
				}
			}else{
				pitch.cnt=0;
				pitch_move(pitch.d * 300 + pitch.dd * 240);
			}
		}
		if (roll_flag){
			roll.now = (TIM3_round * 30000.f - TIM3->CNT)/10000.f;
			roll.d = pur_roll - roll.now;
			roll.dd = roll.d - roll.d_last;
			roll.d_last = roll.d;
			if (fabs(roll.d)<0.01){
				if (roll.cnt > 10){
					roll_flag=false;
					roll_move(0);
				}else{
					roll.cnt++;
					roll_move(roll.d * 300 + roll.dd * 240);
				}
			}else{
				roll.cnt=0;
				roll_move(roll.d * 300 + roll.dd * 240);
			}
		}
		if (TIM3_round*30000-TIM3->CNT!=TIM3_display){
			TIM3_display=TIM3_round*30000-TIM3->CNT;
			USART_SendString(UART5,"TIM3:%d\n",TIM3_display);}
		if (TIM4_round*30000-TIM4->CNT!=TIM4_display){
			TIM4_display=TIM4_round*30000-TIM4->CNT;
			USART_SendString(UART5,"TIM4:%d\n",TIM4_display);}		
	}
}



void ConveySpeed(int CS ,int WS ,float EX ,float EY){
	if((powf(EX ,2)+powf(EY,2))>=0.02){
		if(CS<WS){
		
		}
	}else{
	
	}
}
