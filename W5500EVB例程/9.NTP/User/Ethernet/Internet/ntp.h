#ifndef	__NTP_H__
#define	__NTP_H__
#include "types.h"
/* for ntpclient */ 

extern uint8 ntp_server_ip[4];
extern uint8 ntp_port;
typedef signed char s_char;
typedef unsigned long long tstamp;
typedef unsigned int tdist;

#pragma pack(1)
typedef struct _NPTformat
{
   
        uint8  dstaddr[4];        /* destination (local) address */
        char    version;        /* version number */
        char    leap;           /* leap indicator */
        char    mode;           /* mode */
        char    stratum;        /* stratum */
        char    poll;           /* poll interval */
        s_char  precision;      /* precision */
        tdist   rootdelay;      /* root delay */
        tdist   rootdisp;       /* root dispersion */
        char    refid;          /* reference ID */
        tstamp  reftime;        /* reference time */
        tstamp  org;            /* origin timestamp */
        tstamp  rec;            /* receive timestamp */
        tstamp  xmt;            /* transmit timestamp */
      

} NPTformat;
#pragma pack()

#pragma pack(1)
typedef struct _DateTime	/*�˽ṹ�嶨����NTPʱ��ͬ������ر���*/
{
  uint16 year[2];					/*��Ϊ2�ֽ� ��Ϊ�ߵ���λ */
  uint8 month;						/*	�� */
  uint8 day;							/*	�� */
  uint8 hour;							/*	ʱ */
  uint8 minute;						/*	�� */
  uint8 second;						/*	�� */
} DateTime;
#pragma pack()


#define SECS_PERDAY     	86400UL             	// seconds in a day = 60*60*24
#define UTC_ADJ_HRS         	9              	        // SEOUL : GMT+9 
#define EPOCH                   1900                    // NTP start year
void get_seconds_from_ntp_server(uint8* buf,uint16 idx);
void ntp_client_init(void);
void do_ntp_client(void);
tstamp change_datetime_to_seconds(void);
void calc_date_time(tstamp seconds);
#endif

