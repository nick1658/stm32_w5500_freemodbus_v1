/**
******************************************************************************
* @file    			main.c
* @author  			WIZnet Software Team
* @version 			V1.0
* @date    			2015-02-14
* @brief   			��3.5.0�汾�⽨�Ĺ���ģ��
* @attention  
*               Ĭ��ʹ��Ұ�𿪷����SPI1�ӿڣ���Ͽ���J10��A4��cs��Ķ̽�ñ
*							
*               �������ԣ��뽲Ұ�������������·������ӡ���ȷ��·������
*               DHCP���ܴ��ڴ򿪵�״̬��
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

int main(void)
{
	uint8 PHY_connect=0;
	systick_init(72);				            /*��ʼ��Systick����ʱ��*/
	USART1_Config(); 				            /*��ʼ������ͨ��:115200@8-n-1*/
	i2c_CfgGpio();				    	        /*��ʼ��eeprom*/
	
	printf("  Ұ����������� DHCP Demo V1.0 \r\n");		
	
	gpio_for_w5500_config();	         	/*��ʼ��MCU�������*/
	//SPI_FLASH_Init();
	reset_w5500();					            /*Ӳ��λW5500*/
	set_w5500_mac();                    /*����MAC��ַ*/
	
  socket_buf_init(txsize, rxsize);    /*��ʼ��8��Socket�ķ��ͽ��ջ����С*/
	 
	while(1)                            /*ѭ��ִ�еĺ���*/ 
	{
		PHY_connect=0x01&getPHYStatus();
		if(PHY_connect==0)
		{
			PHY_connect=0x01&getPHYStatus();
			while(PHY_connect);
			 
			dhcp_state =STATE_DHCP_READY;
		}
		do_dhcp();                        /*DHCP���Գ���*/
	}
} 


