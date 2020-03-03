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

uint8 Buffer[1024];													// ����һ��1KB�����飬�������Socket��ͨ������

int main()
{
	uint8 len,ret;	
	uint8 g_socket_rcv_buf[MAX_MTU_SIZE];
	uint8 filename_flag=0;
	uint32 addr;
	uint32 tftp_server;	
  uint32 app_first_4_bytes;
	
	/***** MCUʱ�ӳ�ʼ�� *****/							  
	Systick_Init(72);	
	
	/***** �жϿ��������� *****/
	NVIC_Configuration();
	
	/***** GPIO��SPI��ʼ�� *****/
	GPIO_Configuration();			
	WIZ_SPI_Init();
	
	/***** ���ڳ�ʼ�� *****/
	USART1_Init(); 		
	
	/***** Ӳ����W5500 *****/
	Reset_W5500();
	
	/***** W5500��IP��Ϣ��ʼ�� *****/
	set_default(); 																						// ����Ĭ��IP��Ϣ
	set_network();																						// ���ó�ʼ��IP��Ϣ����ӡ����ʼ��8��Socket
	set_w5500_mac();																					// ����MAC��ַ

	printf("Remote IP:%d.%d.%d.%d\r\n", remote_ip[0],remote_ip[1],remote_ip[2],remote_ip[3]);
	printf("W5500 Init Complete!\r\n");
  printf("Start TFTP Client Test!\r\n\r\n"); 

  addr = ApplicationAddress;
  app_first_4_bytes = *(vu32*)addr;
	
  if ((app_first_4_bytes & 0xffffffff) == 0xffffffff)				// ��app��ǰ���ֽ�Ϊ0x08008000��˵��Flash����δ��¼app����
  { 
		write_config_to_eeprom(); 															// ��Flash����¼app
  }	
	get_config();																							// ��Flash�ж�ȡ������Ϣ
	TFTP_init(SOCK_TFTP,g_socket_rcv_buf);										// TFTP��ʼ��������һ·Socket����TFTPͨ��
	
	while(1)
  {
		if((ConfigMsg.newfirm[0]!=ConfigMsg.sw_ver[0])||(ConfigMsg.newfirm[1]!=ConfigMsg.sw_ver[1]))
		{			
			if(USART_RX_STA & 0x8000)  														// ��������
			{
				len = USART_RX_STA & 0x3fff;												// ���㴮���������ݵĳ���
				printf("Send new version app:%s\r\n",USART_RX_BUF);	// ��ӡ���������				
				memcpy(ConfigMsg.filename,USART_RX_BUF,len);				// ����������ݼ�Filename��Ҫ��/д��λ�ڷ������ϵ��ļ�������ͬ
				USART_RX_STA=0;																			// ����0��β
				memset(USART_RX_BUF,0,len+1);												// USART_RX_BUF��0
				filename_flag=1;																		// Filename��־λ��1
			}		
			if(filename_flag==1)																	// ��Filename��־λΪ1
			{
				filename_flag=0;																		// Filename��־λ��0
				printf("TFTP Start!\r\n");
				tftp_server = (remote_ip[0] << 24) | (remote_ip[1] << 16) | (remote_ip[2] << 8) | (remote_ip[3]);		// ����TFTP������IP��ַ
				TFTP_read_request(tftp_server,ConfigMsg.filename);	// TFTP������
				
				while(1)
				{	
					ret = TFTP_run();																	// TFTP����
					if(ret != TFTP_PROGRESS)													// TFTP���̽�������ת��TFTP�ɹ�
					break;
				}
				if(ret == TFTP_SUCCESS)															// TFTP�ɹ�
				{
					printf("TFTP Success!\r\n");								
					Delay_ms(10);
					reboot_app();																			// �����������°汾��app
				}			
			}
		}
		else																										// ���ް汾���£������ɰ汾��app
		{
			printf("No new version, run the old version.\r\n");
			reboot_app();																					
		}
	}										
}

