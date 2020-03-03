#ifndef __DEVICE_H
#define	__DEVICE_H

#include "port.h"
//#define TRUE	0xffffffff
//#define FALSE	0x00000000

#define S_RX_SIZE	2048
#define S_TX_SIZE	2048

typedef  unsigned char SOCKET;

#define SPI_X SPI2

/* Reset W5500 ¸´Î»*/
#define RCC_W5500_RST							RCC_APB2Periph_GPIOB
#define GPIO_W5500_RST_PORT				GPIOB
#define GPIO_W5500_RST_Pin				GPIO_Pin_9



/* W5500 SPI Chip select output Æ¬Ñ¡*/
#define RCC_W5500_SCS							RCC_APB2Periph_GPIOB
#define GPIO_W5500_SCS_PORT				GPIOB
#define GPIO_W5500_SCS_Pin				GPIO_Pin_12
#define W5500_SCS_LOW				   		GPIO_ResetBits(GPIO_W5500_SCS_PORT, GPIO_W5500_SCS_Pin)
#define W5500_SCS_HIGH						GPIO_SetBits(GPIO_W5500_SCS_PORT, GPIO_W5500_SCS_Pin)												







//unsigned int S1_SendOK=1;
//unsigned int S1_TimeOut=0;

//unsigned short S0_Port=8000;
//unsigned short S1_Port=9000;

//unsigned char S_Data_Buffer[2048];
//unsigned char S_Data_Buffer1[2048];

extern void System_Initialization(void);

/* Write W5500 Common Register a byte */
extern void Write_1_Byte(unsigned short reg, unsigned char dat);
/* Write W5500 Common Register 2 bytes */
extern void Write_2_Byte(unsigned short reg, unsigned short dat);
/* Write W5500 Common Register n bytes */
extern void Write_Bytes(unsigned short reg, unsigned char *dat_ptr, unsigned short size);

/* Write W5500 Socket Register 1 byte */
extern void Write_SOCK_1_Byte(SOCKET s, unsigned short reg, unsigned char dat);
/* Write W5500 Socket Register 2 byte */
extern void Write_SOCK_2_Byte(SOCKET s, unsigned short reg, unsigned short dat);
/* Write W5500 Socket Register 2 byte */
extern void Write_SOCK_4_Byte(SOCKET s, unsigned short reg, unsigned char *dat_ptr);

/* Read W5500 Common register 1 Byte */
extern unsigned char Read_1_Byte(unsigned short reg);
/* Read W5500 Socket register 1 Byte */
extern unsigned char Read_SOCK_1_Byte(SOCKET s, unsigned short reg);
/* Read W5500 Socket register 2 Bytes (short) */
extern unsigned short Read_SOCK_2_Byte(SOCKET s, unsigned short reg);

/* Read data from W5500 Socket data RX Buffer */
extern unsigned short Read_SOCK_Data_Buffer(SOCKET s, unsigned char *dat_ptr);
/* Write data to W5500 Socket data TX Buffer */
extern void Write_SOCK_Data_Buffer(SOCKET s, unsigned char *dat_ptr, unsigned short size);

/* Set Socket n in TCP Client mode */
extern unsigned int Socket_Connect(SOCKET s);

extern unsigned int Socket_Listen(SOCKET s);

extern void Socket_X_Config(uint16_t socket_t, unsigned short usTCPPort);

extern void modbus_appcall(void);


extern void modbus_sendpoll(void);

extern void Process_IR(void);

extern void Process_LoopBack(void);

extern BOOL check_new(void);
#endif

