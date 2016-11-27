#ifndef __USART_H
#define __USART_H
#include "global.h"
 
u8  UART5_RX_BUF[USART_REC_LEN]; //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 
u16 UART5_RX_STA;         		//接收状态标记	
u8  USART3_RX_BUF[USART_REC_LEN];    
u16 USART3_RX_STA;

void uart_init(u32 bound);
u8 cmd,sticks[4];
u8 ptr;

#endif


