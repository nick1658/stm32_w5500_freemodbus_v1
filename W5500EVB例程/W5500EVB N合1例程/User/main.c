/**
******************************************************************************
* @file    			main.c
* @author  			WIZnet Software Team
* @version 			V1.0
* @date    			2015-02-24
* @brief   			��3.5.0�汾�⽨�Ĺ���ģ��
* @attention  	ʵ��ƽ̨��Ұ�� iSO-MINI STM32 ������ + Ұ��W5500���������
*
*               Ĭ��ʹ��Ұ�𿪷����SPI1�ӿڣ���Ͽ���J10��A4��cs��Ķ̽�ñ

*								������Ϊ�������ԣ��뱣֤W5500�����߿��Է���Internet
* 							�����Զ���ȡIP��ַ��Ҳ����ʹ��Ĭ��ָ����IP��ַ
*
* 
* ʵ��ƽ̨:Ұ�� iSO-MINI STM32 ������ 
* ��̳    :http://www.chuxue123.com
* �Ա�    :http://firestm32.taobao.com
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
/*app����ͷ�ļ�*/
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
    
	systick_init(72);				/* ��ʼ��Systick����ʱ��*/
	USART1_Config(); 				/*��ʼ������ͨ��:115200@8-n-1*/
	i2c_CfgGpio();					/*��ʼ��eeprom*/	
	LED_GPIO_Config(); 				/*��ʼ��led*/
	DHT11_GPIO_Config();			/*��ʼ��dht11��ʪ�ȴ�����*/
	demo_num=demo_select();			/*ѡ��Ҫ���Եĳ��� */	
	printf(" ��ʼ��W5500����\r\n"); 	
	gpio_for_w5500_config();		/* ��ʼ��MCU�������*/
	reset_w5500();					/*��λW5500*/
	printf(" W5500 Hardware initialized! \r\n");	
	set_w5500_mac();				/*����W5500MAC��ַ*/
	if(ip_from==IP_FROM_EEPROM||ip_from==IP_FROM_DEFINE)/*��DHCP��ȡIP��ַ��������Ĭ��IP��ַ*/
	set_w5500_ip();					/*����W5500IP��ַ*/

	socket_buf_init(txsize, rxsize); /*��ʼ��8��Socket�ķ��ͽ��ջ����С*/
	printf(" Ӧ�ó���ִ���С��� \r\n"); 
	mail_message();					 /*��ʼ�� �ʼ�������Ϣ*/
	ntp_client_init(); 				 /*��ʼ��ntp��������Ϣ*/
	while(1)//ѭ��ִ�еĺ��� 
	{ 
		main_menu();
	}
} 
void main_menu(void)
{
	bTreat = (bool)RESET ;
	/*ע��socket��Ҫ��ͻ*/
	switch(demo_num)
	{
		case 1:
				bTreat = (bool)SET;
				if ((serial_key_pressed((char*)&key) == 1) && (key == 'Q'))
				{
				close(SOCK_TCPS);
				printf("\r\n Stop\r\n");
				demo_num=demo_select();			/*ѡ��Ҫ���Եĳ��� */
				set_w5500_ip();					
					break;
				}
				do_tcp_server();/*TCP_Server ���ݻػ����Գ���*/
		break;
		case 2:
				bTreat = (bool)SET;
				if ((serial_key_pressed((char*)&key) == 1) && (key == 'Q'))
				{
				close(SOCK_TCPC);
				printf("\r\n Stop\r\n");
				demo_num=demo_select();			/*ѡ��Ҫ���Եĳ��� */	
				set_w5500_ip();
				break;
				}
				do_tcp_client();/*TCP_Client ���ݻػ����Գ���*/
		break;
		case 3:
				bTreat = (bool)SET;
				if ((serial_key_pressed((char*)&key) == 1) && (key == 'Q'))
				{
				close(SOCK_UDPS);
				printf("\r\n Stop\r\n");
				demo_num=demo_select();			/*ѡ��Ҫ���Եĳ��� */
				set_w5500_ip();
				break;	
				}
				do_udp();/*UDP ���ݻػ�����*/
		break;
		case 4:
				bTreat = (bool)SET;
				if ((serial_key_pressed((char*)&key) == 1) && (key == 'Q'))
				{
				close(SOCK_DHCP);
				printf("\r\n Stop\r\n");
				demo_num=demo_select();			/*ѡ��Ҫ���Եĳ��� */
				set_w5500_ip();	
				break;					
				}
				do_dhcp();/*DHCP���Գ���*/
		break;
		case 5:
				bTreat = (bool)SET;
				if ((serial_key_pressed((char*)&key) == 1) && (key == 'Q'))
				{
				close(SOCK_DNS);
				printf("\r\n Stop\r\n");
				demo_num=demo_select();			/*ѡ��Ҫ���Եĳ��� */	
				set_w5500_ip();	
				break;
				}
				do_dns();  /*�����������Գ���*/
		break;
		case 6:
				bTreat = (bool)SET;
				if ((serial_key_pressed((char*)&key) == 1) && (key == 'Q'))
				{
				close(SOCK_DNS);
				close(SOCK_SMTP);
				printf("\r\n Stop\r\n");
				demo_num=demo_select();			/*ѡ��Ҫ���Եĳ��� */	
				set_w5500_ip();
				break;
				}
				if(!mail_send_ok)/*���û���ʼ�����*/
				{
					do_dns(); /*�����ʼ�������IP*/
					do_smtp();/*smtp���Գ���*/
				}
				if(mail_send_ok)/*����ʼ����ͳɹ����˳�*/
					break;
		break;
		case 7:
				bTreat = (bool)SET;
				if ((serial_key_pressed((char*)&key) == 1) && (key == 'Q'))
				{
				close(SOCK_HTTPS);
				printf("\r\n Stop\r\n");
				demo_num=demo_select();			/*ѡ��Ҫ���Եĳ��� */	
				set_w5500_ip();
				break;					
				}
				do_https();/*Web server���Գ���*/
				if(reboot_flag==1)
				{
				set_network();
				do_https();/*Web server���Գ���*/						
				}
		break;
		case 8:
				bTreat = (bool)SET;
				if ((serial_key_pressed((char*)&key) == 1) && (key == 'Q'))
				{
				close(SOCK_HTTPC);
				printf("\r\n Stop\r\n");
				demo_num=demo_select();			/*ѡ��Ҫ���Եĳ��� */	
				set_w5500_ip();
				break;
				}
				do_http_client();/*http_client���Գ���*/
		break;
		case 9:
				bTreat = (bool)SET;
				if ((serial_key_pressed((char*)&key) == 1) && (key == 'Q'))
				{
				close(SOCK_HTTPS);
				close(SOCK_NETBIOS);					
				printf("\r\n Stop\r\n");
				demo_num=demo_select();			/*ѡ��Ҫ���Եĳ��� */	
				set_w5500_ip();
				}
				do_dhcp();//ִ��dhcp��ȡ��̬IP
				if(dhcp_ok==1)
				{
					do_https();//
					do_netbios(); /*netbios���Գ���*/
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
				demo_num=demo_select();			/*ѡ��Ҫ���Եĳ��� */	
				set_w5500_ip();
				}
				do_dhcp();                        /*��DHCP��������ȡ������IP��ַ*/
				if(dhcp_ok==1)										/*�ɹ���ȡIP��ַ*/
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
				demo_num=demo_select();			/*ѡ��Ҫ���Եĳ��� */	
				set_w5500_ip();
				}
				do_weibo();	  /*weiboץȡ���Գ���*/
		break;

		case 12:
				bTreat = (bool)SET;
				if ((serial_key_pressed((char*)&key) == 1) && (key == 'Q'))
				{	
				close(SOCK_PING);					
				printf("\r\n Stop\r\n");
				demo_num=demo_select();			/*ѡ��Ҫ���Եĳ��� */	
				set_w5500_ip();
				}
				do_ping(); /*ping���Գ���*/
		break;
		default:
		break;		
	}
}


