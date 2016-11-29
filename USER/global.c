#include "global.h"
#include "usart.h"


void pitch_move(float v){
	if (v - 1200.f > 0.0000001)
		v = 1200;
	else if (v + 1200.f < 0.0000001)
		v=-1200;
	USART_SendString(USART2,"3v%d\r",(int)v);
}

void roll_move(float v){
	if (v - 1200.f > 0.0000001)
		v = 1200;
	else if (v + 1200.f < 0.0000001)
		v=-1200;
	USART_SendString(USART2,"0v%d\r",-(int)v);
}







