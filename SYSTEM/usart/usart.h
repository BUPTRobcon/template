#ifndef __USART_H
#define __USART_H
#include "global.h"
 
u8  UART5_RX_BUF[USART_REC_LEN]; //���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
u16 UART5_RX_STA;         		//����״̬���	
u8  USART3_RX_BUF[USART_REC_LEN];    
u16 USART3_RX_STA;

void uart_init(u32 bound);
u8 cmd,sticks[4];
u8 ptr;

#endif


