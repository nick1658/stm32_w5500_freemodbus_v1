/**
******************************************************************************
* @file   		weibo.c
* @author  		WIZnet Software Team 
* @version 		V1.0
* @date    		2015-02-14
* @brief   		微博 演示函数  
******************************************************************************
**/
#include "weibo.h"
#include "socket.h"
#include "w5500.h"
#include "W5500_conf.h"
#include "utility.h"

#include <stdio.h>
#include <string.h>

#define weibo_server    "61.109.255.136"     					 						/*微博服务器IP*/
#define http_path       "/wiznet/"                      					/*HTTP 路径*/
#define weibo_id        "3196855541@qq.com"        								/*我的新浪微博账号*/
#define weibo_pwd       "wildfire123"        											/*我的新浪微博密码*/
#define BUF_SIZE    512																						/*微博内容长度，发送内容多的时候需要修改*/
                           
unsigned char weibo_server_ip[4] = {61,109,255,136};    					/*微博服务器IP*/
static unsigned int any_local_port = 1004;          							/*TCP socket 远程端口*/
char weibo_buffer[]={"测试一下新的W5500开发板，成功后程序马上上线!"};

/**
*@brief		微博上传函数
*@param	  weibo： 要上传的微博内容
*@return	无
*/
unsigned char post_weibo_update(char* weibo)
{
	char post_data[385]={0x00,};
	char tmp_buf[BUF_SIZE]={0x00,};                            
	unsigned char ret=0;
	unsigned int len=0;
	char* p;
	if(socket(SOCK_WEIBO,Sn_MR_TCP,any_local_port++,0)!=1)  				/*初始化TCP socket*/
	{
		printf("Socket initialization failed.\r\n");
		return 0;
	}
	else
	{
		printf("Connect with Weibo server.\r\n");
		ret=connect(SOCK_WEIBO,weibo_server_ip,80);       						/*连接微博服务器, 默认端口为 80	*/
		if(ret!=1)
		{
			printf("Connect Weibo server failed.\r\n");
			return 0;
		}
		else
		{
			while(getSn_SR(SOCK_WEIBO)!=SOCK_ESTABLISHED);  							/*等待连接建立成功*/
			printf("Connected with Weibo server.\r\n");
			sprintf(post_data,"id=%s&pw=%s&cmd=update&status=%s",(char*)weibo_id,(char*)weibo_pwd,weibo);
			sprintf(tmp_buf,"POST %s HTTP/1.1\r\nHost: %s\r\nUser-Agent: w5500\r\nContent-Type: application/x-www-form-urlencoded; charset=gb2312\r\nConnection: close\r\nContent-Length: %d\r\n\r\n%s",(char*)http_path,(char*)weibo_server,strlen(post_data),post_data);
			len=send(SOCK_WEIBO,(unsigned char*)tmp_buf,strlen(tmp_buf)); /*上传传微博内容*/
			while(1)
			{
				len=getSn_RX_RSR(SOCK_WEIBO);
				if(len>0)
				{
					memset(tmp_buf,0x00,MAX_BUF_SIZE);
					len=recv(SOCK_WEIBO, (unsigned char*)tmp_buf, len);       /*接收来自微博服务器的返回结果*/
					p=strstr(tmp_buf,(char*)"\r\n\r\n")+4;                    /*获取有效负载*/
					printf("rev%s\r\n",p);
			    printf("发送成功\r\n");
					close(SOCK_WEIBO);          											      	/*关闭socket*/
					return 1;                  									 							/*成功就返回 1*/
				}
			}
		}
	}
}

/**
*@brief		执行微博上传函数
*@param	  无
*@return	无
*/
void do_weibo(void)
{
	post_weibo_update(weibo_buffer);
//	Delay_ms(1000); // wait 1000ms
//	while(1);
}

