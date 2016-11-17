#include "stm32f4xx.h"
#include "global.h"

typedef struct node{
    void *data;
    struct node *link;
}list_node,*link_list;


int list_init(link_list *first);
void list_clear(link_list *first);
int list_get_length(link_list *first);
int list_isempty(link_list *first);


list_node* list_search(link_list *first,void *tmp,bool (*cmp)(void *,void *));
list_node *list_locate(link_list *first,int i);
void node_move(link_list* first,int i,list_node *p);
int list_insert(link_list *first,int i,void *new_data);
int list_remove_data(link_list *first,void *data,bool (*cmp)(void *,void *));
int list_remove_num(link_list *first,int t);
void list_copy(link_list *dest,link_list *src);
//void list_print(USART_TypeDef *USARTx,link_list *first,int param_group);

