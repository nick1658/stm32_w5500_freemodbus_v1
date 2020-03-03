/**
******************************************************************************
* @file   			smtp.c
* @author  			WIZnet Software Team 
* @version 			V1.0
* @date    			2015-xx-xx
* @brief   			smtp ��ʾ���� 
******************************************************************************
**/
#include <stdio.h>
#include <string.h>
#include "w5500.h"
#include "smtp.h"
#include "socket.h"

extern CONFIG_MSG ConfigMsg;
uint8 mail_send_ok=0;
char RX_BUF[2048];
char hello[50]="HELO localhost";      			//��ݱ�ʶ����
char hello_reply[]="250 OK";          			//��ݱ�ʶ�ɹ���Ӧ
char AUTH[50]="AUTH LOGIN";           			//��֤����
char AUTH_reply[]="334 dXNlcm5hbWU6";       //��֤�����ͳɹ���Ӧ
char name_126[100]="wiznet2013@126.com";    //126��¼������ 
char base64name_126[200];                   // 126��¼��������base64����
char name_reply[]="334 UGFzc3dvcmQ6";       //���͵�¼���ɹ���Ӧ
char password_126[50]="hello123";           //126 ��½��������
char base64password_126[100];               //base64 126��¼��������
char password_reply[]="235 Authentication successful";      //��½�ɹ���Ӧ
char from[]="wiznet2013@126.com";             //����������
char from_reply[]="250 Mail OK";
char to[]="wiznet_w5500@qq.com";            			//�ռ�������
char to_reply[]="250 Mail OK";
char data_init[10]="data";                		//�������ݴ���
char data_reply[]="354";                  		//����ɹ���Ӧ HEAD
char Cc[]="3196855541@qq.com";         				//����������
char subject[]="Hello!WIZnet!";           		//����
char content[]="Hello!WIZnet!";           		//����
char mime_reply[]="250 Mail OK queued as";		//�ʼ����ͳɹ���Ӧ
char mailfrom[50]="MAIL FROM:<>";
char rcptto[50]="rcpt to:<>";
char mime[200]="From:\r\n";
char mime1[50]="To:\r\n";
char mime2[50]="Cc:\r\n";
char mime3[50]="Subject:\r\n";
char mime4[50]="MIME-Version:1.0\r\nContent-Type:text/plain\r\n\r\n";
char mime5[50]="\r\n.\r\n";

/**
*@brief		�ʼ�������Ϣ����
*@param		��
*@return	��
*/
void mail_message(void)    			
{
	uint16 len_from=strlen(from);
	uint16 len_to=strlen(to);
	uint16 len_Cc=strlen(Cc);
	uint16 len_sub=strlen(subject);
	strcat (hello,"\r\n");
	strcat (AUTH,"\r\n");
	base64encode(name_126,base64name_126);
	base64encode(password_126,base64password_126);
	strcat(base64name_126,"\r\n");
	strcat(base64password_126,"\r\n");
	str_insert(mailfrom,from,11);
	strcat(mailfrom,"\r\n");
	str_insert(rcptto,to,9);
	strcat(rcptto,"\r\n");
	strcat(data_init,"\r\n");
	
	str_insert(mime,from,5);
	str_insert(mime1,to,3);
	str_insert(mime2,Cc,3);
	str_insert(mime3,subject,8);
	str_insert(mime5,content,0);
	strcat(mime,mime1);
	strcat(mime,mime2);
	strcat(mime,mime3);
	strcat(mime,mime4);
	strcat(mime,mime5);
}

uint8 SMTP_STATE=waitfor220;
/**
*@brief		�����ʼ�
*@param		��
*@return	��
*/
void send_mail(void)           
{
	switch(SMTP_STATE)
	{
		case waitfor220:																//�ȴ����ӳɹ�����ȷ��Ӧ״̬
			if(strstr((const char *)RX_BUF,"220")!=NULL)
			{
				send(SOCK_SMTP,(const uint8 *)hello,strlen(hello));//����hello����
				SMTP_STATE=waitforHELO250;   
			}
		break;
		case waitforHELO250:															//�ȴ�hello�������ȷ��Ӧ״̬
			if(strstr((const char *)RX_BUF,hello_reply)!=NULL&&strstr((const char *)RX_BUF,"Mail")==NULL)
			{
				send(SOCK_SMTP,(const uint8 *)AUTH,strlen(AUTH));//����AUTH��֤����
				SMTP_STATE=waitforAUTH334;
			}
		break;
		case waitforAUTH334:																//�ȴ�AUTH��֤�������ȷ��Ӧ
			if(strstr((const char *)RX_BUF,AUTH_reply)!=NULL)
			{
				send(SOCK_SMTP,(const uint8 *)base64name_126,strlen(base64name_126));//���������¼��
				SMTP_STATE=waitforuser334;
			}
		break;
		case waitforuser334:															//�ȴ������¼����½�ɹ�����ȷ��Ӧ
			if(strstr((const char *)RX_BUF,name_reply)!=NULL)
			{ 
				send(SOCK_SMTP,(const uint8 *)base64password_126,strlen(base64password_126));//���������¼����
				SMTP_STATE=waitforpassword235;
			}
		break;
		case waitforpassword235:												//�ȴ����������½�ɹ���ȷ��Ӧ
			if(strstr((const char *)RX_BUF,password_reply)!=NULL)
			{
				send(SOCK_SMTP,(const uint8 *)mailfrom,strlen(mailfrom));//���ͷ�������Ϣ
				SMTP_STATE=waitforsend250;
			}
		break;
		case waitforsend250:																					//�ȴ���������Ϣ���ͳɹ���ȷ��Ӧ
			if(strstr((const char *)RX_BUF,from_reply)!=NULL&&strstr((const char *)RX_BUF,"queued as")==NULL)
			{
				send(SOCK_SMTP,(const uint8 *)rcptto,strlen(rcptto));				//�����ռ�����Ϣ
				SMTP_STATE=waitforrcpt250;
			}
		break;
		case waitforrcpt250:																						//�ȴ��ռ�����Ϣ���ͳɹ���ȷ��Ӧ
			if(strstr((const char *)RX_BUF,to_reply)!=NULL)
			{
				send(SOCK_SMTP,(const uint8 *)data_init,strlen(data_init));//�������ݴ�����������
				SMTP_STATE=waitfordate354;
			}
		break;
		case waitfordate354:																				//�ȴ��������ݴ�����������ͳɹ���ȷ��Ӧ
			if(strstr((const char *)RX_BUF,data_reply)!=NULL)
			{
				send(SOCK_SMTP,(const uint8 *)mime,strlen(mime));  //����mime��������
				SMTP_STATE=waitformime250;
			}
		break;
		case waitformime250:																//�ȴ�mime����ȷ��Ӧ
			if(strstr((const char *)RX_BUF,mime_reply)!=NULL)
			{
				mail_send_ok=1;																			//�ʼ����ͳɹ�
				printf("mail send OK\r\n");    
			}
		break;
		default:
		break;
	}
}

/**
*@brief		 	SMTP ������
*@param			��
*@return		��
*/
void do_smtp(void)                   
{
	uint8 ch=SOCK_SMTP;														//����һ����������ֵSMTPͨ��socket��
	uint16 len;																		//����SMTP Client��ͨ�Ŷ˿ں�
	uint16 anyport=5000;													//SMTP Server �Ķ˿ںţ�Ĭ��Ϊ25
	uint8 Smtp_PORT=25;
	memset(RX_BUF,0,sizeof(RX_BUF));
	switch(getSn_SR(ch)) 												 //��ȡW5500��socket״̬
	{
		case SOCK_INIT:															//��ʼ�����
			connect(ch, ConfigMsg.rip ,Smtp_PORT );	 //����SMTP Server
		break;
		case SOCK_ESTABLISHED: 											//socket�����ɹ�
			if(getSn_IR(ch) & Sn_IR_CON)
			{
				setSn_IR(ch, Sn_IR_CON);								//��������жϱ�־
			}
			if ((len = getSn_RX_RSR(ch)) > 0)		
			{
				while(!mail_send_ok)										//	����ʼ�û�з��ͳɹ�
				{
					memset(RX_BUF,0,sizeof(RX_BUF));			//���ܻ�����ڴ�ռ�����
					len = recv(ch, (uint8*)RX_BUF,len);    //W5500�������ݲ�����RX_BUF
					send_mail();													//�����ʼ�
				}
				disconnect(ch);														//�Ͽ�socket����
			}
		break;
		case SOCK_CLOSE_WAIT:   /*�ȴ�socket�ر�*/
			if ((len = getSn_RX_RSR(ch)) > 0)
			{       
				while(!mail_send_ok)									//	����ʼ�û�з��ͳɹ�
				{
					len = recv(ch, (uint8*)RX_BUF, len);//W5500�������ݲ�����RX_BUF
					send_mail();												//�����ʼ�
				} 
			}
			disconnect(ch);											//�Ͽ�socket����
		break;
		case SOCK_CLOSED:                   
			socket(ch, Sn_MR_TCP,anyport++, 0x00);//��socket
		break;
		default:
		break;
	} 
}

/**
*@brief		 	base64����ת������
*@param			*s: ��Ҫת�����ַ�      *r:���ת������ַ�
*@return		��
*/
void base64encode(char *s, char *r)       
{
	char padstr[4];
	char base64chars[]=	"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
	uint16 i=0,c;
	uint32 n;
	c=strlen(s)%3;
	if(c>0)   
	{
		for(i=0;c<3;c++)
		{
	 		padstr[i++]='=';
		}
	}
	padstr[i]=0;
	i=0;
	for(c=0;c<strlen(s);c+=3)
	{
		n=s[c];
		n<<=8;
		n+=s[c+1];
		if(c+2>strlen(s))
		{
			n&=0xff00;
		}
		n<<=8;
		n+=s[c+2];
		if(c+1>strlen(s))
		{
		  n&=0xffff00;
		}
		r[i++]=base64chars[(n>>18)&63];
		r[i++]=base64chars[(n>>12)&63];
		r[i++]=base64chars[(n>>6)&63];
		r[i++]=base64chars[n&63];
	}
		i-=strlen(padstr);
	for(c=0;c<strlen(padstr);c++)
	{
		r[i++]=padstr[c];
	}
	r[i]=0;
}

/**
*@brief		 	���ַ���s2����s1��posλ��ʼ�ĵط�
*@param			*s1: �ַ���1    *s2:�ַ���2   pos:��ʼ��ַ
*@return		��
*/
void str_insert(char *s1,char *s2,int pos)       
{
	int i;
	int len=strlen(s2);
	for(i=0;i<len;i++)
	{
		*(s1+pos+len+i)=s1[pos+i];
		*(s1+pos+i)=s2[i];
	}
}
