#include "usart.h"
#include "device.h"
#include "spi2.h"
#include "ult.h"
#include "socket.h"
#include "w5500.h"
#include "24c16.h"
#include "md5.h"
#include <string.h>

extern uint8 txsize[];										// 引用外部变量，声明Socket发送缓存大小
extern uint8 rxsize[];										// 引用外部变量，声明Socket接收缓存大小

uint8 buffer[2048];												// 定义一个2KB的数组，用来存放Socket的通信数据

int main()
{
  uint8 remote_ip[4]={192,168,1,151};			// 配置远程IP地址
	uint16 remote_port=6000;								// 配置远程端口
  uint16 local_port=5000;									// 初始化一个本地端口
  uint16 len=0;

  /***** MCU时钟初始化 *****/							  
	Systick_Init(72);	
	
	/***** GPIO、SPI初始化 *****/
	GPIO_Configuration();			
	WIZ_SPI_Init();
	
	/***** 串口初始化 *****/
	USART1_Init(); 		
	
	/***** 初始化eeprom *****/
  at24c16_init();
	
	/***** 硬重启W5500 *****/
	Reset_W5500();
	
	/***** W5500的IP信息初始化 *****/
	set_default(); 														// 设置默认MAC、IP、GW、SUB、DNS
	set_network();														// 配置初始化IP信息并打印，初始化8个Socket

  printf("UDP Local Port: %d \r\n",local_port); 
	printf("UDP Remote IP: %d.%d.%d.%d \r\n",remote_ip[0],remote_ip[1],remote_ip[2],remote_ip[3]);
  printf("UDP Remote Port: %d \r\n",remote_port);
	printf("W5500 Init Complete!\r\n");
  printf("Start UDP Test!\r\n"); 	

	/*Socket状态机，MCU通过读Sn_SR(0)的值进行判断Socket应该处于何种状态
		Sn_SR状态描述：
		0x00		SOCK_CLOSED
		0x13		SOCK_INIT
		0x14		SOCK_LISTEN
		0x17		SOCK_ESTABLISHED
		0x1C		SOCK_CLOSE_WAIT
		0x22		SOCK_UDP
	*/
	while(1)																												// Socket状态机
	{
		switch(getSn_SR(0))																						// 获取socket0的状态
		{
			case SOCK_UDP:																							// Socket处于初始化完成(打开)状态
					Delay_ms(100);
					if(getSn_IR(0) & Sn_IR_RECV)
					{
						setSn_IR(0, Sn_IR_RECV);															// Sn_IR的RECV位置1
					}
					// 数据回环测试程序：数据从远程上位机发给W5500，W5500接收到数据后再回给远程上位机
					if((len=getSn_RX_RSR(0))>0)
					{ 
						memset(buffer,0,len+1);
						recvfrom(0,buffer, len, remote_ip,&remote_port);			// W5500接收来自远程上位机的数据，并通过SPI发送给MCU
						printf("%s\r\n",buffer);															// 串口打印接收到的数据
						sendto(0,buffer,len, remote_ip, remote_port);		  		// 接收到数据后再回给远程上位机，完成数据回环
					}
			break;
			case SOCK_CLOSED:																						// Socket处于关闭状态
					socket(0,Sn_MR_UDP,local_port,0);												// 打开Socket0，并配置为UDP模式，打开一个本地端口
			break;
		}
	}
}
