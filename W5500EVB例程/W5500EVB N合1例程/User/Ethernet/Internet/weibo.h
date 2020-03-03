#ifndef _WEIBO_H
#define _WEIBO_H

extern unsigned char weibo_server_ip[4];
void do_weibo(void);
unsigned char post_weibo_update(char* weibo);
#endif
