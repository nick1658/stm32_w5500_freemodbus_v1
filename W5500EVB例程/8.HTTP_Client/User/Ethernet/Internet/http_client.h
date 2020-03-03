#ifndef __HTTP_CLIENT_H
#define __HTTP_CLIENT_H
#include "stm32f10x.h"
#include "types.h"
#define yeelink_id				"wildfire"        //your yeelink ID
#define yeelink_pwd				"wildfire123"     //your yeelink password
extern uint8 yeelink_server_ip[];
extern uint8 yeelink_port;

void do_http_client(void);//TCP Clinet回环演示函数
#endif 

