/******************** (C) COPYRIGHT 2014 91mcu ***************************
 * 文件名  ：led.c
 * 描述    ：led 应用函数库         
 * 实验平台：STM32F103VCT6
 * 硬件连接：-------------------
 *          |   PD8 - LED1     	|
 *          |   PD9 - LED2     	|
 *          |   PD10 - LED3		|
 *		    |	PD11 - LED4		|				
 *           ------------------- 
 * 库版本  ：ST3.5.0
 * 作者    ：zhangsz 
****************************************************************************/
#include "led.h"

/*
 * 函数名：LED_GPIO_Config
 * 描述  ：配置LED用到的I/O口
 * 输入  ：无
 * 输出  ：无
 */
void LED_GPIO_Config(void)
{		
	/*定义一个GPIO_InitTypeDef类型的结构体*/
	GPIO_InitTypeDef GPIO_InitStructure;

	/*开启 LED1 的GPIOx的外设时钟*/
	RCC_APB2PeriphClockCmd( RCC_LED1, ENABLE); 
	/*选择要控制的LED1 GPIOx引脚*/															   
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_LED1;	
	/*设置引脚模式为通用推挽输出*/
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   
	/*设置引脚速率为50MHz */   
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	/*调用库函数，初始化LED1 的GPIOx*/
  	GPIO_Init(GPIO_LED1_PORT, &GPIO_InitStructure);		  

	/*开启 LED2 的GPIOx的外设时钟*/
	RCC_APB2PeriphClockCmd( RCC_LED2, ENABLE); 
	/*选择要控制的LED2 GPIOx引脚*/															   
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_LED2;	
	/*设置引脚模式为通用推挽输出*/
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   
	/*设置引脚速率为50MHz */   
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	/*调用库函数，初始化LED2 的GPIOx*/
  	GPIO_Init(GPIO_LED2_PORT, &GPIO_InitStructure);	

		/*开启 LED3 的GPIOx的外设时钟*/
	RCC_APB2PeriphClockCmd( RCC_LED3, ENABLE); 
	/*选择要控制的LED3 GPIOx引脚*/															   
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_LED3;	
	/*设置引脚模式为通用推挽输出*/
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   
	/*设置引脚速率为50MHz */   
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	/*调用库函数，初始化LED3 的GPIOx*/
  	GPIO_Init(GPIO_LED3_PORT, &GPIO_InitStructure);	

		/*开启 LED4 的GPIOx的外设时钟*/
	RCC_APB2PeriphClockCmd( RCC_LED4, ENABLE); 
	/*选择要控制的LED4 GPIOx引脚*/															   
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_LED4;	
	/*设置引脚模式为通用推挽输出*/
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   
	/*设置引脚速率为50MHz */   
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	/*调用库函数，初始化LED4 的GPIOx*/
  	GPIO_Init(GPIO_LED4_PORT, &GPIO_InitStructure);	


	/* 关闭所有led灯	*/
	GPIO_ResetBits(GPIO_LED1_PORT, GPIO_Pin_LED1);
	GPIO_ResetBits(GPIO_LED2_PORT, GPIO_Pin_LED2);
	GPIO_ResetBits(GPIO_LED3_PORT, GPIO_Pin_LED3);
	GPIO_ResetBits(GPIO_LED4_PORT, GPIO_Pin_LED4);
		 
}


/******************* (C) COPYRIGHT 2014 91mcu *****END OF FILE************/
