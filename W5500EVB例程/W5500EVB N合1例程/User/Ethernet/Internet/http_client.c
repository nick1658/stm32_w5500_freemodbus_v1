/**
******************************************************************************
* @file   		http_client.c
* @author  		WIZnet Software Team 
* @version 		V1.0
* @date    		2015-xx-xx
* @brief   		http_client ��ʾ���� 
******************************************************************************/
#include <stdio.h>
#include <string.h>
#include "http_client.h"
#include "W5500_conf.h"
#include "bsp_dht11.h"
#include "w5500.h"
#include "socket.h"
#include "utility.h"

uint8 temp_rh[2];									//p[0]��ʪ�� p[1]���¶�
DHT11_Data_TypeDef dht11_data;     /*temp_rh[0]��ʪ�� temp_rh[1]���¶�*/
char  postT[]={										//�ύ�¶�
"POST /v1.0/device/17530/sensor/30435/datapoints HTTP/1.1\r\n"
"Host:api.yeelink.net\r\n"
"Accept:*/*\r\n"
"U-ApiKey:07597cf93ee715d2fee144f25aa3ff65\r\n"
"Content-Length:12\r\n"
"Content-Type:application/x-www-form-urlencoded\r\n"
"Connection:close\r\n"
"\r\n"
"{\"value\":xx}\r\n"
};//xx���������ֵ

char  postH[]={					//�ύʪ��
"POST /v1.0/device/17530/sensor/30436/datapoints HTTP/1.1\r\n"
"Host:api.yeelink.net\r\n"
"Accept:*/*\r\n"
"U-ApiKey:07597cf93ee715d2fee144f25aa3ff65\r\n"
"Content-Length:12\r\n"
"Content-Type:application/x-www-form-urlencoded\r\n"
"Connection:close\r\n"
"\r\n"
"{\"value\":xx}\r\n"
};//xx����
char *post[]={postH,postT};

uint8 yeelink_server_ip[4]={42,96,164,52};				//api.yeelink.net  ��ip��ַ
uint8 yeelink_port=80;														//api.yeelink.net  �Ķ˿ں�	

/**
*@brief		ִ��http client������
*@param		��
*@return	��
*/
void do_http_client(void)
{	
	static uint8 i=0;
	uint8 ch=SOCK_HTTPC;
	uint8 buffer[1024]={0};
	uint16 anyport=3000;
	switch(getSn_SR(ch))																							/*��ȡsocket SOCK_TCPS ��״̬*/
	{
		case SOCK_INIT:																									/*socket��ʼ�����*/
			connect(ch, yeelink_server_ip ,yeelink_port);									/*��socket�˿�*/
		break;	
		case SOCK_ESTABLISHED: 																					/*socket���ӽ���*/
			if(getSn_IR(ch) & Sn_IR_CON)  
			{
				setSn_IR(ch, Sn_IR_CON);																		/*��������жϱ�־*/
			}

			if( Read_DHT11(&dht11_data)==SUCCESS)													/*�ж��Ƿ�õ�DHT11����*/

			{
				temp_rh[0]=dht11_data.humi_int;															/*8bit ʪ������	*/
				temp_rh[1]=dht11_data.temp_int;															/*8bit �¶�����*/
				memcpy(buffer,post[i],strlen(post[i]));	
				buffer[231]=temp_rh[i]/10+0x30;		  
				buffer[232]=temp_rh[i]%10+0x30;
				send(ch,(const uint8 *)buffer,sizeof(buffer));							/*W5500�� Yeelink��������������*/
				i=1;
				printf("RH:%d, Temp:%d\r\n", temp_rh[0], temp_rh[1]);
			}

			delay_ms(500);
			
		break;
		case SOCK_CLOSE_WAIT:  																					/*socket�ȴ��ر�״̬*/
			close(ch);  
		break;
		case SOCK_CLOSED: 																							/*socket�ر�*/
			if(i==1)printf("Send to Yeelink:OK\r\n");
       i=0;		
			socket(ch, Sn_MR_TCP,anyport++ , 0x00);   
		break;
		default:
		break;
	}  
}


