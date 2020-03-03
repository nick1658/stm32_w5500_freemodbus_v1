/**
******************************************************************************
* @file    			main.c
* @author  			WIZnet Software Team
* @version 			V1.0
* @date    			2015-02-24
* @brief   			用3.5.0版本库建的工程模板
* @attention  	实验平台：野火 iSO-MINI STM32 开发板 + 野火W5500网络适配板
*
*               默认使用野火开发板的SPI1接口，请断开“J10”A4与cs间的短接帽

*								本测试为外网测试，请保证W5500的网线可以访问Internet
* 							可以自动获取IP地址，也可以使用默认指定的IP地址
*
* 
* 实验平台:野火 iSO-MINI STM32 开发板 
* 论坛    :http://www.chuxue123.com
* 淘宝    :http://firestm32.taobao.com
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
#include "tcp_demo.h"
#include "udp_demo.h"
#include "http_client.h"
#include "dhcp.h"
#include "http_server.h"
#include "httputil.h"
#include "netbios.h"
#include "dns.h"
#include "smtp.h"
#include "ntp.h"
#include "weibo.h"
#include "ping.h"

uint8 reboot_flag = 0;
uint8 demo_num;
char key = 0;
bool bTreat;
void main_menu(void);
int main(void)
{ 	
    
	systick_init(72);				/* 初始化Systick工作时钟*/
	USART1_Config(); 				/*初始化串口通信:115200@8-n-1*/
	i2c_CfgGpio();					/*初始化eeprom*/	
	LED_GPIO_Config(); 				/*初始化led*/
	DHT11_GPIO_Config();			/*初始化dht11温湿度传感器*/
	demo_num=demo_select();			/*选择要测试的程序 */	
	printf(" 初始化W5500……\r\n"); 	
	gpio_for_w5500_config();		/* 初始化MCU相关引脚*/
	reset_w5500();					/*复位W5500*/
	printf(" W5500 Hardware initialized! \r\n");	
	set_w5500_mac();				/*配置W5500MAC地址*/
	if(ip_from==IP_FROM_EEPROM||ip_from==IP_FROM_DEFINE)/*不DHCP获取IP地址，就配置默认IP地址*/
	set_w5500_ip();					/*配置W5500IP地址*/

	socket_buf_init(txsize, rxsize); /*初始化8个Socket的发送接收缓存大小*/
	printf(" 应用程序执行中…… \r\n"); 
	mail_message();					 /*初始化 邮件命令信息*/
	ntp_client_init(); 				 /*初始化ntp服务器信息*/
	while(1)//循环执行的函数 
	{ 
		main_menu();
	}
} 
void main_menu(void)
{
	bTreat = (bool)RESET ;
	/*注意socket不要冲突*/
	switch(demo_num)
	{
		case 1:
				bTreat = (bool)SET;
				if ((serial_key_pressed((char*)&key) == 1) && (key == 'Q'))
				{
				close(SOCK_TCPS);
				printf("\r\n Stop\r\n");
				demo_num=demo_select();			/*选择要测试的程序 */
				set_w5500_ip();					
					break;
				}
				do_tcp_server();/*TCP_Server 数据回环测试程序*/
		break;
		case 2:
				bTreat = (bool)SET;
				if ((serial_key_pressed((char*)&key) == 1) && (key == 'Q'))
				{
				close(SOCK_TCPC);
				printf("\r\n Stop\r\n");
				demo_num=demo_select();			/*选择要测试的程序 */	
				set_w5500_ip();
				break;
				}
				do_tcp_client();/*TCP_Client 数据回环测试程序*/
		break;
		case 3:
				bTreat = (bool)SET;
				if ((serial_key_pressed((char*)&key) == 1) && (key == 'Q'))
				{
				close(SOCK_UDPS);
				printf("\r\n Stop\r\n");
				demo_num=demo_select();			/*选择要测试的程序 */
				set_w5500_ip();
				break;	
				}
				do_udp();/*UDP 数据回环测试*/
		break;
		case 4:
				bTreat = (bool)SET;
				if ((serial_key_pressed((char*)&key) == 1) && (key == 'Q'))
				{
				close(SOCK_DHCP);
				printf("\r\n Stop\r\n");
				demo_num=demo_select();			/*选择要测试的程序 */
				set_w5500_ip();	
				break;					
				}
				do_dhcp();/*DHCP测试程序*/
		break;
		case 5:
				bTreat = (bool)SET;
				if ((serial_key_pressed((char*)&key) == 1) && (key == 'Q'))
				{
				close(SOCK_DNS);
				printf("\r\n Stop\r\n");
				demo_num=demo_select();			/*选择要测试的程序 */	
				set_w5500_ip();	
				break;
				}
				do_dns();  /*域名解析测试程序*/
		break;
		case 6:
				bTreat = (bool)SET;
				if ((serial_key_pressed((char*)&key) == 1) && (key == 'Q'))
				{
				close(SOCK_DNS);
				close(SOCK_SMTP);
				printf("\r\n Stop\r\n");
				demo_num=demo_select();			/*选择要测试的程序 */	
				set_w5500_ip();
				break;
				}
				if(!mail_send_ok)/*如果没有邮件发送*/
				{
					do_dns(); /*解析邮件服务器IP*/
					do_smtp();/*smtp测试程序*/
				}
				if(mail_send_ok)/*如果邮件发送成功，退出*/
					break;
		break;
		case 7:
				bTreat = (bool)SET;
				if ((serial_key_pressed((char*)&key) == 1) && (key == 'Q'))
				{
				close(SOCK_HTTPS);
				printf("\r\n Stop\r\n");
				demo_num=demo_select();			/*选择要测试的程序 */	
				set_w5500_ip();
				break;					
				}
				do_https();/*Web server测试程序*/
				if(reboot_flag==1)
				{
				set_network();
				do_https();/*Web server测试程序*/						
				}
		break;
		case 8:
				bTreat = (bool)SET;
				if ((serial_key_pressed((char*)&key) == 1) && (key == 'Q'))
				{
				close(SOCK_HTTPC);
				printf("\r\n Stop\r\n");
				demo_num=demo_select();			/*选择要测试的程序 */	
				set_w5500_ip();
				break;
				}
				do_http_client();/*http_client测试程序*/
		break;
		case 9:
				bTreat = (bool)SET;
				if ((serial_key_pressed((char*)&key) == 1) && (key == 'Q'))
				{
				close(SOCK_HTTPS);
				close(SOCK_NETBIOS);					
				printf("\r\n Stop\r\n");
				demo_num=demo_select();			/*选择要测试的程序 */	
				set_w5500_ip();
				}
				do_dhcp();//执行dhcp获取动态IP
				if(dhcp_ok==1)
				{
					do_https();//
					do_netbios(); /*netbios测试程序*/
//					if(reboot_flag) reboot();
				}
		break;

		case 10:
				bTreat = (bool)SET;
				if ((serial_key_pressed((char*)&key) == 1) && (key == 'Q'))
				{
				close(SOCK_DHCP);
				close(SOCK_NTP);					
				printf("\r\n Stop\r\n");
				demo_num=demo_select();			/*选择要测试的程序 */	
				set_w5500_ip();
				}
				do_dhcp();                        /*从DHCP服务器获取并配置IP地址*/
				if(dhcp_ok==1)										/*成功获取IP地址*/
				{				
				do_ntp_client();
				} 
		break;
		case 11:
				bTreat = (bool)SET;
				if ((serial_key_pressed((char*)&key) == 1) && (key == 'Q'))
				{
				close(SOCK_WEIBO);				
				printf("\r\n Stop\r\n");
				demo_num=demo_select();			/*选择要测试的程序 */	
				set_w5500_ip();
				}
				do_weibo();	  /*weibo抓取测试程序*/
		break;

		case 12:
				bTreat = (bool)SET;
				if ((serial_key_pressed((char*)&key) == 1) && (key == 'Q'))
				{	
				close(SOCK_PING);					
				printf("\r\n Stop\r\n");
				demo_num=demo_select();			/*选择要测试的程序 */	
				set_w5500_ip();
				}
				do_ping(); /*ping测试程序*/
		break;
		default:
		break;		
	}
}


