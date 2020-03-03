/******************** (C) COPYRIGHT 2014 91mcu **************************
 * 文件名  ：main.c
 * 描述    ：W5500 以太网模块测试程序：Modbus-TCP 功能实现。       
 * 实验平台：STM32F103VCT6+W5500
 * 库版本  ：ST3.5.0
 *
 * 作者    ：zhangsz
 * 论坛    ：http://www.91mcu.com
 * 创建日期：2014-03-10
 * 修改日期：2014-04-04
**********************************************************************************/	
#include <string.h>
#include <stdio.h>
#include "stm32f10x.h"

#include "W5500.h"
#include "device.h" 
#include "systick.h"
#include "led.h"
#include "mb.h"


/* ----------------------- Holding register Defines ------------------------------------------*/

#define REG_HOLDING_START 1000
#define REG_HOLDING_NREGS 4

/* ----------------------- Static variables ---------------------------------*/
static unsigned short usRegHoldingStart = REG_HOLDING_START;
static unsigned short usRegHoldingBuf[REG_HOLDING_NREGS]={12,23,1234,4567};

/* ----------------------- input register Defines ------------------------------------------*/
#define REG_INPUT_START 1000
#define REG_INPUT_NREGS 4

/* ----------------------- Static variables ---------------------------------*/
static unsigned short usRegInputStart = REG_INPUT_START;
static unsigned short usRegInputBuf[REG_INPUT_NREGS]={0,0,0x5599,0x5588};

/* ----------------------- coils register Defines ------------------------------------------*/
#define REG_COILS_START     1000
#define REG_COILS_SIZE      16

/* ----------------------- Static variables ---------------------------------*/
static unsigned char ucRegCoilsBuf[REG_COILS_SIZE / 8]={0xaa,0xfe};	  //数量低于8个还需要验证一下，是否需要加1呢。

/* ----------------------- discrete register Defines ------------------------------------------*/
#define REG_DISC_START     1000
#define REG_DISC_SIZE      16

/* ----------------------- Static variables ---------------------------------*/
static unsigned char ucRegDiscBuf[REG_DISC_SIZE / 8] = { 0x98, 0x6e };	   //数量8的整数倍。


void BSP_LED(void);	  //LED指示线圈操作
 
/* W5500 configuration */
void W5500_Configuration()
{
	unsigned char array[6];

	GPIO_SetBits(GPIO_W5500_RST_PORT, GPIO_W5500_RST_Pin);
	Delay_ms(100);    /*delay 100ms 使用systick 1ms时基的延时*/
	while((Read_1_Byte(PHYCFGR)&LINK)==0); 		/* Waiting for Ethernet Link */

	Write_1_Byte(MR, RST);
	Delay_ms(20);		/*delay 20ms */

	/* Set Gateway IP as: 192.168.1.1 */
	array[0]=192;
	array[1]=168;
	array[2]=2;
	array[3]=1;
	Write_Bytes(GAR, array, 4);

	/* Set Subnet Mask as: 255.255.255.0 */
	array[0]=255;
	array[1]=255;
	array[2]=255;
	array[3]=0;
	Write_Bytes(SUBR, array, 4);

	/* Set MAC Address as: 0x48,0x53,0x00,0x57,0x55,0x00 */
	array[0]=0x48;
	array[1]=0x53;
	array[2]=0x00;
	array[3]=0x57;
	array[4]=0x55;
	array[5]=0x00;
	Write_Bytes(SHAR, array, 6);

	/* Set W5500 IP as: 192.168.1.128 */
	array[0]=192;
	array[1]=168;
	array[2]=2;
	array[3]=250;
	Write_Bytes(SIPR, array, 4);
}




/*****************************************************************
                           Main Program
*****************************************************************/
int main(void)
{
	unsigned char i;
	uint16_t port = 502;
	uint8_t sn = 0;

	/* Initialize STM32F103 */
	System_Initialization();
	SysTick_Init();

	/* Config W5500 */
	W5500_Configuration();
	Delay_ms(200);

	/* Modbus-TCP Init */
    eMBTCPInit(MB_TCP_PORT_USE_DEFAULT); 
	Delay_ms(200); 
	
	/* Enable Modbus-TCP Stack */    
    eMBEnable();	
    

    printf("\r\nModbus-TCP Start!\r\n");
    printf("IP:192.168.2.250\r\n");


	while(1)
	{
		
		sn = 0;
		i=Read_SOCK_1_Byte(sn,Sn_SR);  //读W5500状态
		switch(i)
		{
			case SOCK_ESTABLISHED :
				if(Read_SOCK_1_Byte(sn, Sn_IR) & IR_CON)
				{
					printf("%d:Connected\r\n",sn);
					Write_SOCK_1_Byte(sn,Sn_IR,IR_CON);
				}
				eMBPoll();
				break;
			case SOCK_CLOSE_WAIT :
				printf("%d:CloseWait\r\n",sn);
				do
				{
					Delay_ms(100);
				
				}while(Socket_Listen(sn)==FALSE);
				break;
			case SOCK_INIT :
				printf("%d:Listen, port [%d]\r\n",sn, port);
				do
				{
					Delay_ms(100);
				
				}while(Socket_Listen(sn)==FALSE);
				break;
			case SOCK_CLOSED:
				printf("%d:LBTStart\r\n",sn);
				do
				{
					Delay_ms(100);
				
				}while(Socket_Listen(sn)==FALSE);
				break;
			default:break;
		}
		
		sn = 1;
		i=Read_SOCK_1_Byte(sn,Sn_SR);  //读W5500状态
		
		switch(i)
		{
			case SOCK_ESTABLISHED :
				if(Read_SOCK_1_Byte(sn, Sn_IR) & IR_CON)
				{
					printf("%d:Connected\r\n",sn);
					Write_SOCK_1_Byte(sn,Sn_IR,IR_CON);
				}
				eMBPoll();
				break;
			case SOCK_CLOSE_WAIT :
				printf("%d:CloseWait\r\n",sn);
				do
				{
					Delay_ms(100);
				
				}while(Socket_Listen(sn)==FALSE);
				break;
			case SOCK_INIT :
				printf("%d:Listen, port [%d]\r\n",sn, port);
				do
				{
					Delay_ms(100);
				
				}while(Socket_Listen(sn)==FALSE);
				break;
			case SOCK_CLOSED:
				printf("%d:LBTStart\r\n",sn);
				do
				{
					Delay_ms(100);
				
				}while(Socket_Listen(sn)==FALSE);
				break;
			default:break;
		}
	}
}



eMBErrorCode
eMBRegHoldingCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNRegs,
                 eMBRegisterMode eMode )
{
    eMBErrorCode    eStatus = MB_ENOERR;
    int             iRegIndex;

    if( ( usAddress >= REG_HOLDING_START ) &&
        ( usAddress + usNRegs <= REG_HOLDING_START + REG_HOLDING_NREGS ) )
    {
        iRegIndex = ( int )( usAddress - usRegHoldingStart );
        switch ( eMode )
        {
                /* Pass current register values to the protocol stack. */
            case MB_REG_READ:
                while( usNRegs > 0 )
                {
                    *pucRegBuffer++ =
                        ( unsigned char )( usRegHoldingBuf[iRegIndex] >> 8 );
                    *pucRegBuffer++ =
                        ( unsigned char )( usRegHoldingBuf[iRegIndex] &
                                           0xFF );
                    iRegIndex++;
                    usNRegs--;
                }
                break;

                /* Update current register values with new values from the
                 * protocol stack. */
            case MB_REG_WRITE:
                while( usNRegs > 0 )
                {
                    usRegHoldingBuf[iRegIndex] = *pucRegBuffer++ << 8;
                    usRegHoldingBuf[iRegIndex] |= *pucRegBuffer++;
                    iRegIndex++;
                    usNRegs--;
                }
        }
    }
    else
    {
        eStatus = MB_ENOREG;
    }
    return eStatus;
}

/**
  * @功能
  * @参数
  * @返回值
  */
eMBErrorCode
eMBRegInputCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNRegs )
{
    eMBErrorCode    eStatus = MB_ENOERR;
    int             iRegIndex;

    if( ( usAddress >= REG_INPUT_START )
        && ( usAddress + usNRegs <= REG_INPUT_START + REG_INPUT_NREGS ) )
    {
        iRegIndex = ( int )( usAddress - usRegInputStart );
        while( usNRegs > 0 )
        {
            *pucRegBuffer++ =
                ( unsigned char )( usRegInputBuf[iRegIndex] >> 8 );
            *pucRegBuffer++ =
                ( unsigned char )( usRegInputBuf[iRegIndex] & 0xFF );
            iRegIndex++;
            usNRegs--;
        }
    }
    else
    {
        eStatus = MB_ENOREG;
    }

    return eStatus;
}


/**
  * @功能
  * @参数
  * @返回值
  */


eMBErrorCode
eMBRegCoilsCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNCoils,
               eMBRegisterMode eMode )
{
    eMBErrorCode    eStatus = MB_ENOERR;
    int             iNCoils = ( int )usNCoils;
    unsigned short  usBitOffset;

    /* Check if we have registers mapped at this block. */
    if( ( usAddress >= REG_COILS_START ) &&
        ( usAddress + usNCoils <= REG_COILS_START + REG_COILS_SIZE ) )
    {
        usBitOffset = ( unsigned short )( usAddress - REG_COILS_START );
        switch ( eMode )
        {
                /* Read current values and pass to protocol stack. */
            case MB_REG_READ:
                while( iNCoils > 0 )
                {
                    *pucRegBuffer++ =
                        xMBUtilGetBits( ucRegCoilsBuf, usBitOffset,
                                        ( unsigned char )( iNCoils >
                                                           8 ? 8 :
                                                           iNCoils ) );
                    iNCoils -= 8;
                    usBitOffset += 8;
                }
                break;

                /* Update current register values. */
            case MB_REG_WRITE:
                while( iNCoils > 0 )
                {
                    xMBUtilSetBits( ucRegCoilsBuf, usBitOffset, 
                                    ( unsigned char )( iNCoils > 8 ? 8 : iNCoils ),
                                    *pucRegBuffer++ );
                    iNCoils -= 8;
                    usBitOffset += 8;
                }
                break;
        }

    }
    else
    {
        eStatus = MB_ENOREG;
    }
    return eStatus;
}

eMBErrorCode
eMBRegDiscreteCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNDiscrete )
{
    eMBErrorCode    eStatus = MB_ENOERR;
    short           iNDiscrete = ( short )usNDiscrete;
    unsigned short  usBitOffset;

    /* Check if we have registers mapped at this block. */
    if( ( usAddress >= REG_DISC_START ) &&
        ( usAddress + usNDiscrete <= REG_DISC_START + REG_DISC_SIZE ) )
    {
        usBitOffset = ( unsigned short )( usAddress - REG_DISC_START );
        while( iNDiscrete > 0 )
        {
            *pucRegBuffer++ =
                xMBUtilGetBits( ucRegDiscBuf, usBitOffset,
                                ( unsigned char )( iNDiscrete >
                                                   8 ? 8 : iNDiscrete ) );
            iNDiscrete -= 8;
            usBitOffset += 8;
        }
    }
    else
    {
        eStatus = MB_ENOREG;
    }
    return eStatus;
}

/**
  * @}
  */


// 线圈操作用于LED控制
void BSP_LED(void)
{
    uint8_t led_state = ucRegCoilsBuf[0];
    
    if(led_state & 0x01)
		LED1(ON);
	else
		LED1(OFF);

	if(led_state & 0x02)
		LED2(ON);
	else
		LED2(OFF);

	if(led_state & 0x04)
		LED3(ON);
	else
		LED3(OFF);

	if(led_state & 0x08)
		LED4(ON);
	else
		LED4(OFF);
    
}



/******************* (C) COPYRIGHT 2014 91mcu *****END OF FILE************/
