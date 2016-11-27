#include "spi.h"
#include "global.h"
#include "can.h"
#include "vega.h"

#include "math.h"
#include "TIM.h"
//F11   E14


int g_vega_pos_x = 0,g_vega_pos_y= 0;   //vega全场定位模块返回的x数据和y数据
float g_vega_angle = 0;  //vega全场定位模块返回的x数据和y数据
float pos_x,pos_y;
float angle; 
float Default_angle;
int Chassis_motor2 =0 , Chassis_motor3 =0 , Chassis_motor4 =0 ;
float CH_angle_M2 = -PI/2 - 2*PI/3 ,CH_angle_M3 = -PI/2 , CH_angle_M4 = -PI/2 + 2*PI/3;
static u8 run_FLAG = 0;
static u8 run_FLAG_turn = 0;
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
	
	if( TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET ) 
	{
		TIM_ClearITPendingBit(TIM2,TIM_FLAG_Update);//必须清除中断标志位否则一直中断	
	}	
}
//-----------------草稿结束------------------------

int main(void)
{  
	float errorAngle ,error_X ,error_Y;
	float anglebuf =0;
	int Xianding = 0;
	u8 POS_Mark = 0;
	float Pre_ErrorY = 0;
	
	
	rcc_config();
	gpio_config();
	//TIM2_Configuration();
	delay_init(168);  //初始化延时函数
	uart_init(115200);
	can_init();
    vega_init(&g_vega_pos_x,&g_vega_pos_y,&g_vega_angle);
    vega_reset();
	nvic_config();
	delay_ms(6000);
	USART_SendString(UART5,"start");
	
	
	direction_angle = PI/2;
	START.X = g_vega_pos_x* 0.0001 * 0.81;
	START.Y = g_vega_pos_y* 0.0001 * 0.81;
	START.ANG = (g_vega_angle/180.f)*PI;
	
	END.X = 0;
	END.Y = -2;
	END.ANG =PI/4;
	
	
    while(1) 
	{
		pos_x = g_vega_pos_x* 0.0001 * 0.81;
		pos_y = g_vega_pos_y* 0.0001 * 0.81;
		angle = (g_vega_angle/180.f)*PI;
		USART_SendString(UART5,"\n\nX=%f   Y=%f  angle=%f\n",pos_x, pos_y , angle);
		ChassisSpeed= WANTSPEED;		
		if(OPEN_Hander ==0){
			/**-------------------------自动部分--------------------------------**/
			
			USART_SendString(UART5,"\n\n\n\nEND.Y=%f   X=%f   Y=%f  angle=%f\n",END.Y,pos_x, pos_y , angle);
			USART_SendString(UART5,"errorAngle=%f   CH_angle_M3 =%f   CH_angle_M2 =%f   CH_angle_M4 =%f\n",errorAngle,CH_angle_M3,CH_angle_M2,CH_angle_M4);

			if(POS_Mark ==0){//去
				
				errorAngle = angle - END.ANG;
				error_X = END.X - pos_x;
				error_Y = END.Y - pos_y;

				if(errorAngle >0.01){          //角度调整
					TURN_speed =-300*errorAngle;
				}else if(errorAngle <-0.01){
					TURN_speed =-300*errorAngle;
				}else{
					TURN_speed= 0;
				}
				
				if(error_X >0.001||error_X <-0.001){            //前进方向
					direction_angle = atan2(error_Y,error_X);
				}else{
					if((Pre_ErrorY*error_Y)<=0.f&&error_Y<0.1&&error_Y>-0.1){//已经到达
						POS_Mark = 1;
						USART_SendString(UART5,"***回\n");
					}else{
						direction_angle = atan2(error_Y,error_X);
					}
				}

				USART_SendString(UART5,"***errorAngle===%f  ,error_X =%f ,error_Y =%f\n",errorAngle,error_X,error_Y);
				USART_SendString(UART5,"***direction_angle===%f\n",direction_angle);
				
				Chassis_motor3 = -1* (ChassisSpeed*cos((CH_angle_M3 +angle -START.ANG) - direction_angle)+TURN_speed);//Y轴方向，这里direction_angle代表小车相对于场地坐标系的方向
				Chassis_motor4 =      ChassisSpeed*cos((CH_angle_M4 +angle -START.ANG)- direction_angle)+TURN_speed;
				Chassis_motor2 = -1* (ChassisSpeed*cos((CH_angle_M2 +angle -START.ANG) - direction_angle)+TURN_speed);
				
			}else if(POS_Mark ==1){//回
				errorAngle = angle - START.ANG;
				error_X = START.X - pos_x;
				error_Y = START.Y - pos_y;
				
				if(errorAngle >0.01){              //角度调整
					TURN_speed =-300*errorAngle;
				}else if(errorAngle <-0.01){
					TURN_speed =-300*errorAngle;
				}else{
					TURN_speed= 0;
				}
				
				if(error_X >0.001||error_X <-0.001){                 //前进方向
					direction_angle = atan2(error_Y,error_X);//返回值0--2PI
				}else{
					if((Pre_ErrorY*error_Y)<=0.f&&error_Y<0.1&&error_Y>-0.1){//已经到达
						POS_Mark = 2;
						USART_SendString(UART5,"***qu\n");
					}else{
						direction_angle = atan2(error_Y,error_X);
					}
				}
				
				USART_SendString(UART5,"errorAngle===%f  ,error_X =%f ,error_Y =%f\n",errorAngle,error_X,error_Y);
				USART_SendString(UART5,"direction_angle===%f\n",direction_angle);
				
				Chassis_motor3 = -1* (ChassisSpeed*cos((CH_angle_M3 +errorAngle) - direction_angle)+TURN_speed);//Y轴方向，这里direction_angle代表小车相对于场地坐标系的方向
				Chassis_motor4 = ChassisSpeed*cos((CH_angle_M4+errorAngle)- direction_angle)+TURN_speed;
				Chassis_motor2 = -1* (ChassisSpeed*cos((CH_angle_M2 +errorAngle) - direction_angle)+TURN_speed);
			}else{
				Chassis_motor3 = 0;
				Chassis_motor2 = 0;
				Chassis_motor4 = 0;

			}
			
			USART_SendString(UART5,"Chassis_motor3===%d  ,Chassis_motor2 =%d ,Chassis_motor4 =%d\n",Chassis_motor3,Chassis_motor2,Chassis_motor4);
			//USART_SendString(MOTOR_USARTx,"2v%d\r3v%d\r4v%d\r",Chassis_motor2, Chassis_motor3, Chassis_motor4);
			delay_ms(8);
		
		}else if(OPEN_Hander ==1){
					
			/*---------------------------------手柄部分-----------------------------------------*/			
			errorAngle = angle - START.ANG;
			
			if(TURN_Flag >=1){
				TURN_Flag--;
				run_FLAG_turn = 0;
				if(Turn_R_Flag == 1){
					TURN_speed = 300;
					
				}
				if(Turn_L_Flag == 1){
					TURN_speed = -300;
					
				}
				if(Hand_flag <=1){
					USART_SendString(MOTOR_USARTx,"2v%d\r3v%d\r4v%d\r",(-1* TURN_speed ), (-1*TURN_speed) , TURN_speed);
				}
			}else{
				Turn_R_Flag = 0;
				Turn_L_Flag = 0;
				if(Hand_flag <=1&&run_FLAG_turn ==0){
					USART_SendString(MOTOR_USARTx,"2v%d\r3v%d\r4v%d\r",0 , 0 , 0);
					USART_SendString(MOTOR_USARTx,"2v%d\r3v%d\r4v%d\r",0 , 0 , 0);
					USART_SendString(MOTOR_USARTx,"2v%d\r3v%d\r4v%d\r",0 , 0 , 0);
				}
				TURN_speed = 0;
				run_FLAG_turn=1;
			}
			if(Hand_flag >0){
				Hand_flag--;
				Chassis_motor3 =-1* (ChassisSpeed*cos((CH_angle_M3 +errorAngle) - direction_angle)+TURN_speed);
				Chassis_motor4 = ChassisSpeed*cos((CH_angle_M4+errorAngle)- direction_angle)+TURN_speed;
				Chassis_motor2 = -1* (ChassisSpeed*cos((CH_angle_M2 +errorAngle) - direction_angle)+TURN_speed);
				USART_SendString(MOTOR_USARTx,"2v%d\r3v%d\r4v%d\r",Chassis_motor2 , Chassis_motor3 , Chassis_motor4);
				run_FLAG = 0;
				//USART_SendString(UART5,"timD=%d \n",TURN_speed);
			}else{
				if(run_FLAG ==0){
					USART_SendString(MOTOR_USARTx,"2v%d\r3v%d\r4v%d\r",0 , 0 , 0);
					USART_SendString(MOTOR_USARTx,"2v%d\r3v%d\r4v%d\r",0 , 0 , 0);
					USART_SendString(MOTOR_USARTx,"2v%d\r3v%d\r4v%d\r",0 , 0 , 0);
				}					
				run_FLAG=1;
			}
			delay_ms(8);
		}else {
			USART_SendString(MOTOR_USARTx,"2v%d\r3v%d\r4v%d\r",0 , 0 , 0);
			delay_ms(8);
		}
	}
}



void ConveySpeed(int CS ,int WS ,float EX ,float EY){
	if((powf(EX ,2)+powf(EY,2))>=0.02){
		if(CS<WS){
		
		}
	}else{
	
	}
}
