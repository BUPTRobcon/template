#ifndef __PARAM_H
#define __PARAM_H

#include "link_list.h"

typedef struct 		//launch链表节点数据
{
	float pitch;	//俯仰角
	float roll;		//滚转角
	float yaw;		//偏航角
	float speed;	//飞轮转速
}Launch_data;

typedef struct 		//目标线性表
{
	u32 launch_num;
	link_list launch_ptr; 
}Target;

typedef struct 		//pos链表节点数据
{
	float x,y;
	Target d[7];
}Pos_data;

typedef struct 		//参数总结构线性表
{
	u32 pos_num;
	link_list pos_ptr;
}Param;

#define FIRST_RUN 1 
#define PARAM_FLASH_SETOR ((uint16_t)0x0028) //  扇区5
#define PARAM_FLASH_ADDR_START ((uint32_t)0x08020000)
#define PARAM_FLASH_ADDR_END ((uint32_t)0x08020FFF)

int param_init(Param * param); //初始化参数，从flash读取参数

int param_save(Param * param); //将所有参数存入flash

#endif
