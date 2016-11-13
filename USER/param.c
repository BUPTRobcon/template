#include "param.h"
#include "flash.h"
#include <stdlib.h>

/**
  * @brief  初始化参数结构体
  *     
  * @note	请传入地址的指针，以实现自增的效果
  *
  * @param  faddr: 数据的地址指针  
  *          
  * @retval u32: 地址对应的flash中的数据
  */
int param_init(Param * param)
{
	u32 addr = PARAM_FLASH_ADDR_START;
	int i, j;
	Pos_data * pos_data;
	Launch_data * launch_data;
	param = (Param *) malloc(sizeof(Param));
	list_init(&param->pos_ptr);
#if FIRST_RUN == 1
	param->pos_num = 0;
#else 
	param->pos_num = STMFLASH_ReadWord_Inc(&addr);
	for (i = 0; i < param->pos_num; ++i)
	{
		pos_data = (Pos_data *) malloc(sizeof(Pos_data));
		pos_data->x = STMFLASH_ReadFloat_Inc(&addr);
		pos_data->y = STMFLASH_ReadFloat_Inc(&addr);
		for (j = 0; j < 7; ++j)
		{
			pos_data->d[i].launch_num = STMFLASH_ReadWord_Inc(&addr);
			for (k = 0; k < pos_data->d[i].launch_num; ++k)
			{
				launch_data = (Launch_data *) malloc(sizeof(Launch_data));
				launch_data->pitch = STMFLASH_ReadFloat_Inc(&addr);
				launch_data->roll = STMFLASH_ReadFloat_Inc(&addr);
				launch_data->speed = STMFLASH_ReadFloat_Inc(&addr);
				launch_data->yaw = STMFLASH_ReadFloat_Inc(&addr);
				
				//insert
			}
		}
	}
#endif
}
