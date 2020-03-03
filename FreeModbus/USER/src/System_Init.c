/******************** (C) COPYRIGHT 2014 91mcu **************************
 * 文件名  ：system_init.c
 * 描述    ：系统初始化       
 * 实验平台：STM32F103VCT6开发板
 * 库版本  ：ST3.5.0
 *
 * 作者    ：zhangsz
 * 论坛    ：http://www.91mcu.com
 * 编写日期：
 * 编写日期：2014-03-11
**********************************************************************************/	

#include "stm32f10x.h"              /* STM32F10x库 */
#include "led.h"
#include "usart1.h"
#include "device.h"
#include "w5500.h"


/*   SPI Initialization  */
void SPI_Configuration(void)
{
	GPIO_InitTypeDef 		GPIO_InitStructure;
	SPI_InitTypeDef   		SPI_InitStructure;

	/*开启GPIOx的外设时钟*/
//	/* Enable SPI1 and GPIOA clocks */
//   RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
//   RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_W5500_SCS, ENABLE);

//  	/* Configure SPI1 pins: SCK, MISO and MOSI -------------*/
//  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
//  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
//  	GPIO_Init(GPIOA, &GPIO_InitStructure);

//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
//   	GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_IPU;   //上拉输入
//   	GPIO_Init(GPIOA, &GPIO_InitStructure);

//	/* Set Chip Select pin */
//	GPIO_InitStructure.GPIO_Pin=GPIO_W5500_SCS_Pin;
//	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
//	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
//	GPIO_Init(GPIO_W5500_SCS_PORT, &GPIO_InitStructure);
//	GPIO_SetBits(GPIO_W5500_SCS_PORT, GPIO_W5500_SCS_Pin);

//	/* Set SPI interface */
//	SPI_InitStructure.SPI_Direction=SPI_Direction_2Lines_FullDuplex;
//	SPI_InitStructure.SPI_Mode=SPI_Mode_Master;
//	SPI_InitStructure.SPI_DataSize=SPI_DataSize_8b;
//	SPI_InitStructure.SPI_CPOL=SPI_CPOL_Low;
//	SPI_InitStructure.SPI_CPHA=SPI_CPHA_1Edge;
//	SPI_InitStructure.SPI_NSS=SPI_NSS_Soft;
//	SPI_InitStructure.SPI_BaudRatePrescaler=SPI_BaudRatePrescaler_4;
//	SPI_InitStructure.SPI_FirstBit=SPI_FirstBit_MSB;
//	SPI_InitStructure.SPI_CRCPolynomial=7;

//	SPI_Init(SPI1,&SPI_InitStructure);

//	SPI_Cmd(SPI1,ENABLE);					//Enable  SPI1

		RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO, ENABLE);
				
		/*!< Configure SPI_FLASH_SPI pins: SCK */
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
		GPIO_Init(GPIOB, &GPIO_InitStructure);
		GPIO_SetBits(GPIOB, GPIO_Pin_12);
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13| GPIO_Pin_14| GPIO_Pin_15;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
		GPIO_Init(GPIOB, &GPIO_InitStructure);
		
		SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
		SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
		SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
		SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
		SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
		SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
		SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;
		SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
		SPI_InitStructure.SPI_CRCPolynomial = 7;
		SPI_Init(SPI_X, &SPI_InitStructure);
		SPI_Cmd(SPI_X, ENABLE);
}

/* IO port Configuration */
void GPIO_Configuration(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	/* Define Reset W5500 output */

		/*开启GPIOx的外设时钟*/
	RCC_APB2PeriphClockCmd( RCC_W5500_RST, ENABLE); 
	GPIO_InitStructure.GPIO_Pin  = GPIO_W5500_RST_Pin;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIO_W5500_RST_PORT, &GPIO_InitStructure);
	GPIO_ResetBits(GPIO_W5500_RST_PORT, GPIO_W5500_RST_Pin);

}

/* system Configuration */
void System_Initialization(void)
{

	/* SPI Configuration */
	SPI_Configuration();

	/* IO Configuration */
	GPIO_Configuration();

	/* LED GPIO Configuration */
	LED_GPIO_Config();

	USART1_Config(115200);

	USART1_NVIC_Configuration();

}

void Socket_X_Config(uint16_t socket_t, uint16_t usTCPPort)
{
	/* set Socket n Port Number */
	Write_SOCK_2_Byte(socket_t, Sn_PORT, usTCPPort);

	Write_SOCK_2_Byte(socket_t,RTR,6000);

	Write_SOCK_2_Byte(socket_t,RCR,3);

	/* Set Maximum Segment Size as 1460 */
	Write_SOCK_2_Byte(socket_t, Sn_MSSR, 1460);

	/* Set RX Buffer Size as 2K */
	Write_SOCK_1_Byte(socket_t,Sn_RXBUF_SIZE, 0x02);
	/* Set TX Buffer Size as 2K */
	Write_SOCK_1_Byte(socket_t,Sn_TXBUF_SIZE, 0x02);
}

