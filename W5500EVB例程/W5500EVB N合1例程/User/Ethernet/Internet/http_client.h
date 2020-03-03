#ifndef __HTTP_CLIENT_H
#define __HTTP_CLIENT_H
#include "stm32f10x.h"
#include "types.h"

extern uint8 yeelink_server_ip[];
extern  uint8 yeelink_port;
void do_http_client(void);//TCP Clinet回环演示函数
#endif 

