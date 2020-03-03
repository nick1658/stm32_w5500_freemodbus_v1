#include "dhcp.h"
#include "device.h"
#include "usart.h"
#include "config.h"
#include "socket.h"
#include "ult.h"
#include "w5500.h"
#include "24c16.h"
#include "tftp.h"
#include <stdio.h> 
#include <string.h>

CONFIG_MSG  ConfigMsg, RecvMsg;
uint8 txsize[MAX_SOCK_NUM] = {2,2,2,2,2,2,2,2};
uint8 rxsize[MAX_SOCK_NUM] = {2,2,2,2,2,2,2,2};
//public buffer for DHCP, DNS, HTTP
uint8 BUFPUB[1460];
uint16 second=0;
/***************测试时需要修改为测试PC机的IP地址******************/
/*定义远端IP信息*/
uint8  remote_ip[4]={192,168,1,151};							/*远端IP地址*/
uint16 remote_port=6000;										/*远端端口号*/

/*定义MAC地址,如果多块W5500网络适配板在同一现场工作，请使用不同的MAC地址*/
uint8 mac[6]={0x00,0x08,0xdc,0x11,0x11,0x11};

/*定义默认IP信息*/
uint8 local_ip[4]={192,168,1,150};								/*定义W5500默认IP地址*/
uint8 subnet[4]={255,255,255,0};								/*定义W5500默认子网掩码*/
uint8 gateway[4]={192,168,1,1};									/*定义W5500默认网关*/
uint8 dns_server[4]={114,114,114,114};							/*定义W5500默认DNS*/
uint16 local_port=5000;	                       					/*定义本地端口*/

uint8	ip_from_dhcp=0;
uint8 ip_from_define=1;
uint8 dhcp_ok;

void GPIO_Configuration(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA |RCC_APB2Periph_GPIOB |RCC_APB2Periph_GPIOE|RCC_APB2Periph_AFIO , ENABLE);
  // Port A output
  GPIO_InitStructure.GPIO_Pin =GPIO_Pin_0|GPIO_Pin_1| GPIO_Pin_2 |GPIO_Pin_3; 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  
  GPIO_ResetBits(GPIOA, GPIO_Pin_0);
  GPIO_ResetBits(GPIOA, GPIO_Pin_1);
  GPIO_SetBits(GPIOA, GPIO_Pin_2); // led off
  GPIO_SetBits(GPIOA, GPIO_Pin_3); // led off
  // Port B output;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  GPIO_SetBits(GPIOB, GPIO_Pin_9);
	
}
void Reset_W5500(void)
{
  GPIO_ResetBits(GPIOB, WIZ_RESET);
  Delay_us(2);  
  GPIO_SetBits(GPIOB, WIZ_RESET);
  Delay_ms(1600);
}
void reboot(void)
{
  pFunction Jump_To_Application;
  uint32 JumpAddress;
  JumpAddress = *(vu32*) (0x00000004);
  Jump_To_Application = (pFunction) JumpAddress;
  Jump_To_Application();
}

void reboot_app(void)
{
  pFunction Jump_To_Application;
  uint32 JumpAddress;
	JumpAddress = *(vu32*) (ApplicationAddress + 0x00000004);
	Jump_To_Application = (pFunction) JumpAddress;
	//initialize user application's stack pointer
	 __set_MSP(*(__IO uint32_t *)ApplicationAddress);	
	Jump_To_Application();
}

void set_w5500_mac(void)
{
	memcpy(ConfigMsg.mac, mac, 6);
	setSHAR(ConfigMsg.mac);	/**/
	memcpy(DHCP_GET.mac, mac, 6);
}

void set_network(void)
{
  uint8 ip[4];
  setSHAR(ConfigMsg.mac);/*配置Mac地址*/
  setSUBR(ConfigMsg.sub);/*配置子网掩码*/
  setGAR(ConfigMsg.gw);/*配置默认网关*/
  setSIPR(ConfigMsg.lip);/*配置Ip地址*/
  //Init. TX & RX Memory size of w5500
  sysinit(txsize, rxsize); /*初始化8个socket*/  
  setRTR(2000);/*设置溢出时间值*/
  setRCR(3);/*设置最大重新发送次数*/
  getSIPR (ip);
  printf("IP : %d.%d.%d.%d\r\n", ip[0],ip[1],ip[2],ip[3]);
  getSUBR(ip);
  printf("SN : %d.%d.%d.%d\r\n", ip[0],ip[1],ip[2],ip[3]);
  getGAR(ip);
  printf("GW : %d.%d.%d.%d\r\n", ip[0],ip[1],ip[2],ip[3]);
}

void set_default(void)
{
	if(ip_from_define==1)
	{
		memcpy(ConfigMsg.mac, mac,6);
		memcpy(ConfigMsg.lip,local_ip,4);
		memcpy(ConfigMsg.sub,subnet,4);
		memcpy(ConfigMsg.gw,gateway,4);
		memcpy(ConfigMsg.dns,dns_server,4);
	}
	if(ip_from_dhcp==1)								
	{
		if(dhcp_ok==1)
		{	 
			memcpy(ConfigMsg.lip,DHCP_GET.lip,4);
			memcpy(ConfigMsg.sub,DHCP_GET.sub,4);
			memcpy(ConfigMsg.gw,DHCP_GET.gw,4);
			memcpy(ConfigMsg.dns,DHCP_GET.dns,4);
		}
		else
		printf(" DHCP is fail	\r\n");
	}
  ConfigMsg.dhcp=0;
  ConfigMsg.debug=1;
  ConfigMsg.fw_len=0;  
//  ConfigMsg.state=NORMAL_STATE;
  ConfigMsg.sw_ver[0]=0;
  ConfigMsg.sw_ver[1]=0;
  ConfigMsg.newfirm[0]=1;
  ConfigMsg.newfirm[1]=0;			
}

void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	//中断优先级NVIC设置
//	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3中断
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  //先占优先级0级
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  //从优先级3级
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
//	NVIC_Init(&NVIC_InitStructure);  //初始化NVIC寄存器
   //Usart1 NVIC 配置
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
	NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0000);  //only app, no boot included
}

void write_config_to_eeprom(void)
{
  uint8 i,j;
  uint32 dAddr,data;

  FLASH_Unlock();																		// Flash解锁
  FLASH_ClearFlag(FLASH_FLAG_BSY | FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);	// 清除标志位	
  FLASH_ErasePage(ConfigAddr);											// 擦除Flash   
  dAddr = ConfigAddr;																// 

  for (i = 0, j = 0; i < (uint8)(sizeof(ConfigMsg)/4); i++) 		// 向Flash中写入
  {
    data = *(u32*)(ConfigMsg.mac+j);
    FLASH_ProgramWord(dAddr, data);
    dAddr += 4;
    j +=4;
  }
}

void get_config(void)
{
  uint16 i;
  
  uint32 addr = ConfigAddr;
  for (i =0; i < CONFIG_MSG_SIZE; i++) 
  {
    *(ConfigMsg.mac+i) = *(vu8*)addr;
    addr += 1;
  }
	printf("Old app version is V%d.%d\r\n",ConfigMsg.sw_ver[0],ConfigMsg.sw_ver[1]);
	printf("New app version is V%d.%d\r\n",ConfigMsg.newfirm[0],ConfigMsg.newfirm[1]);
	printf("Input [app.bin] to update the new app version.\r\n");
	ConfigMsg.sw_ver[0]=ConfigMsg.newfirm[0];
	ConfigMsg.sw_ver[1]=ConfigMsg.newfirm[1];
		if(ConfigMsg.newfirm[1]==5)
		{
			ConfigMsg.newfirm[0]=2;
			ConfigMsg.newfirm[1]=0;			
		}
		else
		{
			ConfigMsg.newfirm[0]=1;
			ConfigMsg.newfirm[1]+=1;
		}
		write_config_to_eeprom(); 
}

