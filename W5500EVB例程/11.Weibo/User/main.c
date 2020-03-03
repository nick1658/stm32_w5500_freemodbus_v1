#include "usart.h"
#include "config.h"
#include "device.h"
#include "spi2.h"
#include "socket.h"
#include "w5500.h"
#include "24c16.h"
#include "ult.h"
#include "md5.h"
#include "string.h"
#include "weibo.h"

#include <stdio.h>
#define weibo_id        "3196855541@qq.com"        								/*�ҵ�����΢���˺�*/
#define weibo_pwd       "wildfire123"        											/*�ҵ�����΢������*/
char weibo_buffer[]={"very good!����һ���µ�W5500������!"};
extern uint8 txsize[];
extern uint8 rxsize[];
//extern const	uint8 str1; 
uint8 buffer[2048];/*����һ��2KB�Ļ���*/
uint8 reboot_flag = 0;
int main(void)
{
  Systick_Init(72);/* ��ʼ��Systick����ʱ��*/  
	GPIO_Configuration();/* ����GPIO*/
  USART1_Init(); /*��ʼ������ͨ��:115200@8-n-1*/
  at24c16_init();/*��ʼ��eeprom*/
  printf("W5500 EVB initialization over.\r\n");
  
  Reset_W5500();/*Ӳ����W5500*/
  WIZ_SPI_Init();/*��ʼ��SPI�ӿ�*/
  printf("W5500 initialized!\r\n"); 
  set_default(); 
  set_network();
	printf(" �ҵ�΢��IDΪ:%s \r\n",weibo_id);
	printf(" �ҵ�΢��PWDΪ:%s \r\n",weibo_pwd);
	printf(" �����ڵ�����Ϣ��rev255: ok ˵���ϴ��ɹ�, ��½����΢���˺žͿɿ�������΢����Ϣ \r\n");
					
  printf("W5500 is ready!\r\n");
  
  while(1)
  {
  post_weibo_update(weibo_buffer);
  while(1);
  }

}

