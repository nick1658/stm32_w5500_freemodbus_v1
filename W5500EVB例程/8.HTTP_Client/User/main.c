/**
******************************************************************************
* @file    			main.c
* @author  			WIZnet Software Team
* @version 			V1.0
* @date    			2015-12-12
* @brief   			用3.5.0版本库建的工程模板
* @attention  	
*								本测试为外网测试，请保证W5500的网线可以访问Internet
* 							可以自动获取IP地址，也可以使用默认指定的IP地址
******************************************************************************
*/ 

#include <stdio.h>
#include <string.h>

#include "stm32f10x.h"

#include "bsp_usart1.h"
#include "bsp_i2c_ee.h"
#include "bsp_i2c_gpio.h"
#include "bsp_led.h"
#include "bsp_dht11.h"

#include "w5500.h"
#include "W5500_conf.h"
#include "socket.h"
#include "utility.h"
/*app函数头文件*/

#include "http_client.h"

int main(void)
{ 	
    
	systick_init(72);				            /*初始化Systick工作时钟*/
	USART1_Config(); 				            /*初始化串口通信:115200@8-n-1*/
	i2c_CfgGpio();				    	        /*初始化eeprom*/
	DHT11_GPIO_Config();								/*初始化dht11温湿度传感器*/	
	gpio_for_w5500_config();	         	/*初始化MCU相关引脚*/
	reset_w5500();					            /*硬复位W5500*/
	set_w5500_mac();                    /*配置MAC地址*/
	set_w5500_ip(); 										/*配置IP地址*/
	socket_buf_init(txsize, rxsize);    /*初始化8个Socket的发送接收缓存大小*/
	
	printf(" YEElink服务器IP为:%d.%d.%d.%d\r\n",yeelink_server_ip[0],yeelink_server_ip[1],yeelink_server_ip[2],yeelink_server_ip[3]);
	printf(" YEElink服务器端口默认为:%d \r\n",yeelink_port);
	printf(" 我的Yelink ID为:%s \r\n",yeelink_id);
	printf(" 我的Yelink PWD为:%s \r\n",yeelink_pwd);
	printf(" 请登录以上账号观察实时温度变化， 串口也可以查看获取信息\r\n");
	printf(" 应用程序执行中……\r\n"); 
	while(1)//循环执行的函数 
	{ 
		do_http_client();	
	}
} 



