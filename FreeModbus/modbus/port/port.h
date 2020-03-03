/******************** (C) COPYRIGHT 2013 **************************
 * 文件名  ：port.h
 * 描述    ：STM32 Modbus-TCP 测试程序         
 * 实验平台：STM32F103VCT6+W5500 开发板
 * 库版本  ：ST3.5.0
 *
 * 作者    ：Zhangsz
 * 编写日期：2013-12-06
**********************************************************************************/

#ifndef _PORT_H
#define _PORT_H

/* ----------------------- Platform includes --------------------------------*/

#include "stm32f10x.h"
#include <stm32f10x_conf.h>

/* ----------------------- Defines ------------------------------------------*/
#define	INLINE
#define PR_BEGIN_EXTERN_C           extern "C" {
#define	PR_END_EXTERN_C             }



#define ENTER_CRITICAL_SECTION( )   EnterCriticalSection( )
#define EXIT_CRITICAL_SECTION( )    ExitCriticalSection( )
#define assert( expr )

//#define SMCLK                       ( 8000000UL )	 /*系统晶振：8Mhz*/
//#define ACLK                        ( 32768UL )	  	 /*RTC晶振： 32768Hz */

//TODO  暂时先写B13引脚，等组网测试时再确认
//#define RS485_SEND_MODE  GPIO_SetBits(GPIOA,GPIO_Pin_8)
//#define RS485_RECEIVE_MODE  GPIO_ResetBits(GPIOA,GPIO_Pin_8)


typedef char   			BOOL;		 //int8_t
typedef unsigned char	UCHAR;	  	 //uint8_t
typedef char    		CHAR;		 //int8_t
typedef unsigned short 	USHORT;		 //uint16_t
typedef short   		SHORT;		 //int16_t
typedef unsigned long 	ULONG;		 //uint32_t
typedef long    		LONG;		 //int32_t

#define TRUE	1
#define FALSE	0

void            EnterCriticalSection( void );
void            ExitCriticalSection( void );


#endif
