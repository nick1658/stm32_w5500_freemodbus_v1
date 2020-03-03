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

#include "w5500.h"
#include "W5500_conf.h"
#include "socket.h"
#include "utility.h"
/*app����ͷ�ļ�*/
#include "dhcp.h"
#include "ntp.h"

int main(void)
{ 	
    
	systick_init(72);				            /*��ʼ��Systick����ʱ��*/
	USART1_Config(); 				            /*��ʼ������ͨ��:115200@8-n-1*/
	i2c_CfgGpio();				    	        /*��ʼ��eeprom*/	
	gpio_for_w5500_config();	         	/*��ʼ��MCU�������*/
	reset_w5500();					            /*Ӳ��λW5500*/
	set_w5500_mac();                    /*����MAC��ַ*/
	
  socket_buf_init(txsize, rxsize);    /*��ʼ��8��Socket�ķ��ͽ��ջ����С*/
	
	printf(" NTP������IPΪ:%d.%d.%d.%d\r\n",ntp_server_ip[0],ntp_server_ip[1],ntp_server_ip[2],ntp_server_ip[3]);
	printf(" NTP�������˿�Ϊ:%d \r\n",ntp_port);
	ntp_client_init();                  /*NTP��ʼ��*/ 
	while(1)
	{ 	
		do_dhcp();                        /*��DHCP��������ȡ������IP��ַ*/
		if(dhcp_ok==1)										/*�ɹ���ȡIP��ַ*/
		{
			do_ntp_client();
		} 
	}

} 



