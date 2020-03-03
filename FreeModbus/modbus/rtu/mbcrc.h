/******************** (C) COPYRIGHT 2013 **************************
 * 文件名  ：mbcrc.c
 * 描述    ：STM32 Modbus 移植测试程序         
 * 实验平台：火牛STM32开发板
 * 库版本  ：ST3.5.0
 *
 * 作者    ：Zhangsz
 * 编写日期：2013-10-03
 *
**********************************************************************************/

#ifndef _MB_CRC_H
#define _MB_CRC_H

USHORT          usMBCRC16( UCHAR * pucFrame, USHORT usLen );

#endif
