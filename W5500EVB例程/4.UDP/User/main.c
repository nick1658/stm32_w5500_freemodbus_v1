#include "usart.h"
#include "device.h"
#include "spi2.h"
#include "ult.h"
#include "socket.h"
#include "w5500.h"
#include "24c16.h"
#include "md5.h"
#include <string.h>

extern uint8 txsize[];										// �����ⲿ����������Socket���ͻ����С
extern uint8 rxsize[];										// �����ⲿ����������Socket���ջ����С

uint8 buffer[2048];												// ����һ��2KB�����飬�������Socket��ͨ������

int main()
{
  uint8 remote_ip[4]={192,168,1,151};			// ����Զ��IP��ַ
	uint16 remote_port=6000;								// ����Զ�̶˿�
  uint16 local_port=5000;									// ��ʼ��һ�����ض˿�
  uint16 len=0;

  /***** MCUʱ�ӳ�ʼ�� *****/							  
	Systick_Init(72);	
	
	/***** GPIO��SPI��ʼ�� *****/
	GPIO_Configuration();			
	WIZ_SPI_Init();
	
	/***** ���ڳ�ʼ�� *****/
	USART1_Init(); 		
	
	/***** ��ʼ��eeprom *****/
  at24c16_init();
	
	/***** Ӳ����W5500 *****/
	Reset_W5500();
	
	/***** W5500��IP��Ϣ��ʼ�� *****/
	set_default(); 														// ����Ĭ��MAC��IP��GW��SUB��DNS
	set_network();														// ���ó�ʼ��IP��Ϣ����ӡ����ʼ��8��Socket

  printf("UDP Local Port: %d \r\n",local_port); 
	printf("UDP Remote IP: %d.%d.%d.%d \r\n",remote_ip[0],remote_ip[1],remote_ip[2],remote_ip[3]);
  printf("UDP Remote Port: %d \r\n",remote_port);
	printf("W5500 Init Complete!\r\n");
  printf("Start UDP Test!\r\n"); 	

	/*Socket״̬����MCUͨ����Sn_SR(0)��ֵ�����ж�SocketӦ�ô��ں���״̬
		Sn_SR״̬������
		0x00		SOCK_CLOSED
		0x13		SOCK_INIT
		0x14		SOCK_LISTEN
		0x17		SOCK_ESTABLISHED
		0x1C		SOCK_CLOSE_WAIT
		0x22		SOCK_UDP
	*/
	while(1)																												// Socket״̬��
	{
		switch(getSn_SR(0))																						// ��ȡsocket0��״̬
		{
			case SOCK_UDP:																							// Socket���ڳ�ʼ�����(��)״̬
					Delay_ms(100);
					if(getSn_IR(0) & Sn_IR_RECV)
					{
						setSn_IR(0, Sn_IR_RECV);															// Sn_IR��RECVλ��1
					}
					// ���ݻػ����Գ������ݴ�Զ����λ������W5500��W5500���յ����ݺ��ٻظ�Զ����λ��
					if((len=getSn_RX_RSR(0))>0)
					{ 
						memset(buffer,0,len+1);
						recvfrom(0,buffer, len, remote_ip,&remote_port);			// W5500��������Զ����λ�������ݣ���ͨ��SPI���͸�MCU
						printf("%s\r\n",buffer);															// ���ڴ�ӡ���յ�������
						sendto(0,buffer,len, remote_ip, remote_port);		  		// ���յ����ݺ��ٻظ�Զ����λ����������ݻػ�
					}
			break;
			case SOCK_CLOSED:																						// Socket���ڹر�״̬
					socket(0,Sn_MR_UDP,local_port,0);												// ��Socket0��������ΪUDPģʽ����һ�����ض˿�
			break;
		}
	}
}
