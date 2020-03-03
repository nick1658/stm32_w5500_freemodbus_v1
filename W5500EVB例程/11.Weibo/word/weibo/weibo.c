#include "config.h"
#include "socket.h"
#include "w5500.h"

#include <stdio.h>
#include <string.h>

#define SOCK_WEIBO      2                     //the socket number is used for weibo; it can be a number between 0~7
#define WEIBO_SERVER    "61.109.255.136"      //weibo server IP address string
#define HTTP_PATH       "/wiznet/"            //HTTP path
#define weibo_id        "3196855541@qq.com"        								/*ÎÒµÄÐÂÀËÎ¢²©ÕËºÅ*/
#define weibo_pwd       "wildfire123"        											/*ÎÒµÄÐÂÀËÎ¢²©ÃÜÂë*/
//#define MAX_BUF_SIZE    512
char tmp_buf[MAX_BUF_SIZE]={0x00,};                              //a temp buffer to store weibo content and HTTP header
unsigned char weibo_server_ip[4] = {61,109,255,136};    //Weibo server IP address
static unsigned int any_local_port = 1004;              //TCP socket local port nubmer

unsigned char post_weibo_update(char* weibo)
{
  char post_data[385]={0x00,};                            //weibo content (140 characters, but one Chinese character will be 2 Bytes!) + ID & Password information. If your sina weibo ID & Password are too long, please define a bigger buffer
  unsigned char ret=0;
  unsigned int len=0;
  char* p;
  if(socket(SOCK_WEIBO,Sn_MR_TCP,any_local_port++,0)!=1)  //to initialize a TCP socket
  {
    printf("Socket initialization failed.\r\n");
    return 0;
  }
  else
  {
    printf("Connect with Weibo server.\r\n");
    ret=connect(SOCK_WEIBO,weibo_server_ip,80);       //connect to the weibo server, default TCP port is 80
    if(ret!=1)
    {
      printf("Connect Weibo server failed.\r\n");
      return 0;
    }
    else
    {
      while(getSn_SR(SOCK_WEIBO)!=SOCK_ESTABLISHED);  //wait for the TCP connection established!
      printf("Connected with Weibo server.\r\n");
      sprintf(post_data,"id=%s&pw=%s&cmd=update&status=%s",(char*)weibo_id,(char*)weibo_pwd,weibo);
      sprintf(tmp_buf,"POST %s HTTP/1.1\r\nHost: %s\r\nUser-Agent: w5500\r\nContent-Type: application/x-www-form-urlencoded; charset=gb2312\r\nConnection: close\r\nContent-Length: %d\r\n\r\n%s",(char*)HTTP_PATH,(char*)WEIBO_SERVER,strlen(post_data),post_data);
      len=send(SOCK_WEIBO,(unsigned char*)tmp_buf,strlen(tmp_buf)); //upload your weibo content
      while(1)
      {
        len=getSn_RX_RSR(SOCK_WEIBO);
        if(len>0)
        {
          memset(tmp_buf,0x00,MAX_BUF_SIZE);
          len=recv(SOCK_WEIBO, (unsigned char*)tmp_buf, len);       //receive the return result from weibo server
          p=strstr(tmp_buf,(char*)"\r\n\r\n")+4;//get http payload without http header: return value
          printf("rev%s\r\n",p);
          disconnect(SOCK_WEIBO);     //disconnect with weibo server
          close(SOCK_WEIBO);          //close the socket
          return 1;                   //sucess! return 1
        }
      }
    }
  }
}
