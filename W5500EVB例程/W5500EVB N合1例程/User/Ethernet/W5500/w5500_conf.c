/*
**************************************************************************************************
* @file    		w5500_conf.c
* @author  		WIZnet Software Team 
* @version 		V1.0
* @date    		2015-02-14
* @brief  		配置MCU，移植W5500程序需要修改的文件，配置W5500的MAC和IP地址
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
CONFIG_MSG  ConfigMsg;																	/*配置结构体*/
EEPROM_MSG_STR EEPROM_MSG;															/*EEPROM存储信息结构体*/
/***************测试时需要修改为测试PC机的IP地址******************/
/*定义远端IP信息*/
uint8  remote_ip[4]={192,168,1,103};											/*远端IP地址*/
uint16 remote_port=5000;																/*远端端口号*/

/*定义MAC地址,如果多块W5500网络适配板在同一现场工作，请使用不同的MAC地址*/
uint8 mac[6]={0x00,0x08,0xdc,0x11,0x11,0x11};

/*定义默认IP信息*/
uint8 local_ip[4]={192,168,1,88};												/*定义W5500默认IP地址*/
uint8 subnet[4]={255,255,255,0};												/*定义W5500默认子网掩码*/
uint8 gateway[4]={192,168,1,1};													/*定义W5500默认网关*/
uint8 dns_server[4]={114,114,114,114};									/*定义W5500默认DNS*/

uint16 local_port=5000;	                       					/*定义本地端口*/

#define yeelink_id       "wildfire"        //your sina weibo ID
#define yeelink_pwd      "wildfire123"        //your sina weibo password

#define weibo_id        "3196855541@qq.com"        //your sina weibo ID
#define weibo_pwd       "wildfire123"        //your sina weibo password
/**
@brief  This function is for resetting of the iinchip. Initializes the iinchip to work in whether DIRECT or INDIRECT mode
*/
/*IP配置方法选择，请自行选择*/
uint8	ip_from;			
uint8 dhcp_ok=0;																				/*dhcp成功获取IP*/
uint32	ms=0;																						/*毫秒计数*/
uint32	dhcp_time= 0;																		/*DHCP运行计数*/
vu8	ntptimer = 0;																				/*NPT秒计数*/

/**
*@brief		配置W5500IP地址
*@param		无
*@return	无
*/
void set_w5500_ip(void)
{	
		
 /*复制定义的配置信息到配置结构体*/
	memcpy(ConfigMsg.mac, mac, 6);
	memcpy(ConfigMsg.lip,local_ip,4);
	memcpy(ConfigMsg.sub,subnet,4);
	memcpy(ConfigMsg.gw,gateway,4);
	memcpy(ConfigMsg.dns,dns_server,4);
	if(ip_from==IP_FROM_DEFINE)	
		printf(" 使用定义的IP信息配置W5500	\r\n");
	
	/*使用EEPROM存储的IP参数*/	
	if(ip_from==IP_FROM_EEPROM)
	{
		/*从EEPROM中读取IP配置信息*/
		read_config_from_eeprom();		
		
		/*如果读取EEPROM中MAC信息,如果已配置，则可使用*/		
		if( *(EEPROM_MSG.mac)==0x00&& *(EEPROM_MSG.mac+1)==0x08&&*(EEPROM_MSG.mac+2)==0xdc)		
		{
			printf(" IP from EEPROM	\r\n");
			/*复制EEPROM配置信息到配置的结构体变量*/
			memcpy(ConfigMsg.lip,EEPROM_MSG.lip, 4);				
			memcpy(ConfigMsg.sub,EEPROM_MSG.sub, 4);
			memcpy(ConfigMsg.gw, EEPROM_MSG.gw, 4);
		}
		else
		{
			printf(" EEPROM未配置,使用定义的IP信息配置W5500,并写入EEPROM	\r\n");
			write_config_to_eeprom();	/*使用默认的IP信息，并初始化EEPROM中数据*/
		}			
	}

	/*使用DHCP获取IP参数，需调用DHCP子函数*/		
	if(ip_from==IP_FROM_DHCP)								
	{
		/*复制DHCP获取的配置信息到配置结构体*/
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
			printf(" DHCP子程序未运行,或者不成功	\r\n");
			printf(" 使用定义的IP信息配置W5500	\r\n");
		}
	}
		
	/*以下配置信息，根据需要选用*/	
	ConfigMsg.sw_ver[0]=FW_VER_HIGH;
	ConfigMsg.sw_ver[1]=FW_VER_LOW;	

	/*将IP配置信息写入W5500相应寄存器*/	
	setSUBR(ConfigMsg.sub);
	setGAR(ConfigMsg.gw);
	setSIPR(ConfigMsg.lip);
	
	getSIPR (local_ip);			
	printf(" W5500 IP地址   : %d.%d.%d.%d\r\n", local_ip[0],local_ip[1],local_ip[2],local_ip[3]);
	getSUBR(subnet);
	printf(" W5500 子网掩码 : %d.%d.%d.%d\r\n", subnet[0],subnet[1],subnet[2],subnet[3]);
	getGAR(gateway);
	printf(" W5500 网关     : %d.%d.%d.%d\r\n", gateway[0],gateway[1],gateway[2],gateway[3]);
}
/**
*@brief		w5500网页配置专用
*@param		无
*@return	无
*/
void set_network(void)
{
	setSUBR(ConfigMsg.sub);
	setGAR(ConfigMsg.gw);
	setSIPR(ConfigMsg.lip);
}	
/**
*@brief		配置W5500MAC地址
*@param		无
*@return	无
*/
void set_w5500_mac(void)
{
	memcpy(ConfigMsg.mac, mac, 6);
	setSHAR(ConfigMsg.mac);	/**/
	memcpy(DHCP_GET.mac, mac, 6);
}

/**
*@brief		配置W5500相关的GPIO
*@param		无
*@return	无
					PE5 W5500INT  PE6 W5500RST
*/
void gpio_for_w5500_config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	SPI_FLASH_Init();																			 /*初始化STM32 SPI1接口*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE|RCC_APB2Periph_GPIOG, ENABLE);

	/*使用野火ISO标准开发板*/
	#ifdef STM32F103ZET6
		/*定义RESET引脚*/
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;					 /*选择要控制的GPIO引脚*/		 
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 /*设置引脚速率为50MHz */		
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		 /*设置引脚模式为通用推挽输出*/	
		GPIO_Init(GPIOG, &GPIO_InitStructure);							 /*调用库函数，初始化GPIO*/
		GPIO_SetBits(GPIOG, GPIO_Pin_15);		
		/*定义INT引脚*/	
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;						 /*选择要控制的GPIO引脚*/		 
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 /*设置引脚速率为50MHz*/		
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;				 /*设置引脚模式为通用推挽模拟上拉输入*/		
		GPIO_Init(GPIOG, &GPIO_InitStructure);							 /*调用库函数，初始化GPIO*/
	#endif
	
	/*使用野火MINI开发板*/
	#ifdef STM32F103VET6
		/*定义RESET引脚*/	
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;						 /*选择要控制的GPIO引脚*/		 
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 /*设置引脚速率为50MHz*/		
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		 /*设置引脚模式为通用推挽输出*/		
		GPIO_Init(GPIOE, &GPIO_InitStructure);							 /*调用库函数，初始化GPIO*/
		GPIO_SetBits(GPIOE, GPIO_Pin_6);		
		/*定义INT引脚*/	
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;						 /*选择要控制的GPIO引脚*/		 
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 /*设置引脚速率为50MHz */		
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;				 /*设置引脚模式为通用推挽模拟上拉输入*/	
		GPIO_Init(GPIOE, &GPIO_InitStructure);							 /*调用库函数，初始化GPIO*/
	#endif
}

/**
*@brief		W5500片选信号设置函数
*@param		val: 0 片选端口为低，1 片选端口为高
*@return	无
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
*@brief		设置W5500的片选端口SCSn为低
*@param		无
*@return	无
*/
void iinchip_csoff(void)
{
	wiz_cs(LOW);
}

/**
*@brief		设置W5500的片选端口SCSn为高
*@param		无
*@return	无
*/
void iinchip_cson(void)
{	
   wiz_cs(HIGH);
}

/**
*@brief		W5500复位设置函数
*@param		无
*@return	无
*/
void reset_w5500(void)
{
	/*使用野火ISO标准开发板*/
	#ifdef STM32F103ZET6
		GPIO_ResetBits(GPIOG, WIZ_RESET);
		delay_us(2);  
		GPIO_SetBits(GPIOG, WIZ_RESET);
		delay_ms(1600);
	#endif
	
	/*使用野火MINI开发板*/
	#ifdef STM32F103VET6
		GPIO_ResetBits(GPIOE, WIZ_RESET);
		delay_us(2);  
		GPIO_SetBits(GPIOE, WIZ_RESET);
		delay_ms(1600);	
	#endif
}

/**
*@brief		STM32 SPI1读写8位数据
*@param		dat：写入的8位数据
*@return	无
*/
uint8  IINCHIP_SpiSendData(uint8 dat)
{
   return(SPI_FLASH_SendByte(dat));
}

/**
*@brief		写入一个8位数据到W5500
*@param		addrbsb: 写入数据的地址， data：写入的8位数据
*@return	无
*/
void IINCHIP_WRITE( uint32 addrbsb,  uint8 data)
{
   iinchip_csoff();                              // CS=0, SPI启动
   IINCHIP_SpiSendData( (addrbsb & 0x00FF0000)>>16);// 地址高8位
   IINCHIP_SpiSendData( (addrbsb & 0x0000FF00)>> 8);// 地址低8位
   IINCHIP_SpiSendData( (addrbsb & 0x000000F8) + 4);  // 8数据读写命令控制位；写命令，数据长度为1
   IINCHIP_SpiSendData(data);                    // 写数据
   iinchip_cson();                               // CS=1,  SPI结束
}

/**
*@brief		从W5500读出一个8位数据
*@param		addrbsb: 写入数据的地址
*@return	data：从写入的地址处读取到的8位数据
*/
uint8 IINCHIP_READ(uint32 addrbsb)
{
   uint8 data = 0;
   iinchip_csoff();                             // CS=0, SPI启动
   IINCHIP_SpiSendData( (addrbsb & 0x00FF0000)>>16);//地址高8位
   IINCHIP_SpiSendData( (addrbsb & 0x0000FF00)>> 8);// 地址低8位
   IINCHIP_SpiSendData( (addrbsb & 0x000000F8))    ;// 8数据读写命令控制位；读命令，数据长度为1
   data = IINCHIP_SpiSendData(0x00);             // 读数据
   iinchip_cson();                                // CS=1,  SPI结束
   return data;    
}

/**
*@brief		向W5500写入len字节数据
*@param		addrbsb: 写入数据的地址  buf：写入字符串  len：字符串长度
*@return	len：返回字符串长度
*/
uint16 wiz_write_buf(uint32 addrbsb,uint8* buf,uint16 len)
{
   uint16 idx = 0;
   if(len == 0) printf("Unexpected2 length 0\r\n");
   iinchip_csoff();                                  // CS=0, SPI启动
   IINCHIP_SpiSendData( (addrbsb & 0x00FF0000)>>16);//地址高8位
   IINCHIP_SpiSendData( (addrbsb & 0x0000FF00)>> 8);//地址低8位
   IINCHIP_SpiSendData( (addrbsb & 0x000000F8) + 4);  // 8数据读写命令控制位；写命令，数据长度为1
   for(idx = 0; idx < len; idx++)                //循环写入多个数据
   {
     IINCHIP_SpiSendData(buf[idx]);//写数据
   }
   iinchip_cson();                               // CS=1, SPI结束
   return len;  
}

/**
*@brief		从W5500读出len字节数据
*@param		addrbsb: 读取数据的地址  buf：存放读取数据 len：字符串长度
*@return	len：返回字符串长度
*/
uint16 wiz_read_buf(uint32 addrbsb, uint8* buf,uint16 len)
{
  uint16 idx = 0;
  if(len == 0)
  {
    printf("Unexpected2 length 0\r\n");
  }
  //SPI MODE I/F
  iinchip_csoff();                                  // CS=0, SPI 启动
  IINCHIP_SpiSendData( (addrbsb & 0x00FF0000)>>16);// 地址高8位
  IINCHIP_SpiSendData( (addrbsb & 0x0000FF00)>> 8);// 地址低8位
  IINCHIP_SpiSendData( (addrbsb & 0x000000F8));    // 8位读写命令控制段；读命令，数据长度1
  for(idx = 0; idx < len; idx++)                    // 循环读取多个数据
  {
    buf[idx] = IINCHIP_SpiSendData(0x00);
  }
  iinchip_cson();                                   // CS=1, SPI 结束
  return len;
}

/**
*@brief		写配置信息到EEPROM
*@param		无
*@return	无
*/
void write_config_to_eeprom(void)
{
	uint16 dAddr=0;
	ee_WriteBytes(ConfigMsg.mac,dAddr,(uint8)EEPROM_MSG_LEN);				
	delay_ms(10);																							
}

/**
*@brief		从EEPROM读配置信息
*@param		无
*@return	无
*/
void read_config_from_eeprom(void)
{
	ee_ReadBytes(EEPROM_MSG.mac,0,EEPROM_MSG_LEN);
	delay_us(10);
}
/**
*@brief		STM32系统复位函数
*@param		无
*@return	无
*/
void reboot(void)
{
  pFunction Jump_To_Application;
  uint32 JumpAddress;
   printf(" 系统重启后, 请重新选择应用程序 \r\n");
  JumpAddress = *(vu32*) (0x00000004);
  Jump_To_Application = (pFunction) JumpAddress;
  Jump_To_Application();
}

/**
*@brief		读取串口数据寄存器
*@param		*key：	
*@return	1：得到串口输入数据， 0：未得到串口输入数据
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
*@brief		获取从串口输入的字符
*@param		无
*@return	key：从串口得到的字符
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
*@brief		STM32定时器2初始化
*@param		无
*@return	无
*/
void timer2_init(void)
{
	TIM2_Configuration();																		/* TIM2 定时配置 */
	TIM2_NVIC_Configuration();															/* 定时器的中断优先级 */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 , ENABLE);		/* TIM2 重新开时钟，开始计时 */
}

/**
*@brief		dhcp用到的定时器初始化
*@param		无
*@return	无
*/
void dhcp_timer_init(void)
{
			timer2_init();																	
}

/**
*@brief		ntp用到的定时器初始化
*@param		无
*@return	无
*/
void ntp_timer_init(void)
{
			timer2_init();																	
}

/**
*@brief		定时器2中断函数
*@param		无
*@return	无
*/
void timer2_isr(void)
{
	ms++;	
  if(ms>=1000)
  {  
    ms=0;
    dhcp_time++;																					/*DHCP定时加1S*/
//		#ifndef	__NTP_H__
		ntptimer++;																						/*NTP重试时间加1S*/
//		#endif
  }

}

/**
*@brief		测试程序选择
*@param		无
*@return	temp：返回从串口得到的数据
*/
uint8 demo_select(void)
{		
		uint8  input_num=0,input_ok=0;
		uint8  temp;
		printf(" \r\n");
		printf(" ===========WIZnet W5500 测试例程==============\r\n");
		printf("             选择菜单 :\r\n");
		printf(" ==============================================\r\n");
		printf(" 1 - TCP Server    数据回环测试 \r\n");
		printf(" 2 - TCP Client    数据回环测试 \r\n");
		printf(" 3 - UDP           数据回环测试 \r\n");
		printf(" 4 - DHCP          测试程序 \r\n");
		printf(" 5 - DNS           测试程序 \r\n");
		printf(" 6 - SMTP          测试程序 \r\n");
		printf(" 7 - HTTP Server   测试程序 \r\n");
		printf(" 8 - HTTP Client   测试程序 \r\n");
		printf(" 9 - NETBIOS       测试程序 \r\n");
		printf(" a - NTP           测试程序 \r\n");
		printf(" b - WEIBO         测试程序 \r\n");
		printf(" c - PING          测试程序 \r\n");
		printf(" 请输入1-c之间的数字，选择对应的应用程序：\r\n");
		printf(" 请输入Q退出应用程序 \r\n");
		/* 等待串口1输入数据 */
		while(input_ok==0)
		{
	   		while (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET);//等待串口接收标志复位
			input_num=USART_ReceiveData(USART1); //接收串口数据
			if(input_num>0x30&&input_num<0x3c)//  判断接收数据在0——9之间
			{
			input_ok=1;
			temp=input_num-0x30;
			}
			if(input_num>0x60&&input_num<0x7a)//判断接收串口数据为小写字母a-z
			{
			input_ok=1;
			temp=input_num-'a'+10;
			}
			else 
			printf(" 请输入1-c之间的数字，选择对应的应用程序: \r\n");

		}
		printf(" 您选择测试的应用程序为：\r\n");

		switch(temp)
		{
			case 1:
				printf(" 1 - TCP Server    数据回环测试 \r\n");
				printf(" 野火网络适配版作为TCP 服务器，建立侦听，等待PC作为TCP Client建立连接 \r\n");
				printf(" W5500监听端口为： %d \r\n",local_port);
				printf(" 连接成功后，TCP Client发送数据给W5500，W5500将返回对应数据 \r\n");
			break;
		 	case 2:
				printf(" 2 - TCP Client    数据回环测试 \r\n");
				printf(" 电脑作为TCP服务器,让W5500作为 TCP客户端去连接 \r\n");
				printf(" PC服务器IP默认为:%d.%d.%d.%d\r\n",remote_ip[0],remote_ip[1],remote_ip[2],remote_ip[3]);
				printf(" PC服务器端口默认为:%d \r\n",remote_port);
				printf(" 实现现象：连接成功后，PC机发送数据给W5500，W5500将返回对应数据\r\n");
			break;
			case 3:
		    printf(" 3 - UDP           数据回环测试 \r\n");
				printf(" W5500可以和电脑的UDP端口通讯 \r\n");
				printf(" W5500的本地端口为:%d \r\n",local_port);
				printf(" 远端端口为:%d \r\n",remote_port);
				printf(" 实现现象：连接成功后，PC机发送数据给W5500，W5500将返回对应数据 \r\n");
			break;
			case 4:
					ip_from=IP_FROM_DHCP; //使用DHCP ip地址
		      printf(" 4 - DHCP          测试程序	\r\n");
					printf(" 野火网络适配板作为DHCP客户端，尝试从DHCP服务器获取IP地址 \r\n");
			   	printf(" W5500可以通过路由器自动获取IP	\r\n");
			break;			
			case 5:
				printf(" 5 - DNS           测试程序	\r\n");
				printf(" 解析%s的IP\r\n",domain_name); 
			break;	
				case 6:				
				printf(" 6 - SMTP       	测试程序	\r\n");
				printf(" 默认解析的smtp.126.com的IP	\r\n");
				printf(" 测试前请把收件人邮箱地址改为自己的邮箱	\r\n");
				printf(" 发件人邮箱为:%s \r\n",from);
				printf(" 收件人邮箱为:%s \r\n",to);				   
			break;
			case 7:
				ip_from=IP_FROM_EEPROM;
				printf(" 7 - HTTP Server    测试程序	\r\n");
				printf(" 在IE浏览器中输入W5500的IP就可访问web Server	\r\n");
			
			break;
			case 8:
				printf(" 8 - HTTP Client 测试程序	\r\n");
				printf(" YEElink服务器IP为:%d.%d.%d.%d\r\n",yeelink_server_ip[0],yeelink_server_ip[1],yeelink_server_ip[2],yeelink_server_ip[3]);
				printf(" YEElink服务器端口默认为:%d \r\n",yeelink_port);
				printf(" 我的Yelink ID为:%s \r\n",yeelink_id);
				printf(" 我的Yelink PWD为:%s \r\n",yeelink_pwd);
				printf(" 请登录以上账号观察实时温湿度变化	\r\n");
			break;
			case 9:
			  ip_from=IP_FROM_DHCP; //使用DHCP ip地址
				printf(" 9 - NETBIOS       测试程序	\r\n");
				printf(" 在Dos命令行中输入ping W5500，可获得开发板的IP	\r\n");
			break;

			case 10:
				ip_from=IP_FROM_DHCP; //使用DHCP ip
				printf(" a - NTP       	测试程序	\r\n");
				printf(" NTP服务器IP为:%d.%d.%d.%d\r\n",ntp_server_ip[0],ntp_server_ip[1],ntp_server_ip[2],ntp_server_ip[3]);
				printf(" NTP服务器端口默认为:%d \r\n",ntp_port);
			break;
			case 11:
				printf(" c - WEIBO       测试程序	\r\n");
				printf(" WEIBO服务器IP为:%d.%d.%d.%d\r\n",weibo_server_ip[0],weibo_server_ip[1],weibo_server_ip[2],weibo_server_ip[3]);
				printf(" 我的微博ID为:%s \r\n",weibo_id);
				printf(" 我的微博PWD为:%s \r\n",weibo_pwd);
				printf(" 看串口调试信息：rev255: ok 说明上传成功, 登陆以上微博账号就可看到发送微博信息 \r\n");
				
			break;

			case 12:
				printf(" c - PING       测试程序	\r\n");
				printf(" Ping服务器IP为:%d.%d.%d.%d\r\n",remote_ip[0],remote_ip[1],remote_ip[2],remote_ip[3]);
			break;
		   	default:
			break;		
		}
		
		return temp;
}


