/******************** (C) COPYRIGHT 2013 **************************
 * �ļ���  ��mbascii.h
 * ����    ��STM32 Modbus ��ֲ���Գ���         
 * ʵ��ƽ̨��STM32F103VCT6+W5500 ������
 * ��汾  ��ST3.5.0
 *
 * ����    ��Zhangsz
 * ��д���ڣ�2013-10-03
 *
**********************************************************************************/

#ifndef _MB_ASCII_H
#define _MB_ASCII_H

#ifdef __cplusplus
PR_BEGIN_EXTERN_C
#endif

#if MB_ASCII_ENABLED > 0
eMBErrorCode    eMBASCIIInit( UCHAR slaveAddress, UCHAR ucPort,
                              ULONG ulBaudRate, eMBParity eParity );
void            eMBASCIIStart( void );
void            eMBASCIIStop( void );

eMBErrorCode    eMBASCIIReceive( UCHAR * pucRcvAddress, UCHAR ** pucFrame,
                                 USHORT * pusLength );
eMBErrorCode    eMBASCIISend( UCHAR slaveAddress, const UCHAR * pucFrame,
                              USHORT usLength );
BOOL            xMBASCIIReceiveFSM( void );
BOOL            xMBASCIITransmitFSM( void );
BOOL            xMBASCIITimerT1SExpired( void );
#endif

#ifdef __cplusplus
PR_END_EXTERN_C
#endif
#endif
