/**
******************************************************************************
* @file    			main.c
* @author  			WIZnet Software Team
* @version 			V1.0
* @date    			2015-02-14
* @brief   			��3.5.0�汾�⽨�Ĺ���ģ��
* @attention  	
*								�������ԣ��뱣֤W5500��IP�����PC����ͬһ�����ڣ��Ҳ���ͻ
*               ����������߸�PC��ֱ����������PC���������ӵ�ַIPΪ��̬IP
*								��������Ping W5500_conf.c�ж����remote_ip
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
#include "ping.h"

int main(void)
{ 	
    
	systick_init(72);				            /*��ʼ��Systick����ʱ��*/
	USART1_Config(); 				            /*��ʼ������ͨ��:115200@8-n-1*/
	i2c_CfgGpio();				    	        /*��ʼ��eeprom*/	
	gpio_for_w5500_config();	         	/*��ʼ��MCU�������*/
	reset_w5500();					            /*Ӳ��λW5500*/
	set_w5500_mac();                    /*����MAC��ַ*/
  set_w5500_ip();                     /*����IP��ַ*/
	
  socket_buf_init(txsize, rxsize);    /*��ʼ��8��Socket�ķ��ͽ��ջ����С*/
	printf(" APP running���� \r\n"); 
	while(1)//ѭ��ִ�еĺ��� 
	{ 
		do_ping();
		if(req>=4)
		break;	
	}

} 


