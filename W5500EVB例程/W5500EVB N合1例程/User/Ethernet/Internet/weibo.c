/**
******************************************************************************
* @file   		weibo.c
* @author  		WIZnet Software Team 
* @version 		V1.0
* @date    		2015-02-14
* @brief   		΢�� ��ʾ����  
******************************************************************************
**/
#include "weibo.h"
#include "socket.h"
#include "w5500.h"
#include "W5500_conf.h"
#include "utility.h"

#include <stdio.h>
#include <string.h>

#define weibo_server    "61.109.255.136"     					 						/*΢��������IP*/
#define http_path       "/wiznet/"                      					/*HTTP ·��*/
#define weibo_id        "3196855541@qq.com"        								/*�ҵ�����΢���˺�*/
#define weibo_pwd       "wildfire123"        											/*�ҵ�����΢������*/
#define BUF_SIZE    512																						/*΢�����ݳ��ȣ��������ݶ��ʱ����Ҫ�޸�*/
                           
unsigned char weibo_server_ip[4] = {61,109,255,136};    					/*΢��������IP*/
static unsigned int any_local_port = 1004;          							/*TCP socket Զ�̶˿�*/
char weibo_buffer[]={"����һ���µ�W5500�����壬�ɹ��������������!"};

/**
*@brief		΢���ϴ�����
*@param	  weibo�� Ҫ�ϴ���΢������
*@return	��
*/
unsigned char post_weibo_update(char* weibo)
{
	char post_data[385]={0x00,};
	char tmp_buf[BUF_SIZE]={0x00,};                            
	unsigned char ret=0;
	unsigned int len=0;
	char* p;
	if(socket(SOCK_WEIBO,Sn_MR_TCP,any_local_port++,0)!=1)  				/*��ʼ��TCP socket*/
	{
		printf("Socket initialization failed.\r\n");
		return 0;
	}
	else
	{
		printf("Connect with Weibo server.\r\n");
		ret=connect(SOCK_WEIBO,weibo_server_ip,80);       						/*����΢��������, Ĭ�϶˿�Ϊ 80	*/
		if(ret!=1)
		{
			printf("Connect Weibo server failed.\r\n");
			return 0;
		}
		else
		{
			while(getSn_SR(SOCK_WEIBO)!=SOCK_ESTABLISHED);  							/*�ȴ����ӽ����ɹ�*/
			printf("Connected with Weibo server.\r\n");
			sprintf(post_data,"id=%s&pw=%s&cmd=update&status=%s",(char*)weibo_id,(char*)weibo_pwd,weibo);
			sprintf(tmp_buf,"POST %s HTTP/1.1\r\nHost: %s\r\nUser-Agent: w5500\r\nContent-Type: application/x-www-form-urlencoded; charset=gb2312\r\nConnection: close\r\nContent-Length: %d\r\n\r\n%s",(char*)http_path,(char*)weibo_server,strlen(post_data),post_data);
			len=send(SOCK_WEIBO,(unsigned char*)tmp_buf,strlen(tmp_buf)); /*�ϴ���΢������*/
			while(1)
			{
				len=getSn_RX_RSR(SOCK_WEIBO);
				if(len>0)
				{
					memset(tmp_buf,0x00,MAX_BUF_SIZE);
					len=recv(SOCK_WEIBO, (unsigned char*)tmp_buf, len);       /*��������΢���������ķ��ؽ��*/
					p=strstr(tmp_buf,(char*)"\r\n\r\n")+4;                    /*��ȡ��Ч����*/
					printf("rev%s\r\n",p);
			    printf("���ͳɹ�\r\n");
					close(SOCK_WEIBO);          											      	/*�ر�socket*/
					return 1;                  									 							/*�ɹ��ͷ��� 1*/
				}
			}
		}
	}
}

/**
*@brief		ִ��΢���ϴ�����
*@param	  ��
*@return	��
*/
void do_weibo(void)
{
	post_weibo_update(weibo_buffer);
//	Delay_ms(1000); // wait 1000ms
//	while(1);
}

