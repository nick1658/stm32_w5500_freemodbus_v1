#include "usart.h"
#include "device.h"
#include "spi2.h"
#include "ult.h"
#include "socket.h"
#include "w5500.h"
#include "string.h"
#include "config.h"
#include "tftp.h"

TFTP_MSG  tftp;

uint8 Buffer[1024];													// 定义一个1KB的数组，用来存放Socket的通信数据

int main()
{
	uint8 len,ret;	
	uint8 g_socket_rcv_buf[MAX_MTU_SIZE];
	uint8 filename_flag=0;
	uint32 addr;
	uint32 tftp_server;	
  uint32 app_first_4_bytes;
	
	/***** MCU时钟初始化 *****/							  
	Systick_Init(72);	
	
	/***** 中断控制器配置 *****/
	NVIC_Configuration();
	
	/***** GPIO、SPI初始化 *****/
	GPIO_Configuration();			
	WIZ_SPI_Init();
	
	/***** 串口初始化 *****/
	USART1_Init(); 		
	
	/***** 硬重启W5500 *****/
	Reset_W5500();
	
	/***** W5500的IP信息初始化 *****/
	set_default(); 																						// 设置默认IP信息
	set_network();																						// 配置初始化IP信息并打印，初始化8个Socket
	set_w5500_mac();																					// 配置MAC地址

	printf("Remote IP:%d.%d.%d.%d\r\n", remote_ip[0],remote_ip[1],remote_ip[2],remote_ip[3]);
	printf("W5500 Init Complete!\r\n");
  printf("Start TFTP Client Test!\r\n\r\n"); 

  addr = ApplicationAddress;
  app_first_4_bytes = *(vu32*)addr;
	
  if ((app_first_4_bytes & 0xffffffff) == 0xffffffff)				// 若app的前四字节为0x08008000，说明Flash中尚未烧录app代码
  { 
		write_config_to_eeprom(); 															// 向Flash中烧录app
  }	
	get_config();																							// 从Flash中读取配置信息
	TFTP_init(SOCK_TFTP,g_socket_rcv_buf);										// TFTP初始化，并打开一路Socket用作TFTP通信
	
	while(1)
  {
		if((ConfigMsg.newfirm[0]!=ConfigMsg.sw_ver[0])||(ConfigMsg.newfirm[1]!=ConfigMsg.sw_ver[1]))
		{			
			if(USART_RX_STA & 0x8000)  														// 串口输入
			{
				len = USART_RX_STA & 0x3fff;												// 计算串口输入内容的长度
				printf("Send new version app:%s\r\n",USART_RX_BUF);	// 打印输入的内容				
				memcpy(ConfigMsg.filename,USART_RX_BUF,len);				// 若输入的内容即Filename与要读/写的位于服务器上的文件名称相同
				USART_RX_STA=0;																			// 须以0结尾
				memset(USART_RX_BUF,0,len+1);												// USART_RX_BUF清0
				filename_flag=1;																		// Filename标志位置1
			}		
			if(filename_flag==1)																	// 若Filename标志位为1
			{
				filename_flag=0;																		// Filename标志位清0
				printf("TFTP Start!\r\n");
				tftp_server = (remote_ip[0] << 24) | (remote_ip[1] << 16) | (remote_ip[2] << 8) | (remote_ip[3]);		// 设置TFTP服务器IP地址
				TFTP_read_request(tftp_server,ConfigMsg.filename);	// TFTP读请求
				
				while(1)
				{	
					ret = TFTP_run();																	// TFTP进程
					if(ret != TFTP_PROGRESS)													// TFTP进程结束后跳转至TFTP成功
					break;
				}
				if(ret == TFTP_SUCCESS)															// TFTP成功
				{
					printf("TFTP Success!\r\n");								
					Delay_ms(10);
					reboot_app();																			// 重启，运行新版本的app
				}			
			}
		}
		else																										// 若无版本更新，重启旧版本的app
		{
			printf("No new version, run the old version.\r\n");
			reboot_app();																					
		}
	}										
}

