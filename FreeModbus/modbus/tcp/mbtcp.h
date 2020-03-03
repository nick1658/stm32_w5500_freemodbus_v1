/******************** (C) COPYRIGHT 2013 **************************
 * �ļ���  ��mbtcp.h
 * ����    ��STM32 Modbus ��ֲ���Գ���         
 * ʵ��ƽ̨����ţSTM32������
 * ��汾  ��ST3.5.0
 *
 * ����    ��Zhangsz
 * ��д���ڣ�2013-10-03
 *
**********************************************************************************/

#ifndef _MB_TCP_H
#define _MB_TCP_H

#ifdef __cplusplus
PR_BEGIN_EXTERN_C
#endif

/* ----------------------- Defines ------------------------------------------*/
#define MB_TCP_PSEUDO_ADDRESS   255

/* ----------------------- Function prototypes ------------------------------*/
    eMBErrorCode eMBTCPDoInit( USHORT ucTCPPort );
void            eMBTCPStart( void );
void            eMBTCPStop( void );
eMBErrorCode    eMBTCPReceive( UCHAR * pucRcvAddress, UCHAR ** pucFrame,
                               USHORT * pusLength );
eMBErrorCode    eMBTCPSend( UCHAR _unused, const UCHAR * pucFrame,
                            USHORT usLength );

#ifdef __cplusplus
PR_END_EXTERN_C
#endif
#endif
