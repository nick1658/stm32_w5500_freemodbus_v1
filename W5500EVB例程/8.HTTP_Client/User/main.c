/**
******************************************************************************
* @file    			main.c
* @author  			WIZnet Software Team
* @version 			V1.0
* @date    			2015-12-12
* @brief   			��3.5.0�汾�⽨�Ĺ���ģ��
* @attention  	
*								������Ϊ�������ԣ��뱣֤W5500�����߿��Է���Internet
* 							�����Զ���ȡIP��ַ��Ҳ����ʹ��Ĭ��ָ����IP��ַ
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

#include "http_client.h"

int main(void)
{ 	
    
	systick_init(72);				            /*��ʼ��Systick����ʱ��*/
	USART1_Config(); 				            /*��ʼ������ͨ��:115200@8-n-1*/
	i2c_CfgGpio();				    	        /*��ʼ��eeprom*/
	DHT11_GPIO_Config();								/*��ʼ��dht11��ʪ�ȴ�����*/	
	gpio_for_w5500_config();	         	/*��ʼ��MCU�������*/
	reset_w5500();					            /*Ӳ��λW5500*/
	set_w5500_mac();                    /*����MAC��ַ*/
	set_w5500_ip(); 										/*����IP��ַ*/
	socket_buf_init(txsize, rxsize);    /*��ʼ��8��Socket�ķ��ͽ��ջ����С*/
	
	printf(" YEElink������IPΪ:%d.%d.%d.%d\r\n",yeelink_server_ip[0],yeelink_server_ip[1],yeelink_server_ip[2],yeelink_server_ip[3]);
	printf(" YEElink�������˿�Ĭ��Ϊ:%d \r\n",yeelink_port);
	printf(" �ҵ�Yelink IDΪ:%s \r\n",yeelink_id);
	printf(" �ҵ�Yelink PWDΪ:%s \r\n",yeelink_pwd);
	printf(" ���¼�����˺Ź۲�ʵʱ�¶ȱ仯�� ����Ҳ���Բ鿴��ȡ��Ϣ\r\n");
	printf(" Ӧ�ó���ִ���С���\r\n"); 
	while(1)//ѭ��ִ�еĺ��� 
	{ 
		do_http_client();	
	}
} 



