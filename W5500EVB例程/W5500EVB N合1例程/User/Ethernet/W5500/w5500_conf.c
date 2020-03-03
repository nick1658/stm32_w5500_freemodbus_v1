/*
**************************************************************************************************
* @file    		w5500_conf.c
* @author  		WIZnet Software Team 
* @version 		V1.0
* @date    		2015-02-14
* @brief  		����MCU����ֲW5500������Ҫ�޸ĵ��ļ�������W5500��MAC��IP��ַ
**************************************************************************************************
*/
#include <stdio.h> 
#include <string.h>

#include "w5500_conf.h"
#include "bsp_i2c_ee.h"
#include "bsp_spi_flash.h"
#include "bsp_TiMbase.h"

#include "utility.h"
#include "w5500.h"
#include "tcp_demo.h"
#include "udp_demo.h"
#include "http_client.h"
#include "dhcp.h"
#include "smtp.h"
#include "weibo.h"
#include "ntp.h"
#include "ping.h"
#include "dns.h"
CONFIG_MSG  ConfigMsg;																	/*���ýṹ��*/
EEPROM_MSG_STR EEPROM_MSG;															/*EEPROM�洢��Ϣ�ṹ��*/
/***************����ʱ��Ҫ�޸�Ϊ����PC����IP��ַ******************/
/*����Զ��IP��Ϣ*/
uint8  remote_ip[4]={192,168,1,103};											/*Զ��IP��ַ*/
uint16 remote_port=5000;																/*Զ�˶˿ں�*/

/*����MAC��ַ,������W5500�����������ͬһ�ֳ���������ʹ�ò�ͬ��MAC��ַ*/
uint8 mac[6]={0x00,0x08,0xdc,0x11,0x11,0x11};

/*����Ĭ��IP��Ϣ*/
uint8 local_ip[4]={192,168,1,88};												/*����W5500Ĭ��IP��ַ*/
uint8 subnet[4]={255,255,255,0};												/*����W5500Ĭ����������*/
uint8 gateway[4]={192,168,1,1};													/*����W5500Ĭ������*/
uint8 dns_server[4]={114,114,114,114};									/*����W5500Ĭ��DNS*/

uint16 local_port=5000;	                       					/*���屾�ض˿�*/

#define yeelink_id       "wildfire"        //your sina weibo ID
#define yeelink_pwd      "wildfire123"        //your sina weibo password

#define weibo_id        "3196855541@qq.com"        //your sina weibo ID
#define weibo_pwd       "wildfire123"        //your sina weibo password
/**
@brief  This function is for resetting of the iinchip. Initializes the iinchip to work in whether DIRECT or INDIRECT mode
*/
/*IP���÷���ѡ��������ѡ��*/
uint8	ip_from;			
uint8 dhcp_ok=0;																				/*dhcp�ɹ���ȡIP*/
uint32	ms=0;																						/*�������*/
uint32	dhcp_time= 0;																		/*DHCP���м���*/
vu8	ntptimer = 0;																				/*NPT�����*/

/**
*@brief		����W5500IP��ַ
*@param		��
*@return	��
*/
void set_w5500_ip(void)
{	
		
 /*���ƶ����������Ϣ�����ýṹ��*/
	memcpy(ConfigMsg.mac, mac, 6);
	memcpy(ConfigMsg.lip,local_ip,4);
	memcpy(ConfigMsg.sub,subnet,4);
	memcpy(ConfigMsg.gw,gateway,4);
	memcpy(ConfigMsg.dns,dns_server,4);
	if(ip_from==IP_FROM_DEFINE)	
		printf(" ʹ�ö����IP��Ϣ����W5500	\r\n");
	
	/*ʹ��EEPROM�洢��IP����*/	
	if(ip_from==IP_FROM_EEPROM)
	{
		/*��EEPROM�ж�ȡIP������Ϣ*/
		read_config_from_eeprom();		
		
		/*�����ȡEEPROM��MAC��Ϣ,��������ã����ʹ��*/		
		if( *(EEPROM_MSG.mac)==0x00&& *(EEPROM_MSG.mac+1)==0x08&&*(EEPROM_MSG.mac+2)==0xdc)		
		{
			printf(" IP from EEPROM	\r\n");
			/*����EEPROM������Ϣ�����õĽṹ�����*/
			memcpy(ConfigMsg.lip,EEPROM_MSG.lip, 4);				
			memcpy(ConfigMsg.sub,EEPROM_MSG.sub, 4);
			memcpy(ConfigMsg.gw, EEPROM_MSG.gw, 4);
		}
		else
		{
			printf(" EEPROMδ����,ʹ�ö����IP��Ϣ����W5500,��д��EEPROM	\r\n");
			write_config_to_eeprom();	/*ʹ��Ĭ�ϵ�IP��Ϣ������ʼ��EEPROM������*/
		}			
	}

	/*ʹ��DHCP��ȡIP�����������DHCP�Ӻ���*/		
	if(ip_from==IP_FROM_DHCP)								
	{
		/*����DHCP��ȡ��������Ϣ�����ýṹ��*/
		if(dhcp_ok==1)
		{
			printf(" IP from DHCP	\r\n");		 
			memcpy(ConfigMsg.lip,DHCP_GET.lip, 4);
			memcpy(ConfigMsg.sub,DHCP_GET.sub, 4);
			memcpy(ConfigMsg.gw,DHCP_GET.gw, 4);
			memcpy(ConfigMsg.dns,DHCP_GET.dns,4);
		}
		else
		{
			printf(" DHCP�ӳ���δ����,���߲��ɹ�	\r\n");
			printf(" ʹ�ö����IP��Ϣ����W5500	\r\n");
		}
	}
		
	/*����������Ϣ��������Ҫѡ��*/	
	ConfigMsg.sw_ver[0]=FW_VER_HIGH;
	ConfigMsg.sw_ver[1]=FW_VER_LOW;	

	/*��IP������Ϣд��W5500��Ӧ�Ĵ���*/	
	setSUBR(ConfigMsg.sub);
	setGAR(ConfigMsg.gw);
	setSIPR(ConfigMsg.lip);
	
	getSIPR (local_ip);			
	printf(" W5500 IP��ַ   : %d.%d.%d.%d\r\n", local_ip[0],local_ip[1],local_ip[2],local_ip[3]);
	getSUBR(subnet);
	printf(" W5500 �������� : %d.%d.%d.%d\r\n", subnet[0],subnet[1],subnet[2],subnet[3]);
	getGAR(gateway);
	printf(" W5500 ����     : %d.%d.%d.%d\r\n", gateway[0],gateway[1],gateway[2],gateway[3]);
}
/**
*@brief		w5500��ҳ����ר��
*@param		��
*@return	��
*/
void set_network(void)
{
	setSUBR(ConfigMsg.sub);
	setGAR(ConfigMsg.gw);
	setSIPR(ConfigMsg.lip);
}	
/**
*@brief		����W5500MAC��ַ
*@param		��
*@return	��
*/
void set_w5500_mac(void)
{
	memcpy(ConfigMsg.mac, mac, 6);
	setSHAR(ConfigMsg.mac);	/**/
	memcpy(DHCP_GET.mac, mac, 6);
}

/**
*@brief		����W5500��ص�GPIO
*@param		��
*@return	��
					PE5 W5500INT  PE6 W5500RST
*/
void gpio_for_w5500_config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	SPI_FLASH_Init();																			 /*��ʼ��STM32 SPI1�ӿ�*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE|RCC_APB2Periph_GPIOG, ENABLE);

	/*ʹ��Ұ��ISO��׼������*/
	#ifdef STM32F103ZET6
		/*����RESET����*/
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;					 /*ѡ��Ҫ���Ƶ�GPIO����*/		 
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 /*������������Ϊ50MHz */		
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		 /*��������ģʽΪͨ���������*/	
		GPIO_Init(GPIOG, &GPIO_InitStructure);							 /*���ÿ⺯������ʼ��GPIO*/
		GPIO_SetBits(GPIOG, GPIO_Pin_15);		
		/*����INT����*/	
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;						 /*ѡ��Ҫ���Ƶ�GPIO����*/		 
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 /*������������Ϊ50MHz*/		
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;				 /*��������ģʽΪͨ������ģ����������*/		
		GPIO_Init(GPIOG, &GPIO_InitStructure);							 /*���ÿ⺯������ʼ��GPIO*/
	#endif
	
	/*ʹ��Ұ��MINI������*/
	#ifdef STM32F103VET6
		/*����RESET����*/	
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;						 /*ѡ��Ҫ���Ƶ�GPIO����*/		 
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 /*������������Ϊ50MHz*/		
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		 /*��������ģʽΪͨ���������*/		
		GPIO_Init(GPIOE, &GPIO_InitStructure);							 /*���ÿ⺯������ʼ��GPIO*/
		GPIO_SetBits(GPIOE, GPIO_Pin_6);		
		/*����INT����*/	
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;						 /*ѡ��Ҫ���Ƶ�GPIO����*/		 
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 /*������������Ϊ50MHz */		
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;				 /*��������ģʽΪͨ������ģ����������*/	
		GPIO_Init(GPIOE, &GPIO_InitStructure);							 /*���ÿ⺯������ʼ��GPIO*/
	#endif
}

/**
*@brief		W5500Ƭѡ�ź����ú���
*@param		val: 0 Ƭѡ�˿�Ϊ�ͣ�1 Ƭѡ�˿�Ϊ��
*@return	��
*/
void wiz_cs(uint8_t val)
{
	if (val == LOW) 
	{
		GPIO_ResetBits(GPIOA, WIZ_SCS); 
	}
	else if (val == HIGH)
	{
		GPIO_SetBits(GPIOA, WIZ_SCS); 
	}
}

/**
*@brief		����W5500��Ƭѡ�˿�SCSnΪ��
*@param		��
*@return	��
*/
void iinchip_csoff(void)
{
	wiz_cs(LOW);
}

/**
*@brief		����W5500��Ƭѡ�˿�SCSnΪ��
*@param		��
*@return	��
*/
void iinchip_cson(void)
{	
   wiz_cs(HIGH);
}

/**
*@brief		W5500��λ���ú���
*@param		��
*@return	��
*/
void reset_w5500(void)
{
	/*ʹ��Ұ��ISO��׼������*/
	#ifdef STM32F103ZET6
		GPIO_ResetBits(GPIOG, WIZ_RESET);
		delay_us(2);  
		GPIO_SetBits(GPIOG, WIZ_RESET);
		delay_ms(1600);
	#endif
	
	/*ʹ��Ұ��MINI������*/
	#ifdef STM32F103VET6
		GPIO_ResetBits(GPIOE, WIZ_RESET);
		delay_us(2);  
		GPIO_SetBits(GPIOE, WIZ_RESET);
		delay_ms(1600);	
	#endif
}

/**
*@brief		STM32 SPI1��д8λ����
*@param		dat��д���8λ����
*@return	��
*/
uint8  IINCHIP_SpiSendData(uint8 dat)
{
   return(SPI_FLASH_SendByte(dat));
}

/**
*@brief		д��һ��8λ���ݵ�W5500
*@param		addrbsb: д�����ݵĵ�ַ�� data��д���8λ����
*@return	��
*/
void IINCHIP_WRITE( uint32 addrbsb,  uint8 data)
{
   iinchip_csoff();                              // CS=0, SPI����
   IINCHIP_SpiSendData( (addrbsb & 0x00FF0000)>>16);// ��ַ��8λ
   IINCHIP_SpiSendData( (addrbsb & 0x0000FF00)>> 8);// ��ַ��8λ
   IINCHIP_SpiSendData( (addrbsb & 0x000000F8) + 4);  // 8���ݶ�д�������λ��д������ݳ���Ϊ1
   IINCHIP_SpiSendData(data);                    // д����
   iinchip_cson();                               // CS=1,  SPI����
}

/**
*@brief		��W5500����һ��8λ����
*@param		addrbsb: д�����ݵĵ�ַ
*@return	data����д��ĵ�ַ����ȡ����8λ����
*/
uint8 IINCHIP_READ(uint32 addrbsb)
{
   uint8 data = 0;
   iinchip_csoff();                             // CS=0, SPI����
   IINCHIP_SpiSendData( (addrbsb & 0x00FF0000)>>16);//��ַ��8λ
   IINCHIP_SpiSendData( (addrbsb & 0x0000FF00)>> 8);// ��ַ��8λ
   IINCHIP_SpiSendData( (addrbsb & 0x000000F8))    ;// 8���ݶ�д�������λ����������ݳ���Ϊ1
   data = IINCHIP_SpiSendData(0x00);             // ������
   iinchip_cson();                                // CS=1,  SPI����
   return data;    
}

/**
*@brief		��W5500д��len�ֽ�����
*@param		addrbsb: д�����ݵĵ�ַ  buf��д���ַ���  len���ַ�������
*@return	len�������ַ�������
*/
uint16 wiz_write_buf(uint32 addrbsb,uint8* buf,uint16 len)
{
   uint16 idx = 0;
   if(len == 0) printf("Unexpected2 length 0\r\n");
   iinchip_csoff();                                  // CS=0, SPI����
   IINCHIP_SpiSendData( (addrbsb & 0x00FF0000)>>16);//��ַ��8λ
   IINCHIP_SpiSendData( (addrbsb & 0x0000FF00)>> 8);//��ַ��8λ
   IINCHIP_SpiSendData( (addrbsb & 0x000000F8) + 4);  // 8���ݶ�д�������λ��д������ݳ���Ϊ1
   for(idx = 0; idx < len; idx++)                //ѭ��д��������
   {
     IINCHIP_SpiSendData(buf[idx]);//д����
   }
   iinchip_cson();                               // CS=1, SPI����
   return len;  
}

/**
*@brief		��W5500����len�ֽ�����
*@param		addrbsb: ��ȡ���ݵĵ�ַ  buf����Ŷ�ȡ���� len���ַ�������
*@return	len�������ַ�������
*/
uint16 wiz_read_buf(uint32 addrbsb, uint8* buf,uint16 len)
{
  uint16 idx = 0;
  if(len == 0)
  {
    printf("Unexpected2 length 0\r\n");
  }
  //SPI MODE I/F
  iinchip_csoff();                                  // CS=0, SPI ����
  IINCHIP_SpiSendData( (addrbsb & 0x00FF0000)>>16);// ��ַ��8λ
  IINCHIP_SpiSendData( (addrbsb & 0x0000FF00)>> 8);// ��ַ��8λ
  IINCHIP_SpiSendData( (addrbsb & 0x000000F8));    // 8λ��д������ƶΣ���������ݳ���1
  for(idx = 0; idx < len; idx++)                    // ѭ����ȡ�������
  {
    buf[idx] = IINCHIP_SpiSendData(0x00);
  }
  iinchip_cson();                                   // CS=1, SPI ����
  return len;
}

/**
*@brief		д������Ϣ��EEPROM
*@param		��
*@return	��
*/
void write_config_to_eeprom(void)
{
	uint16 dAddr=0;
	ee_WriteBytes(ConfigMsg.mac,dAddr,(uint8)EEPROM_MSG_LEN);				
	delay_ms(10);																							
}

/**
*@brief		��EEPROM��������Ϣ
*@param		��
*@return	��
*/
void read_config_from_eeprom(void)
{
	ee_ReadBytes(EEPROM_MSG.mac,0,EEPROM_MSG_LEN);
	delay_us(10);
}
/**
*@brief		STM32ϵͳ��λ����
*@param		��
*@return	��
*/
void reboot(void)
{
  pFunction Jump_To_Application;
  uint32 JumpAddress;
   printf(" ϵͳ������, ������ѡ��Ӧ�ó��� \r\n");
  JumpAddress = *(vu32*) (0x00000004);
  Jump_To_Application = (pFunction) JumpAddress;
  Jump_To_Application();
}

/**
*@brief		��ȡ�������ݼĴ���
*@param		*key��	
*@return	1���õ������������ݣ� 0��δ�õ�������������
*/
u8 serial_key_pressed(char *key)
{
	FlagStatus flag  ;
	/* First clear Rx buffer */
	flag = USART_GetFlagStatus(USART1, USART_FLAG_RXNE);
	if ( flag == SET)
	{
		*key = (char)USART1->DR;
		return 1;
	}
	else
	{
		return 0;
	}
}

/**
*@brief		��ȡ�Ӵ���������ַ�
*@param		��
*@return	key���Ӵ��ڵõ����ַ�
*/
char get_key(void)
{
	char key = 0;
	/* Waiting for user input */
	while (1)
	{
		if (serial_key_pressed((char*)&key)) break;
	}
	return key;
}

/**
*@brief		STM32��ʱ��2��ʼ��
*@param		��
*@return	��
*/
void timer2_init(void)
{
	TIM2_Configuration();																		/* TIM2 ��ʱ���� */
	TIM2_NVIC_Configuration();															/* ��ʱ�����ж����ȼ� */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 , ENABLE);		/* TIM2 ���¿�ʱ�ӣ���ʼ��ʱ */
}

/**
*@brief		dhcp�õ��Ķ�ʱ����ʼ��
*@param		��
*@return	��
*/
void dhcp_timer_init(void)
{
			timer2_init();																	
}

/**
*@brief		ntp�õ��Ķ�ʱ����ʼ��
*@param		��
*@return	��
*/
void ntp_timer_init(void)
{
			timer2_init();																	
}

/**
*@brief		��ʱ��2�жϺ���
*@param		��
*@return	��
*/
void timer2_isr(void)
{
	ms++;	
  if(ms>=1000)
  {  
    ms=0;
    dhcp_time++;																					/*DHCP��ʱ��1S*/
//		#ifndef	__NTP_H__
		ntptimer++;																						/*NTP����ʱ���1S*/
//		#endif
  }

}

/**
*@brief		���Գ���ѡ��
*@param		��
*@return	temp�����شӴ��ڵõ�������
*/
uint8 demo_select(void)
{		
		uint8  input_num=0,input_ok=0;
		uint8  temp;
		printf(" \r\n");
		printf(" ===========WIZnet W5500 ��������==============\r\n");
		printf("             ѡ��˵� :\r\n");
		printf(" ==============================================\r\n");
		printf(" 1 - TCP Server    ���ݻػ����� \r\n");
		printf(" 2 - TCP Client    ���ݻػ����� \r\n");
		printf(" 3 - UDP           ���ݻػ����� \r\n");
		printf(" 4 - DHCP          ���Գ��� \r\n");
		printf(" 5 - DNS           ���Գ��� \r\n");
		printf(" 6 - SMTP          ���Գ��� \r\n");
		printf(" 7 - HTTP Server   ���Գ��� \r\n");
		printf(" 8 - HTTP Client   ���Գ��� \r\n");
		printf(" 9 - NETBIOS       ���Գ��� \r\n");
		printf(" a - NTP           ���Գ��� \r\n");
		printf(" b - WEIBO         ���Գ��� \r\n");
		printf(" c - PING          ���Գ��� \r\n");
		printf(" ������1-c֮������֣�ѡ���Ӧ��Ӧ�ó���\r\n");
		printf(" ������Q�˳�Ӧ�ó��� \r\n");
		/* �ȴ�����1�������� */
		while(input_ok==0)
		{
	   		while (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET);//�ȴ����ڽ��ձ�־��λ
			input_num=USART_ReceiveData(USART1); //���մ�������
			if(input_num>0x30&&input_num<0x3c)//  �жϽ���������0����9֮��
			{
			input_ok=1;
			temp=input_num-0x30;
			}
			if(input_num>0x60&&input_num<0x7a)//�жϽ��մ�������ΪСд��ĸa-z
			{
			input_ok=1;
			temp=input_num-'a'+10;
			}
			else 
			printf(" ������1-c֮������֣�ѡ���Ӧ��Ӧ�ó���: \r\n");

		}
		printf(" ��ѡ����Ե�Ӧ�ó���Ϊ��\r\n");

		switch(temp)
		{
			case 1:
				printf(" 1 - TCP Server    ���ݻػ����� \r\n");
				printf(" Ұ�������������ΪTCP �������������������ȴ�PC��ΪTCP Client�������� \r\n");
				printf(" W5500�����˿�Ϊ�� %d \r\n",local_port);
				printf(" ���ӳɹ���TCP Client�������ݸ�W5500��W5500�����ض�Ӧ���� \r\n");
			break;
		 	case 2:
				printf(" 2 - TCP Client    ���ݻػ����� \r\n");
				printf(" ������ΪTCP������,��W5500��Ϊ TCP�ͻ���ȥ���� \r\n");
				printf(" PC������IPĬ��Ϊ:%d.%d.%d.%d\r\n",remote_ip[0],remote_ip[1],remote_ip[2],remote_ip[3]);
				printf(" PC�������˿�Ĭ��Ϊ:%d \r\n",remote_port);
				printf(" ʵ���������ӳɹ���PC���������ݸ�W5500��W5500�����ض�Ӧ����\r\n");
			break;
			case 3:
		    printf(" 3 - UDP           ���ݻػ����� \r\n");
				printf(" W5500���Ժ͵��Ե�UDP�˿�ͨѶ \r\n");
				printf(" W5500�ı��ض˿�Ϊ:%d \r\n",local_port);
				printf(" Զ�˶˿�Ϊ:%d \r\n",remote_port);
				printf(" ʵ���������ӳɹ���PC���������ݸ�W5500��W5500�����ض�Ӧ���� \r\n");
			break;
			case 4:
					ip_from=IP_FROM_DHCP; //ʹ��DHCP ip��ַ
		      printf(" 4 - DHCP          ���Գ���	\r\n");
					printf(" Ұ�������������ΪDHCP�ͻ��ˣ����Դ�DHCP��������ȡIP��ַ \r\n");
			   	printf(" W5500����ͨ��·�����Զ���ȡIP	\r\n");
			break;			
			case 5:
				printf(" 5 - DNS           ���Գ���	\r\n");
				printf(" ����%s��IP\r\n",domain_name); 
			break;	
				case 6:				
				printf(" 6 - SMTP       	���Գ���	\r\n");
				printf(" Ĭ�Ͻ�����smtp.126.com��IP	\r\n");
				printf(" ����ǰ����ռ��������ַ��Ϊ�Լ�������	\r\n");
				printf(" ����������Ϊ:%s \r\n",from);
				printf(" �ռ�������Ϊ:%s \r\n",to);				   
			break;
			case 7:
				ip_from=IP_FROM_EEPROM;
				printf(" 7 - HTTP Server    ���Գ���	\r\n");
				printf(" ��IE�����������W5500��IP�Ϳɷ���web Server	\r\n");
			
			break;
			case 8:
				printf(" 8 - HTTP Client ���Գ���	\r\n");
				printf(" YEElink������IPΪ:%d.%d.%d.%d\r\n",yeelink_server_ip[0],yeelink_server_ip[1],yeelink_server_ip[2],yeelink_server_ip[3]);
				printf(" YEElink�������˿�Ĭ��Ϊ:%d \r\n",yeelink_port);
				printf(" �ҵ�Yelink IDΪ:%s \r\n",yeelink_id);
				printf(" �ҵ�Yelink PWDΪ:%s \r\n",yeelink_pwd);
				printf(" ���¼�����˺Ź۲�ʵʱ��ʪ�ȱ仯	\r\n");
			break;
			case 9:
			  ip_from=IP_FROM_DHCP; //ʹ��DHCP ip��ַ
				printf(" 9 - NETBIOS       ���Գ���	\r\n");
				printf(" ��Dos������������ping W5500���ɻ�ÿ������IP	\r\n");
			break;

			case 10:
				ip_from=IP_FROM_DHCP; //ʹ��DHCP ip
				printf(" a - NTP       	���Գ���	\r\n");
				printf(" NTP������IPΪ:%d.%d.%d.%d\r\n",ntp_server_ip[0],ntp_server_ip[1],ntp_server_ip[2],ntp_server_ip[3]);
				printf(" NTP�������˿�Ĭ��Ϊ:%d \r\n",ntp_port);
			break;
			case 11:
				printf(" c - WEIBO       ���Գ���	\r\n");
				printf(" WEIBO������IPΪ:%d.%d.%d.%d\r\n",weibo_server_ip[0],weibo_server_ip[1],weibo_server_ip[2],weibo_server_ip[3]);
				printf(" �ҵ�΢��IDΪ:%s \r\n",weibo_id);
				printf(" �ҵ�΢��PWDΪ:%s \r\n",weibo_pwd);
				printf(" �����ڵ�����Ϣ��rev255: ok ˵���ϴ��ɹ�, ��½����΢���˺žͿɿ�������΢����Ϣ \r\n");
				
			break;

			case 12:
				printf(" c - PING       ���Գ���	\r\n");
				printf(" Ping������IPΪ:%d.%d.%d.%d\r\n",remote_ip[0],remote_ip[1],remote_ip[2],remote_ip[3]);
			break;
		   	default:
			break;		
		}
		
		return temp;
}


