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
#define weibo_id        "3196855541@qq.com"        								/*我的新浪微博账号*/
#define weibo_pwd       "wildfire123"        											/*我的新浪微博密码*/
char weibo_buffer[]={"very good!测试一下新的W5500开发板!"};
extern uint8 txsize[];
extern uint8 rxsize[];
//extern const	uint8 str1; 
uint8 buffer[2048];/*定义一个2KB的缓存*/
uint8 reboot_flag = 0;
int main(void)
{
  Systick_Init(72);/* 初始化Systick工作时钟*/  
	GPIO_Configuration();/* 配置GPIO*/
  USART1_Init(); /*初始化串口通信:115200@8-n-1*/
  at24c16_init();/*初始化eeprom*/
  printf("W5500 EVB initialization over.\r\n");
  
  Reset_W5500();/*硬重启W5500*/
  WIZ_SPI_Init();/*初始化SPI接口*/
  printf("W5500 initialized!\r\n"); 
  set_default(); 
  set_network();
	printf(" 我的微博ID为:%s \r\n",weibo_id);
	printf(" 我的微博PWD为:%s \r\n",weibo_pwd);
	printf(" 看串口调试信息：rev255: ok 说明上传成功, 登陆以上微博账号就可看到发送微博信息 \r\n");
					
  printf("W5500 is ready!\r\n");
  
  while(1)
  {
  post_weibo_update(weibo_buffer);
  while(1);
  }

}

