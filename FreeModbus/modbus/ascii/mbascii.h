/******************** (C) COPYRIGHT 2013 **************************
 * 文件名  ：mbascii.h
 * 描述    ：STM32 Modbus 移植测试程序         
 * 实验平台：STM32F103VCT6+W5500 开发板
 * 库版本  ：ST3.5.0
 *
 * 作者    ：Zhangsz
 * 编写日期：2013-10-03
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
