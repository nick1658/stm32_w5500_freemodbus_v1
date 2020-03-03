/******************** (C) COPYRIGHT 2014 91mcu **************************
 * 文件名  ：w5500.c
 * 描述    ：W5500底层驱动函数。       
 * 实验平台：STM32F103VCT6
 * 库版本  ：ST3.5.0
 *
 * 作者    ：zhangsz
 * 论坛    ：http://www.91mcu.com
 * 创建日期：
 * 修改日期：2014-04-04
**********************************************************************************/
#include "stm32f10x.h"            /* STM32F10x库定义 */
#include "port.h"
#include "W5500.h"
#include "device.h"
#include "systick.h"

typedef  unsigned char SOCKET;
#define S_RX_SIZE	2048
#define S_TX_SIZE	2048


/******************************* W5500 Write Operation *******************************/
/* Write W5500 Common Register a byte */
void Write_1_Byte(unsigned short reg, unsigned char dat)
{
	/* Set W5500 SCS Low */
	W5500_SCS_LOW;
	
	/* Write Address */
	SPI_I2S_SendData(SPI_X,reg/256);
	while(SPI_I2S_GetFlagStatus(SPI_X, SPI_I2S_FLAG_TXE) == RESET);
	SPI_I2S_SendData(SPI_X,reg);
	while(SPI_I2S_GetFlagStatus(SPI_X, SPI_I2S_FLAG_TXE) == RESET);

	/* Write Control Byte */
	SPI_I2S_SendData(SPI_X,(FDM1|RWB_WRITE|COMMON_R));
	while(SPI_I2S_GetFlagStatus(SPI_X, SPI_I2S_FLAG_TXE) == RESET);

	/* Write 1 byte */
	SPI_I2S_SendData(SPI_X,dat);
	while(SPI_I2S_GetFlagStatus(SPI_X, SPI_I2S_FLAG_TXE) == RESET);

	/* Set W5500 SCS High */
	W5500_SCS_HIGH;
}

/* Write W5500 Common Register 2 bytes */
void Write_2_Byte(unsigned short reg, unsigned short dat)
{
	/* Set W5500 SCS Low */
	W5500_SCS_LOW;
	
	/* Write Address */
	SPI_I2S_SendData(SPI_X,reg/256);
	while(SPI_I2S_GetFlagStatus(SPI_X, SPI_I2S_FLAG_TXE) == RESET);
	SPI_I2S_SendData(SPI_X,reg);
	while(SPI_I2S_GetFlagStatus(SPI_X, SPI_I2S_FLAG_TXE) == RESET);

	/* Write Control Byte */
	SPI_I2S_SendData(SPI_X,(FDM2|RWB_WRITE|COMMON_R));
	while(SPI_I2S_GetFlagStatus(SPI_X, SPI_I2S_FLAG_TXE) == RESET);

	/* Write 2 bytes */
	SPI_I2S_SendData(SPI_X,dat/256);
	while(SPI_I2S_GetFlagStatus(SPI_X, SPI_I2S_FLAG_TXE) == RESET);
	SPI_I2S_SendData(SPI_X,dat);
	while(SPI_I2S_GetFlagStatus(SPI_X, SPI_I2S_FLAG_TXE) == RESET);

	/* Set W5500 SCS High */
	W5500_SCS_HIGH;
}

/* Write W5500 Common Register n bytes */
void Write_Bytes(unsigned short reg, unsigned char *dat_ptr, unsigned short size)
{
	unsigned short i;

	/* Set W5500 SCS Low */
	W5500_SCS_LOW;
	
	/* Write Address */
	SPI_I2S_SendData(SPI_X,reg/256);
	while(SPI_I2S_GetFlagStatus(SPI_X, SPI_I2S_FLAG_TXE) == RESET);
	SPI_I2S_SendData(SPI_X,reg);
	while(SPI_I2S_GetFlagStatus(SPI_X, SPI_I2S_FLAG_TXE) == RESET);

	/* Write Control Byte */
	SPI_I2S_SendData(SPI_X,(VDM|RWB_WRITE|COMMON_R));
	while(SPI_I2S_GetFlagStatus(SPI_X, SPI_I2S_FLAG_TXE) == RESET);

	/* Write n bytes */
	for(i=0;i<size;i++)
	{
		SPI_I2S_SendData(SPI_X,*dat_ptr);
		while(SPI_I2S_GetFlagStatus(SPI_X, SPI_I2S_FLAG_TXE) == RESET);

		dat_ptr++;
	}

	/* Set W5500 SCS High */
	W5500_SCS_HIGH;
}

/* Write W5500 Socket Register 1 byte */
void Write_SOCK_1_Byte(SOCKET s, unsigned short reg, unsigned char dat)
{
	/* Set W5500 SCS Low */
	W5500_SCS_LOW;
	
	/* Write Address */
	SPI_I2S_SendData(SPI_X,reg/256);
	while(SPI_I2S_GetFlagStatus(SPI_X, SPI_I2S_FLAG_TXE) == RESET);
	SPI_I2S_SendData(SPI_X,reg);
	while(SPI_I2S_GetFlagStatus(SPI_X, SPI_I2S_FLAG_TXE) == RESET);

	/* Write Control Byte */
	SPI_I2S_SendData(SPI_X,(FDM1|RWB_WRITE|(s*0x20+0x08)));
	while(SPI_I2S_GetFlagStatus(SPI_X, SPI_I2S_FLAG_TXE) == RESET);

	/* Write 1 byte */
	SPI_I2S_SendData(SPI_X,dat);
	while(SPI_I2S_GetFlagStatus(SPI_X, SPI_I2S_FLAG_TXE) == RESET);

	/* Set W5500 SCS High */
	W5500_SCS_HIGH;
}

/* Write W5500 Socket Register 2 byte */
void Write_SOCK_2_Byte(SOCKET s, unsigned short reg, unsigned short dat)
{
	/* Set W5500 SCS Low */
	W5500_SCS_LOW;
	
	/* Write Address */
	SPI_I2S_SendData(SPI_X,reg/256);
	while(SPI_I2S_GetFlagStatus(SPI_X, SPI_I2S_FLAG_TXE) == RESET);
	SPI_I2S_SendData(SPI_X,reg);
	while(SPI_I2S_GetFlagStatus(SPI_X, SPI_I2S_FLAG_TXE) == RESET);

	/* Write Control Byte */
	SPI_I2S_SendData(SPI_X,(FDM2|RWB_WRITE|(s*0x20+0x08)));
	while(SPI_I2S_GetFlagStatus(SPI_X, SPI_I2S_FLAG_TXE) == RESET);

	/* Write 2 bytes */
	SPI_I2S_SendData(SPI_X,dat/256);
	while(SPI_I2S_GetFlagStatus(SPI_X, SPI_I2S_FLAG_TXE) == RESET);
	SPI_I2S_SendData(SPI_X,dat);
	while(SPI_I2S_GetFlagStatus(SPI_X, SPI_I2S_FLAG_TXE) == RESET);

	/* Set W5500 SCS High */
	W5500_SCS_HIGH;
}

/* Write W5500 Socket Register 4 byte */
void Write_SOCK_4_Byte(SOCKET s, unsigned short reg, unsigned char *dat_ptr)
{
	/* Set W5500 SCS Low */
	W5500_SCS_LOW;
	
	/* Write Address */
	SPI_I2S_SendData(SPI_X,reg/256);
	while(SPI_I2S_GetFlagStatus(SPI_X, SPI_I2S_FLAG_TXE) == RESET);
	SPI_I2S_SendData(SPI_X,reg);
	while(SPI_I2S_GetFlagStatus(SPI_X, SPI_I2S_FLAG_TXE) == RESET);

	/* Write Control Byte */
	SPI_I2S_SendData(SPI_X,(FDM4|RWB_WRITE|(s*0x20+0x08)));
	while(SPI_I2S_GetFlagStatus(SPI_X, SPI_I2S_FLAG_TXE) == RESET);

	/* Write 4 bytes */
	SPI_I2S_SendData(SPI_X,*dat_ptr);
	while(SPI_I2S_GetFlagStatus(SPI_X, SPI_I2S_FLAG_TXE) == RESET);

	dat_ptr++;
	SPI_I2S_SendData(SPI_X,*dat_ptr);
	while(SPI_I2S_GetFlagStatus(SPI_X, SPI_I2S_FLAG_TXE) == RESET);

	dat_ptr++;
	SPI_I2S_SendData(SPI_X,*dat_ptr);
	while(SPI_I2S_GetFlagStatus(SPI_X, SPI_I2S_FLAG_TXE) == RESET);

	dat_ptr++;
	SPI_I2S_SendData(SPI_X,*dat_ptr);
	while(SPI_I2S_GetFlagStatus(SPI_X, SPI_I2S_FLAG_TXE) == RESET);

	/* Set W5500 SCS High */
	W5500_SCS_HIGH;
}

/******************************* W5500 Read Operation *******************************/
/* Read W5500 Common register 1 Byte */
unsigned char Read_1_Byte(unsigned short reg)
{
	unsigned char i;

	/* Set W5500 SCS Low */
	W5500_SCS_LOW;

	/* Write Address */
	SPI_I2S_SendData(SPI_X,reg/256);
	while(SPI_I2S_GetFlagStatus(SPI_X, SPI_I2S_FLAG_TXE) == RESET);
	SPI_I2S_SendData(SPI_X,reg);
	while(SPI_I2S_GetFlagStatus(SPI_X, SPI_I2S_FLAG_TXE) == RESET);

	/* Write Control Byte */
	SPI_I2S_SendData(SPI_X,(FDM1|RWB_READ|COMMON_R));
	while(SPI_I2S_GetFlagStatus(SPI_X, SPI_I2S_FLAG_TXE) == RESET);

	/* Write a dummy byte */
	i=SPI_I2S_ReceiveData(SPI_X);
	SPI_I2S_SendData(SPI_X,0x00);
	while(SPI_I2S_GetFlagStatus(SPI_X, SPI_I2S_FLAG_TXE) == RESET);

	/* Read 1 byte */
	i=SPI_I2S_ReceiveData(SPI_X);

	/* Set W5500 SCS High*/
	W5500_SCS_HIGH;
	return i;
}

/* Read W5500 Socket register 1 Byte */
unsigned char Read_SOCK_1_Byte(SOCKET s, unsigned short reg)
{
	unsigned char i;

	/* Set W5500 SCS Low */
	W5500_SCS_LOW;

	/* Write Address */
	SPI_I2S_SendData(SPI_X,reg/256);
	while(SPI_I2S_GetFlagStatus(SPI_X, SPI_I2S_FLAG_TXE) == RESET);
	SPI_I2S_SendData(SPI_X,reg);
	while(SPI_I2S_GetFlagStatus(SPI_X, SPI_I2S_FLAG_TXE) == RESET);

	/* Write Control Byte */
	SPI_I2S_SendData(SPI_X,(FDM1|RWB_READ|(s*0x20+0x08)));
	while(SPI_I2S_GetFlagStatus(SPI_X, SPI_I2S_FLAG_TXE) == RESET);

	/* Write a dummy byte */
	i=SPI_I2S_ReceiveData(SPI_X);
	SPI_I2S_SendData(SPI_X,0x00);
	while(SPI_I2S_GetFlagStatus(SPI_X, SPI_I2S_FLAG_TXE) == RESET);

	/* Read 1 byte */
	i=SPI_I2S_ReceiveData(SPI_X);

	/* Set W5500 SCS High*/
	W5500_SCS_HIGH;
	return i;
}

/* Read W5500 Socket register 2 Bytes (short) */
unsigned short Read_SOCK_2_Byte(SOCKET s, unsigned short reg)
{
	unsigned short i;

	/* Set W5500 SCS Low */
	W5500_SCS_LOW;

	/* Write Address */
	SPI_I2S_SendData(SPI_X,reg/256);
	while(SPI_I2S_GetFlagStatus(SPI_X, SPI_I2S_FLAG_TXE) == RESET);
	SPI_I2S_SendData(SPI_X,reg);
	while(SPI_I2S_GetFlagStatus(SPI_X, SPI_I2S_FLAG_TXE) == RESET);

	/* Write Control Byte */
	SPI_I2S_SendData(SPI_X,(FDM2|RWB_READ|(s*0x20+0x08)));
	while(SPI_I2S_GetFlagStatus(SPI_X, SPI_I2S_FLAG_TXE) == RESET);

	/* Write a dummy byte */
	i=SPI_I2S_ReceiveData(SPI_X);
	SPI_I2S_SendData(SPI_X,0x00);
	while(SPI_I2S_GetFlagStatus(SPI_X, SPI_I2S_FLAG_TXE) == RESET);
	i=SPI_I2S_ReceiveData(SPI_X);

	SPI_I2S_SendData(SPI_X,0x00);
	while(SPI_I2S_GetFlagStatus(SPI_X, SPI_I2S_FLAG_TXE) == RESET);
	i*=256;
	i+=SPI_I2S_ReceiveData(SPI_X);

	/* Set W5500 SCS High*/
	W5500_SCS_HIGH;
	return i;
}

/******************** Read data from W5500 Socket data RX Buffer *******************/
unsigned short Read_SOCK_Data_Buffer(SOCKET s, unsigned char *dat_ptr)
{
	unsigned short rx_size;
	unsigned short offset, offset1;
	unsigned short i;
	unsigned char j;

	rx_size=Read_SOCK_2_Byte(s,Sn_RX_RSR);
	if(rx_size==0)		/* if no data received, return */
		return 0;
	if(rx_size>1460)
		rx_size=1460;

	offset=Read_SOCK_2_Byte(s,Sn_RX_RD);
	offset1=offset;
	offset&=(S_RX_SIZE-1);		/* Calculate the real physical address */

	/* Set W5500 SCS Low */
	W5500_SCS_LOW;

	/* Write Address */
	SPI_I2S_SendData(SPI_X,offset/256);
	while(SPI_I2S_GetFlagStatus(SPI_X, SPI_I2S_FLAG_TXE) == RESET);
	SPI_I2S_SendData(SPI_X,offset);
	while(SPI_I2S_GetFlagStatus(SPI_X, SPI_I2S_FLAG_TXE) == RESET);

	/* Write Control Byte */
	SPI_I2S_SendData(SPI_X,(VDM|RWB_READ|(s*0x20+0x18)));		/* Read variable size */
	while(SPI_I2S_GetFlagStatus(SPI_X, SPI_I2S_FLAG_TXE) == RESET);
	j=SPI_I2S_ReceiveData(SPI_X);
	
	if((offset+rx_size)<S_RX_SIZE)
	{
		/* Read Data */
		for(i=0;i<rx_size;i++)
		{
			SPI_I2S_SendData(SPI_X,0x00);		/* Send a dummy byte */
			while(SPI_I2S_GetFlagStatus(SPI_X, SPI_I2S_FLAG_TXE) == RESET);
			j=SPI_I2S_ReceiveData(SPI_X);
			*dat_ptr=j;
			dat_ptr++;
		}
	}
	else
	{
		offset=S_RX_SIZE-offset;
		for(i=0;i<offset;i++)
		{
			SPI_I2S_SendData(SPI_X,0x00); 		/* Send a dummy byte */
			while(SPI_I2S_GetFlagStatus(SPI_X, SPI_I2S_FLAG_TXE) == RESET);
			j=SPI_I2S_ReceiveData(SPI_X);
			*dat_ptr=j;
			dat_ptr++;
		}
		/* Set W5500 SCS High*/
		W5500_SCS_HIGH;

		/* Set W5500 SCS Low */
		W5500_SCS_LOW;
		/* Write Address */
		SPI_I2S_SendData(SPI_X,0x00);
		while(SPI_I2S_GetFlagStatus(SPI_X, SPI_I2S_FLAG_TXE) == RESET);
		SPI_I2S_SendData(SPI_X,0x00);
		while(SPI_I2S_GetFlagStatus(SPI_X, SPI_I2S_FLAG_TXE) == RESET);
		/* Write Control Byte */
		SPI_I2S_SendData(SPI_X,(VDM|RWB_READ|(s*0x20+0x18)));		/* Read variable size */
		while(SPI_I2S_GetFlagStatus(SPI_X, SPI_I2S_FLAG_TXE) == RESET);
		j=SPI_I2S_ReceiveData(SPI_X);
		/* Read Data */
		for(;i<rx_size;i++)
		{
			SPI_I2S_SendData(SPI_X,0x00);		/* Send a dummy byte */
			while(SPI_I2S_GetFlagStatus(SPI_X, SPI_I2S_FLAG_TXE) == RESET);
			j=SPI_I2S_ReceiveData(SPI_X);
			*dat_ptr=j;
			dat_ptr++;
		}
	}
	/* Set W5500 SCS High*/
	W5500_SCS_HIGH;

	/* Update offset*/
	offset1+=rx_size;
	Write_SOCK_2_Byte(s, Sn_RX_RD, offset1);
	Write_SOCK_1_Byte(s, Sn_CR, RECV);					/* Write RECV Command */
	return rx_size;
}

/******************** Write data to W5500 Socket data TX Buffer *******************/
void Write_SOCK_Data_Buffer(SOCKET s, unsigned char *dat_ptr, unsigned short size)
{
	unsigned short offset,offset1;
	unsigned short i;

	offset=Read_SOCK_2_Byte(s,Sn_TX_WR);
	offset1=offset;
	offset&=(S_TX_SIZE-1);		/* Calculate the real physical address */

	/* Set W5500 SCS Low */
	W5500_SCS_LOW;

	/* Write Address */
	SPI_I2S_SendData(SPI_X,offset/256);
	while(SPI_I2S_GetFlagStatus(SPI_X, SPI_I2S_FLAG_TXE) == RESET);
	SPI_I2S_SendData(SPI_X,offset);
	while(SPI_I2S_GetFlagStatus(SPI_X, SPI_I2S_FLAG_TXE) == RESET);

	/* Write Control Byte */
	SPI_I2S_SendData(SPI_X,(VDM|RWB_WRITE|(s*0x20+0x10)));		/* Read variable size */
	while(SPI_I2S_GetFlagStatus(SPI_X, SPI_I2S_FLAG_TXE) == RESET);

	if((offset+size)<S_TX_SIZE)
	{
		/* Write Data */
		for(i=0;i<size;i++)
		{
			SPI_I2S_SendData(SPI_X,*dat_ptr);		/* Send a byte*/
			while(SPI_I2S_GetFlagStatus(SPI_X, SPI_I2S_FLAG_TXE) == RESET);
		
			dat_ptr++;
		}
	}
	else
	{
		offset=S_TX_SIZE-offset;
		for(i=0;i<offset;i++)
		{
			SPI_I2S_SendData(SPI_X,*dat_ptr); 		/* Send a byte */
			while(SPI_I2S_GetFlagStatus(SPI_X, SPI_I2S_FLAG_TXE) == RESET);
	
			dat_ptr++;
		}
		/* Set W5500 SCS High*/
		W5500_SCS_HIGH;

		/* Set W5500 SCS Low */
		W5500_SCS_LOW;
		/* Write Address */
		SPI_I2S_SendData(SPI_X,0x00);
		while(SPI_I2S_GetFlagStatus(SPI_X, SPI_I2S_FLAG_TXE) == RESET);
		SPI_I2S_SendData(SPI_X,0x00);
		while(SPI_I2S_GetFlagStatus(SPI_X, SPI_I2S_FLAG_TXE) == RESET);
		/* Write Control Byte */
		SPI_I2S_SendData(SPI_X,(VDM|RWB_WRITE|(s*0x20+0x10)));		/* Read variable size */
		while(SPI_I2S_GetFlagStatus(SPI_X, SPI_I2S_FLAG_TXE) == RESET);
		/* Read Data */
		for(;i<size;i++)
		{
			SPI_I2S_SendData(SPI_X,*dat_ptr);		/* Send a byte */
			while(SPI_I2S_GetFlagStatus(SPI_X, SPI_I2S_FLAG_TXE) == RESET);
		
			dat_ptr++;
		}
	}
	/* Set W5500 SCS High*/
	W5500_SCS_HIGH;

	/* Updata offset */
	offset1+=size;
	Write_SOCK_2_Byte(s, Sn_TX_WR, offset1);
	Write_SOCK_1_Byte(s, Sn_CR, SEND);					/* Write SEND Command */
}

/*********************** Set Socket n in TCP Client mode ************************/
unsigned int Socket_Connect(SOCKET s)
{
	/* Set Socket n in TCP mode */
	Write_SOCK_1_Byte(s,Sn_MR,MR_TCP);
	/* Open Socket n */
	Write_SOCK_1_Byte(s,Sn_CR,OPEN);

	Delay_ms(5);	/* Wait for a moment */
	if(Read_SOCK_1_Byte(s,Sn_SR)!=SOCK_INIT)
	{
		Write_SOCK_1_Byte(s,Sn_CR,CLOSE);		/* Close Socket n */
		return FALSE;
	}

	/* Set Socket n in Server mode */
	Write_SOCK_1_Byte(s,Sn_CR,CONNECT);
	return TRUE;
}

/*********************** Set Socket n in TCP Server mode ************************/
unsigned int Socket_Listen(SOCKET s)
{
	/* Set Socket n in TCP mode */
	Write_SOCK_1_Byte(s,Sn_MR,MR_TCP);
	/* Open Socket n */
	Write_SOCK_1_Byte(s,Sn_CR,OPEN);

	Delay_ms(5);	/* Wait for a moment */
	if(Read_SOCK_1_Byte(s,Sn_SR)!=SOCK_INIT)
	{
		Write_SOCK_1_Byte(s,Sn_CR,CLOSE);		/* Close Socket n */
		return FALSE;
	}

	/* Set Socket n in Server mode */
	Write_SOCK_1_Byte(s,Sn_CR,LISTEN);
	Delay_ms(5);	/* Wait for a moment */
	if(Read_SOCK_1_Byte(s,Sn_SR)!=SOCK_LISTEN)
	{
		Write_SOCK_1_Byte(s,Sn_CR,CLOSE);		/* Close Socket n */
		return FALSE;
	}
	return TRUE;
}



