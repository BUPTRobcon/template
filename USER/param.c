#include "param.h"

/**
  * @brief  比较两个发射点坐标
  *     
  * @note	在链表中被调用作比较
  *
  * @param  a,b 指向被比较的(void*)data
  *          
  * @retval bool: 是否相同
  */
bool cmp_pos(void *a,void *b){
	Pos_data* p=(Pos_data*)a;
	Pos_data* q=(Pos_data*)b;
	if (p->x==q->x && p->y==q->y)
		return true;
	return false;
}

/**
  * @brief  比较两个发射参数
  *     
  * @note	在链表中被调用作比较
  *
  * @param  a,b: 被比较的(void*)data
  *          
  * @retval bool: 是否相同
  */
bool cmp_launch(void *a,void *b){
	Launch_data* p=(Launch_data*)a;
	Launch_data* q=(Launch_data*)b;
	if (p->pitch==q->pitch &&
		p->roll==q->roll &&
		p->yaw==q->yaw &&
		p->speed==q->speed)
		return true;
	return false;
}

/**
  * @brief  输出发射点坐标
  *     
  * @note	
  *
  * @param  a: 数据的地址指针  
  *          
  * @retval void
  */
void print_pos(void *a){
	Pos_data* p=(Pos_data*)a;
	USART_SendString(bluetooth,"x:%.2f y:%.2f\n",p->x,p->y);
}

/**
  * @brief  输出发射点参数
  *     
  * @note	
  *
  * @param  a: 数据的地址指针  
  *          
  * @retval void
  */
void print_launch(void *a){
	Launch_data* p=(Launch_data*)a;
	USART_SendString(bluetooth,"pitch:%.2f roll:%.2f yaw:%.2f speed:%.2f\n",p->pitch,p->roll,p->yaw,p->speed);
}

/**
  * @brief  输出发射参数链表
  *     
  * @note	
  *
  * @param  p: 第一组发射参数的链表节点指针  
  *          
  * @retval void
  */
void print_launch_list(link_list p){
	while (p!=NULL){
		print_launch(p->data);
		p=p->link;
	}
}

/**
  * @brief  输出发射坐标链表
  *     
  * @note	
  *
  * @param  p: 第一组发射坐标的链表节点指针  
  *          
  * @retval void
  */
void print_pos_list(link_list p){
	while (p!=NULL){
		print_pos(p->data);
		p=p->link;
	}
}

/**
  * @brief  输出整个链表
  *     
  * @note	
  *
  * @param  p: 参数指针  
  *          
  * @retval void
  */
void print_all(Param *p){
	int i;
	Pos_data *pos_temp;
	link_list ptr=p->pos_ptr;
	while (ptr!=NULL){
		print_pos(ptr->data);
		pos_temp=(Pos_data*)(ptr->data);
		for (i=0;i<7;i++)
		if (pos_temp->d[i].launch_num>0)
			print_launch_list(pos_temp->d[i].launch_ptr);
		ptr=ptr->link;
	}
}





