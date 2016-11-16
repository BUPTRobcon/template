#include "global.h"

GPIO_InitTypeDef GPIO_InitStructure;
NVIC_InitTypeDef NVIC_InitStructure;

/*
 *函数名：void GPIO_Configuration(uint16_t GPIO_Pin,
                        GPIOMode_TypeDef  GPIO_Mode,
                        GPIOSpeed_TypeDef GPIO_Speed,
                        GPIOOType_TypeDef GPIO_OType,
                        GPIOPuPd_TypeDef GPIO_PuPd,
                        GPIO_TypeDef* GPIOx)
 * 功能：IO口初始化
 * 参数：uint16_t GPIO_Pin, 			管脚号
        GPIOMode_TypeDef  GPIO_Mode,	输出模式
        GPIOSpeed_TypeDef GPIO_Speed,	速度
        GPIOOType_TypeDef GPIO_OType,	407   GPIO_OType_PP 为推挽   GPIO_OType_OD 为开漏
        GPIOPuPd_TypeDef GPIO_PuPd,		GPIO_PuPd_DOWN GPIO_PuPd_UP GPIO_PuPd_NOPULL
        GPIO_TypeDef* GPIOx				管脚组别
 * 返回值：
 */
void GPIO_Configuration(uint16_t GPIO_Pin,GPIOMode_TypeDef  GPIO_Mode,GPIOOType_TypeDef GPIO_OType,
	GPIOSpeed_TypeDef GPIO_Speed,GPIOPuPd_TypeDef GPIO_PuPd,GPIO_TypeDef* GPIOx)
{
 
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin;                       //管脚号 
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode;                     //模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd;                  
  	GPIO_Init(GPIOx, &GPIO_InitStructure);                        //管脚组别 
}


/*
 *函数名：void NVIC_Configuration(uint8_t NVIC_IRQChannel,
				uint8_t NVIC_IRQChannelPreemptionPriority,
				uint8_t NVIC_IRQChannelSubPriority,
                FunctionalState NVIC_IRQChannelCmd)
 * 功能：中断分组初始化
 * 参数：uint8_t NVIC_IRQChannel, 					 	使能按键所在的外部中断通道
   		uint8_t NVIC_IRQChannelPreemptionPriority,		抢占优先级
		uint8_t NVIC_IRQChannelSubPriority				子优先级
        FunctionalState NVIC_IRQChannelCmd              ENABLE DISABLE
 * 返回值：
 */
void NVIC_Configuration(uint8_t NVIC_IRQChannel,uint8_t NVIC_IRQChannelPreemptionPriority,
	uint8_t NVIC_IRQChannelSubPriority,FunctionalState NVIC_IRQChannelCmd)
{	
	NVIC_InitStructure.NVIC_IRQChannel = NVIC_IRQChannel;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=NVIC_IRQChannelPreemptionPriority;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =NVIC_IRQChannelSubPriority;		
	NVIC_InitStructure.NVIC_IRQChannelCmd = NVIC_IRQChannelCmd;			
	NVIC_Init(&NVIC_InitStructure);
}

//加入以下代码,支持printf函数,而不需要选择use MicroLIB	  
#if _printf
#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 
}; 

FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
_sys_exit(int x) 
{ 
	x = x; 
} 
//重定义fputc函数 
int fputc(int ch, FILE *f)
{ 	
	while((USART_printf->SR&0X40)==0);//循环发送,直到发送完毕  
	USART1->DR = (u8) ch;      
	return ch;
}
#endif

 
 void rcc_config()
{
	RCC_DeInit();			//初始化为缺省值
	//SystemInit();//源自system_stm32f10x.c文件,只需要调用此函数,则可完成RCC的配置.
	//RCC_GetClocksFreq(&RCC_ClockFreq);
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);//使能GPIOA时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE); //使能GPIOB时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE); //使能GPIOC时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,ENABLE); //使能GPIOD时钟
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);//使能SPI1时钟
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);	//使能USART1时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5,ENABLE);	//使能USART5时钟	
}

void gpio_config(void)
{
	
	//GPIO_PinRemapConfig(GPIO_Remap_USART1,ENABLE);   //?
	//GPIO_PinRemapConfig(GPIO_Remap_USART2,ENABLE);   //?
	//GPIO_PinRemapConfig(GPIO_Remap_USART3,ENABLE);   //?
	GPIO_Configuration(GPIO_Pin_6 | GPIO_Pin_7,GPIO_Mode_AF,
	                   GPIO_OType_PP,GPIO_Speed_50MHz,GPIO_PuPd_UP,GPIOB);
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource6,GPIO_AF_USART1); //GPIOB6复用为USART1
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource7,GPIO_AF_USART1); //GPIOB7复用为USART1
	
	GPIO_Configuration(GPIO_Pin_2 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_8 | GPIO_Pin_9,
					   GPIO_Mode_AF,GPIO_OType_PP,GPIO_Speed_50MHz,GPIO_PuPd_UP,GPIOD);
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource2,GPIO_AF_UART5); //GPIOD2复用为USART5
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource8,GPIO_AF_USART3); //GPIOD8复用为USART3
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource9,GPIO_AF_USART3); //GPIOD9复用为USART3
	
	GPIO_Configuration(GPIO_Pin_12,GPIO_Mode_AF,GPIO_OType_PP,GPIO_Speed_50MHz,GPIO_PuPd_UP,GPIOC);
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource12,GPIO_AF_UART5); //GPIOC12复用为USART5
//----------------------------USART---------------------------------------------------------------------------------------------	
//	GPIO_Configuration(GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15,
//	                   GPIO_Mode_AF,GPIO_OType_PP,GPIO_Speed_100MHz,GPIO_PuPd_UP,GPIOB);
//	GPIO_PinAFConfig(GPIOB,GPIO_PinSource5,GPIO_AF_SPI2); 
//	GPIO_PinAFConfig(GPIOB,GPIO_PinSource6,GPIO_AF_SPI2); 
//	GPIO_PinAFConfig(GPIOB,GPIO_PinSource7,GPIO_AF_SPI2);
//----------------------------SPI2---------------------------------------------------------------------------------------------
	GPIO_Configuration(GPIO_Pin_12 | GPIO_Pin_13,GPIO_Mode_AF,GPIO_OType_PP,GPIO_Speed_50MHz,GPIO_PuPd_NOPULL,GPIOD);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource12, GPIO_AF_TIM4); 	
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource13, GPIO_AF_TIM4); 
	GPIO_Configuration(GPIO_Pin_6 | GPIO_Pin_7,GPIO_Mode_AF,GPIO_OType_PP,GPIO_Speed_50MHz,GPIO_PuPd_NOPULL,GPIOA);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_TIM3); 	
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_TIM3); 
//----------------------------增量码盘---------------------------------------------------------------------------------------------
}

/*
 * 函数名：void EXTI_Configuration(uint32_t EXTI_Line,
                EXTIMode_TypeDef EXTI_Mode,
                EXTITrigger_TypeDef EXTI_Trigger,
                FunctionalState EXTI_LineCmd)
 * 功能：外部中断初始化
 * 参数：uint32_t EXTI_Line,               中断线：EXTI_Line0~EXTI_Line15
        EXTIMode_TypeDef EXTI_Mode,         EXTI_Mode_Interrupt,EXTI_Mode_Event
        EXTITrigger_TypeDef EXTI_Trigger,   EXTI_Trigger_Rising,EXTI_Trigger_Falling, EXTI_Trigger_Rising_Falling
        FunctionalState EXTI_LineCmd        ENABLE DISABLE
 * 返回：
 */
void EXTI_Configuration(uint32_t EXTI_Line,
                EXTIMode_TypeDef EXTI_Mode,
                EXTITrigger_TypeDef EXTI_Trigger,
                FunctionalState EXTI_LineCmd)
{
    EXTI_InitTypeDef exti_init_structure;
    exti_init_structure.EXTI_Line = EXTI_Line;
    exti_init_structure.EXTI_Mode = EXTI_Mode;
    exti_init_structure.EXTI_Trigger = EXTI_Trigger;
    exti_init_structure.EXTI_LineCmd = EXTI_LineCmd;
    EXTI_Init(&exti_init_structure);
}


void nvic_config()
{
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2 2:2   抢占：响应    共 3级:3级
	
	NVIC_Configuration(UART5_IRQn,3,3,ENABLE);
	
//	NVIC_InitStructure.NVIC_IRQChannel=SPI1_IRQn;
//    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;//1
//    NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;//0
//    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//    NVIC_Init(&NVIC_InitStructure);
	
}

void USART_SendString(USART_TypeDef* USARTx, char *fmt, ...)
 {
	char buffer[STR_BUFFER_LEN+1];  // 
	u8 i = 0;
	
	va_list arg_ptr;
	va_start(arg_ptr, fmt);  
	vsnprintf(buffer, STR_BUFFER_LEN+1, fmt, arg_ptr);
	USART_ClearFlag(USARTx,USART_FLAG_TXE);
	while ((i < STR_BUFFER_LEN) && buffer[i])
	{
		if(buffer[i] == '\n'){
        USART_SendData(USARTx,(u8)'\r');
        while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET); 
        USART_SendData(USARTx,(u8)buffer[i++]);
        }else{
	    USART_SendData(USARTx, (u8) buffer[i++]);
        }
        while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET); 
	}
	va_end(arg_ptr);
 } 

