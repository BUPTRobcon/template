#include "cmd_func.h"
#include "cmd.h"
#include "global.h"
#include "stdlib.h"
#include "string.h"
#include "math.h"
#include "param.h"

extern Param * param;
extern bool g_stop_flag;
extern u8 target;       //目标0-6
extern float g_vega_x;
extern float g_vega_y;
static Pos_data * now_pos;     //当前点的数据指针

void cmd_reboot_func(int argc,char *argv[]){
    NVIC_SystemReset();
}

void cmd_stop_func(int argc,char *argv[]){
    if(argc == 1){
        g_stop_flag = 0x01&(~g_stop_flag);
    }else if(argc == 2){
        g_stop_flag = atoi(argv[1]);
    }
}

void cmd_hello_func(int argc,char *argv[]){
    USART_SendString(CMD_USARTx, "Hello World");
}

void cmd_pos_func(int argc,char *argv[])
{
    int no;
    int no0;
    int i;
    float pos_x,pos_y;
    Pos_data *data;
    list_node * ptr;
    int start_no,end_no;
    int erro_no;
    if (strcmp(argv[1], "now"))
    {
        USART_SendString(CMD_USARTx, "x:%f,y:%f", g_vega_x,g_vega_y);
    }else
    if(strcmp(argv[1],"add") == 0){
        if(argc < 5){
            USART_SendString(CMD_USARTx,"Error!please enter:\n");
            USART_SendString(CMD_USARTx,"   pos add <no> <x> <y>\n");
        }
        no = atoi(argv[2]);
        pos_x = atof(argv[3]);
        pos_y = atof(argv[4]);
        data = (Pos_data *)malloc(sizeof(Pos_data));
        data->x = pos_x;
        data->y = pos_y;
        for (i = 0; i < 7; ++i)
        {
            data->d[i].launch_num = 0;
            list_init(&data->d[i].launch_ptr);
        }
        if((erro_no = list_insert(&param->pos_ptr, no, data)) < 0){
            USART_SendString(CMD_USARTx,"Error:%d\n",erro_no);
        }
        print_pos_list(param->pos_ptr);
    }else if(strcmp(argv[1],"print") == 0){
        print_pos_list(param->pos_ptr);
    }else if(strcmp(argv[1],"del") == 0){
        no = atoi(argv[2]);
        ptr = list_locate(&param->pos_ptr, no);
        data = ptr->data;
        for (int i = 0; i < 7; ++i)
        {
            clear_launch(&data->d[i].launch_ptr); 
        }
        free(data);
        list_remove_num(&param->pos_ptr,no);
        param->pos_num-=1;
        print_pos_list(param->pos_ptr);
    }else if(strcmp(argv[1],"clear") == 0){
        clear_pos(param->pos_ptr);
        param->pos_num = 0;
    }else if(strcmp(argv[1],"save") == 0){
        erro_no = param_save(param);
        if(erro_no < 0){
            USART_SendString(CMD_USARTx,"Save error:%d\n",erro_no);
        }
    }else if(strcmp(argv[1],"modi") == 0){
        no = atoi(argv[2]);
        if((ptr = local_pos(no)) == 0){
            USART_SendString(CMD_USARTx,"Not found point No:%d\n",no);
            return;
        }
        if(argc < 5){
            USART_SendString(CMD_USARTx,"Error cmd format\n");
            return;
        }
        pos_x = atof(argv[3]);
        pos_y = atof(argv[4]);

        data->x = pos_x;
        data->y = pos_y;
        print_pos_list(param->pos_ptr);
    }else if (strcmp(argv[1], "jmp"))
    {
        no = atoi(argv[2]);
        no0 = atoi(argv[3]);
        ptr = list_locate(&param->pos_ptr, no);
        node_move(&param->pos_ptr, no0, ptr);
    }else if (strcmp(argv[1], "pop"))
    {
        /* code */
    }
}

void cmd_action_func(int argc,char *argv[])
{
    int no;
    float x, y, v;
    float yaw;
    list_node * ptr;
    if (argc == 1)
    {
        //跑到下一个点
    }else if (argc == 2){
        no = atoi(argv[1]);
        ptr = list_locate(param->pos_ptr, no);
        now_pos = ptr->data;
        //跑到指定的点去
    }else if (argc == 3){
        x = atof(argv[1]);
        y = atof(argv[2]);
        v = atof(argv[3]);
        //跑到指定的位置
    }else if (strcmp(argv[1],"rotate")){
        v = atof(argv[2]);
        yaw = atof(argv[3]);
        //测试底盘电机转动到一定角度
    }
}

void cmd_switch_func(int argc,char *argv[])
{
    u8 state1,state2;
    state1 = GPIO_ReadInputDataBit(GPIOF, GPIO_Pin_11);
    state2 = GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_14);
    USART_SendString(CMD_USARTx, "roll_switch:%dpitch_switch:%d", state1, state2);
}

void cmd_speed_func(int argc,char *argv[])
{

}

void cmd_launch_func(int argc,char *argv[])
{
    int no;
    float pitch, roll, speed, yaw;
    if (argc = 1)
    {
        //如果没开无刷，那就开无刷，转一圈推飞盘
    }else if (strcmp(argv[1], "now"))
    {
        //发射参数
        //USART_SendString(CMD_USARTn, "", g_vega_x,g_vega_y);
    }else if (strcmp(argv[1],"start"))
    {
        //如果没有开无刷，那就开无刷，一直推飞盘
    }else if (strcmp(argv[1],"load"))
    {
        no = atoi(argv[2]);
        //
    }else if (strcmp(argv[1], "set"))
    {
        pitch = atof(argv[2]);
        roll = atof(argv[3]);
        speed = atof(argv[4]);
        yaw = atof(argv[5]);
        //
    }else if (strcmp(argv[1], "print"))
    {
        print_launch_list(now_pos->d[target].launch_ptr);
    }else if (strcmp(argv[1], "add"))
    {

    }
}
