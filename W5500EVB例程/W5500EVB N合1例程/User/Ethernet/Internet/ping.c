/**
******************************************************************************
* @file   		ping.c
* @author  		WIZnet Software Team 
* @version 		V1.0
* @date    		2015-02-14
* @brief   		ping ��ʾ���� 
******************************************************************************
**/
#include "ping.h"

#define PING_DEBUG
PINGMSGR PingRequest;	 
PINGMSGR PingReply;	   

static uint16 RandomID = 0x1234; 
static uint16 RandomSeqNum = 0x4321;
uint8 ping_reply_received = 0; 
uint8 req=0;
uint8 rep=0;

/**
*@brief		�Զ�PING����IP����
*@param		s��socket number
*@param		addr������IP��ַ
*@return	��
*/
void ping_auto(uint8 s, uint8 *addr)
{
	uint8 i;
	int32_t len = 0;
	uint8 cnt=0;
	for(i = 0; i<=3;i++)																			/*ѭ��ping3��*/
	{
		switch(getSn_SR(0))																			/*��ȡsocket״*/
		{
			case SOCK_CLOSED:																			/*socket�ر�״̬*/
				close(0);
				IINCHIP_WRITE(Sn_PROTO(0), IPPROTO_ICMP);           /*����ICMP Э��*/
				if(socket(0,Sn_MR_IPRAW,3000,0)!=0)									/*�ж�ip rawģʽsocket�Ƿ���*/
				{ 
					// open the SOCKET with IPRAW mode, if fail then Error
				}
				/* Check socket register */
				while(getSn_SR(0)!=SOCK_IPRAW);
				delay_us(1000); 																		/*�ȴ� 1000ms*/
				delay_us(1000); 																		/*�ȴ� 1000ms*/
			break;
			case SOCK_IPRAW:																			/*ip rawģʽ*/
				ping_request(0, addr);															/*����Ping����*/
				req++;
				while(1)
				{	
					if ( (len = getSn_RX_RSR(0) ) > 0)
					{
						ping_reply(0, addr, len);												/*��ȡ�ظ���Ϣ*/
						rep++;
						break;
					}
					else if(cnt > 100)
					{
						printf( "Request Time out. \r\n");
						cnt = 0;
						break;
					}
					else
					{
						cnt++;
						delay_us(50); // wait 50ms
					}
					// wait_time for 2 seconds, Break on fail
				}
	
			break;
			default:
			break;
		}

		if(req>=3)
		{
			#ifdef PING_DEBUG
				printf("Ping Request = %d, PING_Reply = %d\r\n",req,rep);

				if(rep == req)
				if(rep == req)
					printf( "PING SUCCESS\r\n " );
				else
					printf( "REPLY_ERROR\r\n " );	
			#endif  

		}
	}

}

/**
*@brief		�趨����ping����IP����
*@param		s-   socket number
*@param		addr-  ����IP��ַ
*@param		pCount- ping�Ĵ���
*@return	��
*/
void ping_count(uint8 s, uint16 pCount, uint8 *addr)
{
	uint16 rlen, cnt,i;
	cnt = 0;
	
	for(i=0; i<pCount+1;i++)																	/*ѭ��ping pCount��*/
	{

		if(i!=0)
		{
			/* Output count number */
			printf( "\r\nNo.%d\r\n",   (i-1));
		}
		switch(getSn_SR(s))																			/*��ȡsocket״̬*/
		{
			case SOCK_CLOSED:																			/*socket�ر�״̬*/
				close(0);
				close(s);  
				/* Create Socket */  
				IINCHIP_WRITE(Sn_PROTO(s), IPPROTO_ICMP);						/*����ICMP Э��*/
				/* Check socket register */
				while(getSn_SR(s)!=SOCK_IPRAW);		
				delay_us(1000); 																		/*�ȴ� 1000ms*/
				delay_us(1000); 																		/*�ȴ� 1000ms*/
			break;
			case SOCK_IPRAW:																			/*ip rawģʽ*/
				ping_request(s, addr);															/*����Ping����*/
				req++;			
				while(1)
				{
					if ( (rlen = getSn_RX_RSR(s) ) > 0)
					{
						ping_reply(s, addr, rlen);											/*��ȡ�ظ���Ϣ*/
						rep++;
						if (ping_reply_received)  break;				
					}		
					if ( (cnt > 100) ) 
					{
						printf( "\r\nRequest Time out. \r\n") ;
						cnt = 0;
						break;
					}
					else 
					{ 
						cnt++; 	
						delay_us(50); // wait 50ms
					}
				}
			break;
			default:		
			break;
		}
		if(req>=pCount)
		{	
			#ifdef PING_DEBUG
			printf("Ping Request = %d, PING_Reply = %d\r\n",req,rep);
			if(rep == req)
			printf( "PING SUCCESS\r\n " );
			else
			printf( "REPLY_ERROR\r\n " );	
			#endif  
		}
	}
}

/**
*@brief		ping��Ӧ����
*@param		s-  socket number
*@param		addr-  P��ַ
*@return	��
*/
uint8 ping_request(uint8 s, uint8 *addr)
{
	uint16 i;
	ping_reply_received = 0;	        												/*ping �ظ���ʼ����־λ*/
	PingRequest.Type = PING_REQUEST;                  				/*Ping-Request*/
	PingRequest.Code = CODE_ZERO;	                   					/*���� '0'*/
	PingRequest.ID = htons(RandomID++);	       								/*����ping��ӦIDΪ��������ͱ���*/
	PingRequest.SeqNum =htons(RandomSeqNum++);								/*����ping��Ӧ�����к�Ϊ������α���*/
	for(i = 0 ; i < BUF_LEN; i++)
	{	                                
		PingRequest.Data[i] = (i) % 8;		 				 							/*ping��Ӧ������'0'~'8��*/
	}
	PingRequest.CheckSum = 0;
	/* ������Ӧ����*/
	PingRequest.CheckSum = htons(checksum((uint8*)&PingRequest,sizeof(PingRequest))); 
	/*����ping��Ӧ��Ŀ�ķ� */
	if(sendto(s,(uint8 *)&PingRequest,sizeof(PingRequest),addr,3000)==0)	
	{  
		printf( "\r\n Fail to send ping-reply packet  r\n") ;					
	}
	else
	{
		printf( "Send Ping Request  to Destination (") ;					
		printf( "%d.%d.%d.%d )",   (addr[0]),  (addr[1]),  (addr[2]),  (addr[3])) ;
		printf( " ID:%x  SeqNum:%x CheckSum:%x\r\n",   htons(PingRequest.ID),  htons(PingRequest.SeqNum),  htons(PingRequest.CheckSum)) ;
	}
	return 0;
} 

/**
*@brief		����Ping�ظ�
*@param		s-  socket number
*@param		addr- Ping��ַ
*@return	��
*/
uint8 ping_reply(uint8 s, uint8 *addr,  uint16 rlen)
{
	uint16 tmp_checksum;
	uint16 len;
	uint16 i;
	uint8 data_buf[128];
	
	uint16 port = 3000;
	PINGMSGR PingReply;
	len = recvfrom(s, (uint8 *)data_buf,rlen,addr,&port);			/*��Ŀ�Ķ˽�������*/
	if(data_buf[0] == PING_REPLY) 
	{
		PingReply.Type 		 = data_buf[0];
		PingReply.Code 		 = data_buf[1];
		PingReply.CheckSum   = (data_buf[3]<<8) + data_buf[2];
		PingReply.ID 		 = (data_buf[5]<<8) + data_buf[4];
		PingReply.SeqNum 	 = (data_buf[7]<<8) + data_buf[6];
		
		for(i=0; i<len-8 ; i++)
		{
			PingReply.Data[i] = data_buf[8+i];
		}
		tmp_checksum = ~checksum(data_buf,len);									/*���ping�ظ��Ĵ���*/
		if(tmp_checksum != 0xffff)
			printf("tmp_checksum = %x\r\n",tmp_checksum);
		else
		{
			printf("Reply from %d.%d.%d.%d  ID:%x SeqNum:%x  :data size %d bytes\r\n",
			(addr[0]),  (addr[1]),  (addr[2]),  (addr[3]),  htons(PingReply.ID),  htons(PingReply.SeqNum),  (rlen+6) );
			printf("\r\n");
			ping_reply_received =1;																/*���˳�ping�ظ�ѭ��ʱ������ping�ظ���־Ϊ1*/
		}
	}
	else if(data_buf[0] == PING_REQUEST)
	{
		PingReply.Code 	 = data_buf[1];
		PingReply.Type 	 = data_buf[2];
		PingReply.CheckSum  = (data_buf[3]<<8) + data_buf[2];
		PingReply.ID 		 = (data_buf[5]<<8) + data_buf[4];
		PingReply.SeqNum 	 = (data_buf[7]<<8) + data_buf[6];		
		for(i=0; i<len-8 ; i++)
		{
			PingReply.Data[i] = data_buf[8+i];
		}
		tmp_checksum = PingReply.CheckSum;											/*���ping�ظ�����*/
		PingReply.CheckSum = 0;
		if(tmp_checksum != PingReply.CheckSum)
		{
			printf( " \n CheckSum is in correct %x shold be %x \n",   (tmp_checksum),  htons(PingReply.CheckSum)) ;
		}
		else
		{		}
		printf("Request from %d.%d.%d.%d  ID:%x SeqNum:%x  :data size %d bytes\r\n",
		(addr[0]),  (addr[1]),  (addr[2]),  (addr[3]),  (PingReply.ID),  (PingReply.SeqNum),  (rlen+6) );	   
		ping_reply_received =1;																	/* ���˳�ping�ظ�ѭ��ʱ������ping�ظ���־Ϊ1	*/
	}
	else
	{      
		printf(" Unkonwn msg. \n");
	}
	return 0;
}

/**
*@brief		ִ��ping������
*@param		��
*@return	��
*/
void do_ping(void)
{	 
	printf("------------PING_TEST_START-----------------------\r\n");
	delay_ms(3000); // wait 1000ms
	ping_auto(SOCK_PING,remote_ip);
	//	 while(1);
}




