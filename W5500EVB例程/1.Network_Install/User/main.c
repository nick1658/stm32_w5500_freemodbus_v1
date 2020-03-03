/**
******************************************************************************
* @file    			main.c
* @author  			WIZnet Software Team
* @version 			V1.0
* @date    			2015-12-11
* @brief   			用3.5.0版本库建的工程模板
* @attention  								
*               内网测试，请保证W5500的IP与测试PC机在同一网段内，且不冲突
*               如果是用网线跟PC机直连，请设置PC机本地连接地址IP为静态IP
******************************************************************************
*/ 
#include <stdio.h>
#include <string.h>

#include "stm32f10x.h"

#include "bsp_usart1.h"
#include "bsp_i2c_ee.h"
#include "bsp_i2c_gpio.h"
#include "bsp_spi_flash.h"
#include "bsp_led.h"

#include "w5500.h"
#include "W5500_conf.h"
#include "socket.h"
#include "utility.h"
	
int main(void)
{ 	
	systick_init(72);										/*初始化Systick工作时钟*/
	USART1_Config();										/*初始化串口通信:115200@8-n-1*/
	i2c_CfgGpio();											/*初始化eeprom*/	

	gpio_for_w5500_config();						/*初始化MCU相关引脚*/
	reset_w5500();											/*硬复位W5500*/
	set_w5500_mac();										/*配置MAC地址*/
  set_w5500_ip();											/*配置IP地址*/
	
  socket_buf_init(txsize, rxsize);		/*初始化8个Socket的发送接收缓存大小*/
	
	printf(" 网络已完成初始化……\r\n");
	printf(" 在Dos命令行中输入cmd，之后输入ping %d.%d.%d.%d \r\n",ConfigMsg.lip[0],ConfigMsg.lip[1],ConfigMsg.lip[2],ConfigMsg.lip[3]); 
	
	while(1) 														/*循环执行的函数*/ 
	{
		
  }
} 


