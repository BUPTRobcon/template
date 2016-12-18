#include "main.h" 

//extern u8 cmd, sticks[4];
//extern u8 ptr;

enum {stop, running, ready} state;//车的运动状态

//全局变量定义区
bool g_stop_flag = false;
float pos_x,pos_y;
float angle; 
int WantSpeed = 0;//飞轮目标速度

float Default_angle;
int Chassis_motor2 =0 , Chassis_motor3 =0 , Chassis_motor4 =0 ;
float CH_angle_M2 = -PI/2 - 2*PI/3, CH_angle_M3 = -PI/2, CH_angle_M4 = -PI/2 + 2*PI/3;

float Speed_Duty = 770;
float Speed_P = 0.0030, Speed_D = 0.3000 , Speed_I = 0.0f;

POSITION END, START;

#define MAXSPEED 970
#define MINSPEED 600

int Speed_error = 0;
int PRESpeed_error = 0;
int truespeed = 0;


//--------------暂时用来草稿-------------

float Move_radium, Angle_radium;
int Speed_min, Speed_max, Angle_speed, Move_speed;
int ms = 0;

void set_params(){
	Speed_min = 30;
	Speed_max = 6000;
	Move_radium = 0.00001;
	Angle_radium = 0.00001;
	Angle_speed = 2400;
	Move_speed = 3500;
}

void TIM2_IRQHandler(void){
	int i;
	if( TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET ) 
	{
		ms++;
		TIM_ClearITPendingBit(TIM2, TIM_FLAG_Update);//必须清除中断标志位否则一直中断
		/*truespeed =  TIM5->CNT - 6000;
		TIM5->CNT = 6000;
		
		PRESpeed_error = Speed_error;
		Speed_error = truespeed - WantSpeed;

		Speed_Duty -= (Speed_P*Speed_error + Speed_D*(Speed_error - PRESpeed_error));
	
		if(Speed_Duty >MAXSPEED)
			Speed_Duty = MAXSPEED ;
		if(Speed_Duty < MINSPEED)
			Speed_Duty = MINSPEED ;
		TIM_SetCompare1(TIM8,1000000/50*Speed_Duty/10000 - 1);
		//USART_SendString(bluetooth,"msg: TrueSpeed:%d  WantSpeed:%d duty:%f\n",truespeed,WantSpeed,Speed_Duty);
		if (wait_cnt>-1)
			if (++wait_cnt==50)
			{	USART_SendString(UART5,"Good to go\n");
				USART_ITConfig(UART5, USART_IT_RXNE, ENABLE);wait_cnt=-1;}
		TIM_ClearITPendingBit(TIM2,TIM_FLAG_Update);//必须清除中断标志位否则一直中断
		for (i=0;i<12;i++) 
		 {
			 if (b[i]->cnt>0) b[i]->cnt--;
			 if (b[i]->cnt==0)
				 b[i]->ispressed=false;
		 }*/
	}	
}


//-----------------草稿结束------------------------

int main(void)
{   //system_stm32f4xx.c #define PLL_M=8 PLL_N=336 HSE -> SYSCLK 168MHZ
	// RCC->CFGR |= RCC_CFGR_PPRE2_DIV2; APHB1_CLK 84MHZ
	//AHB 168MHZ  APB2 84MHZ  APB1 42MHZ

	extern int TIM3_round, TIM4_round;
	int Hx, Hy;
	
	int g_vega_pos_x = 0, g_vega_pos_y= 0;   
	float g_vega_angle = 0;
	
	int temp_x, temp_y;
	float temp_angle;
	
	float errorAngle, error_X, error_Y, Serror_X, Serror_Y;
	float factor = 5;
	float anglebuf =0;
	int Xianding = 0;
	u8 POS_Mark = 0;
	float Pre_ErrorY = 0;
	
	static int TURN_speed;
	static int ChassisSpeed;
	
	rcc_config();
	gpio_config();
	delay_init(168);  //初始化延时函数
//	EXTI_config();
	nvic_config();
	
	TIM3_Init();
	TIM4_Init();
//	TIM5_Init();
	TIM8_Init();
//	SPI2_Init();
	usart_init(&Hx, &Hy);
	cmd_init();
	param_init();
//	can_add_callback();

	can_init();
    vega_init(&g_vega_pos_x,&g_vega_pos_y,&g_vega_angle);
    vega_reset();
	delay_ms(4000);
	TIM2_Init();
	USART_SendString(UART5,"msg: Let's go!\n");
	
	set_params();
	direction_angle = PI/2;
	START.X = g_vega_pos_x* 0.0001 * 0.81;
	START.Y = g_vega_pos_y* 0.0001 * 0.81;
	START.ANG = (g_vega_angle/180.f)*PI;
	OPEN_Hander = 0;

	END = START;
	state = ready;
	temp_x = g_vega_pos_x;
	temp_y = g_vega_pos_y;
	temp_angle = g_vega_angle;

    while(1) 
	{
		if(g_stop_flag){//停止一切运动
			USART_SendString(MOTOR_USARTx,"2v0\r3v0\r4v0\r");
			USART_SendString(USART2,"0v0\r2v0\r");
			USART_SendString(UART4,"1v0\r");
		}else{
			//bottons_check();
			//sticks_check(Hx,Hy);
			pos_x = g_vega_pos_x* 0.0001 * 0.81;
			pos_y = g_vega_pos_y* 0.0001 * 0.81;
			angle = (g_vega_angle/180.f)*PI;
			//USART_SendString(UART5,"\n\nX=%f   Y=%f  angle=%f\n",pos_x, pos_y , angle);
			ChassisSpeed= Move_speed;		
			if(OPEN_Hander ==0){
				/**-------------------------自动部分--------------------------------**/		
				if(state == ready){
					/*vega_set_angle(temp_angle);
					vega_set_pos(temp_x, temp_y);
					g_vega_angle = temp_angle;
					g_vega_pos_x = temp_x;
					g_vega_pos_y = temp_y;
					pos_x = g_vega_pos_x* 0.0001 * 0.81;
					pos_y = g_vega_pos_y* 0.0001 * 0.81;
					angle = (g_vega_angle/180.f)*PI;*/
					START.X = g_vega_pos_x* 0.0001 * 0.81;
					START.Y = g_vega_pos_y* 0.0001 * 0.81;
					START.ANG = (g_vega_angle/180.f)*PI;
					ms = 0;
					state = running;
				}
				if(state == running){
					errorAngle = angle - END.ANG;
					error_X = END.X - pos_x;
					error_Y = END.Y - pos_y;
					
					Serror_X = START.X - pos_x;
					Serror_Y = START.Y - pos_y;
					
					if(factor * factor * (powf(Serror_X,2)+powf(Serror_Y,2)) < (powf(error_X,2)+powf(error_Y,2))) {
						if((powf(Serror_X,2)+powf(Serror_Y,2)) > 0.0009)
							ChassisSpeed = sqrt(powf(Serror_X,2)+powf(Serror_Y,2))*Move_speed * factor;
						else 
							ChassisSpeed = 0.03 * Move_speed * factor;
					}
					else {
						if((powf(error_X,2)+powf(error_Y,2)) > 1)
							ChassisSpeed = sqrt(powf(error_X,2)+powf(error_Y,2))*Move_speed;
						else if((powf(error_X,2)+powf(error_Y,2)) > 0.0025)
							ChassisSpeed = sqrt(sqrt(powf(error_X,2)+powf(error_Y,2))) * Move_speed;
						else
							ChassisSpeed = 89.44 * (powf(error_X,2)+powf(error_Y,2)) * Move_speed;
					}
					/*if((powf(error_X,2)+powf(error_Y,2)) > 1)
						ChassisSpeed = sqrt(powf(error_X,2)+powf(error_Y,2))*Move_speed;
					else if((powf(error_X,2)+powf(error_Y,2)) > 0.01)
						ChassisSpeed = sqrt(sqrt(powf(error_X,2)+powf(error_Y,2)))*Move_speed;
					else 
						ChassisSpeed = 31.6 * (powf(error_X,2)+powf(error_Y,2)) * Move_speed;
					*/
					if(ChassisSpeed>Speed_max)
						ChassisSpeed = Speed_max;
					
					if(errorAngle >= 0.05 && errorAngle < 2)
					{          //角度调整
						TURN_speed = -1*Angle_speed*errorAngle;
					}
					else if(errorAngle <=-0.05 && errorAngle > -2)
					{
						TURN_speed = -1*Angle_speed*errorAngle;
					}
					else if(errorAngle > Angle_radium && errorAngle < 0.05)
					{
						TURN_speed = -1*Angle_speed*errorAngle;
					}
					else if(errorAngle < -1*Angle_radium && errorAngle > -0.05)
					{
						TURN_speed = -1*Angle_speed*errorAngle;
					}
					else if(errorAngle>=2)
					{
						TURN_speed =-1*Angle_speed*2;
					}
					else if(errorAngle<=-2)
					{
						TURN_speed =-1*Angle_speed*(-2);
					}
					else
					{
						TURN_speed= 0;
					}
					
					if(powf(error_X,2)+powf(error_Y,2) <= Move_radium)
					{//已经到达
						ChassisSpeed = 0;
					}
					else
					{
						direction_angle = atan2(error_Y,error_X);
					}
			
					Chassis_motor3 = -1* (ChassisSpeed * cos((CH_angle_M3 +angle) - direction_angle) + TURN_speed);//Y轴方向，这里direction_angle代表小车相对于场地坐标系的方向
					Chassis_motor4 = -1* (ChassisSpeed * cos((CH_angle_M4 +angle) - direction_angle) + TURN_speed);
					Chassis_motor2 = -1* (ChassisSpeed * cos((CH_angle_M2 +angle) - direction_angle) + TURN_speed);
					
					temp_x = g_vega_pos_x;
					temp_y = g_vega_pos_y;
					temp_angle = g_vega_angle;
					
					if(fabs(Chassis_motor2) < 1 && fabs(Chassis_motor4) < 1 && fabs(Chassis_motor3) < 1)
					{
						USART_SendString(MOTOR_USARTx,"2v0\r3v0\r4v0\r");
						state = stop;
					}
					else
					{
						USART_SendString(MOTOR_USARTx,"2v%d\r3v%d\r4v%d\r",Chassis_motor2 , Chassis_motor3 , Chassis_motor4);
					}
				}
				else if(state == stop)
				{
					if(ms>200)
						USART_SendString(CMD_USARTx,"msg: %fs\n",ms*5.0/1000);
					ms = 0;
					/*pos_x = temp_x* 0.0001 * 0.81;
					pos_y = temp_y* 0.0001 * 0.81;
					angle = (temp_angle/180.f)*PI;*/
					state = ready;
				}
			}
			else if(OPEN_Hander ==1)
			{
				/********手柄部分***********/
				errorAngle = angle;
				TURN_speed = 0;
				if (L1.ispressed) TURN_speed -= 400;
				if (R1.ispressed) TURN_speed += 400;
				
				if (LU.ispressed) direction_angle = -PI/2;
				else if (LD.ispressed) direction_angle = PI/2;
				else if (LL.ispressed) direction_angle = PI;
				else if (LR.ispressed) direction_angle = 0;
				else ChassisSpeed = 0;
				
				Chassis_motor3 = -1* (ChassisSpeed*cos((CH_angle_M3 + errorAngle) - direction_angle)+TURN_speed);
				Chassis_motor4 = -1* (ChassisSpeed*cos((CH_angle_M4 + errorAngle) - direction_angle)+TURN_speed);
				Chassis_motor2 = -1* (ChassisSpeed*cos((CH_angle_M2 + errorAngle) - direction_angle)+TURN_speed);
				USART_SendString(MOTOR_USARTx,"2v%d\r3v%d\r4v%d\r",Chassis_motor2 , Chassis_motor3 , Chassis_motor4);
				
				END.X=pos_x;
				END.Y=pos_y;
				END.ANG=angle;
			}
			
			if (pitch_flag){
				pitch.now = (TIM4_round * 30000.f - TIM4->CNT) / 10000.f;
				pitch.d = pur_pitch - pitch.now;
				pitch.dd = pitch.d - pitch.d_last;
				pitch.d_last = pitch.d;
				if (fabs(pitch.d) < 0.01)
				{
					if (pitch.cnt > 10)
					{
						pitch_flag=false;
						pitch_move(0);
					}
					else
					{
						pitch.cnt++;
						pitch_move(pitch.d * 300 + pitch.dd * 240);
					}
				}
				else
				{
					pitch.cnt = 0;
					pitch_move(pitch.d * 300 + pitch.dd * 240);
				}
			}
			if (roll_flag)
			{
				roll.now = (TIM3_round * 30000.f - TIM3->CNT)/10000.f;
				roll.d = pur_roll - roll.now;
				roll.dd = roll.d - roll.d_last;
				roll.d_last = roll.d;
				if (fabs(roll.d) < 0.01)
				{
					if (roll.cnt > 10)
					{
						roll_flag=false;
						roll_move(0);
					}
					else
					{
						roll.cnt++;
						roll_move(roll.d * 300 + roll.dd * 240);
					}
				}
				else
				{
					roll.cnt=0;
					roll_move(roll.d * 300 + roll.dd * 240);
				}
			}	
		}
	}
}
