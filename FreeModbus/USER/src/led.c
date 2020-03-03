/******************** (C) COPYRIGHT 2014 91mcu ***************************
 * �ļ���  ��led.c
 * ����    ��led Ӧ�ú�����         
 * ʵ��ƽ̨��STM32F103VCT6
 * Ӳ�����ӣ�-------------------
 *          |   PD8 - LED1     	|
 *          |   PD9 - LED2     	|
 *          |   PD10 - LED3		|
 *		    |	PD11 - LED4		|				
 *           ------------------- 
 * ��汾  ��ST3.5.0
 * ����    ��zhangsz 
****************************************************************************/
#include "led.h"

/*
 * ��������LED_GPIO_Config
 * ����  ������LED�õ���I/O��
 * ����  ����
 * ���  ����
 */
void LED_GPIO_Config(void)
{		
	/*����һ��GPIO_InitTypeDef���͵Ľṹ��*/
	GPIO_InitTypeDef GPIO_InitStructure;

	/*���� LED1 ��GPIOx������ʱ��*/
	RCC_APB2PeriphClockCmd( RCC_LED1, ENABLE); 
	/*ѡ��Ҫ���Ƶ�LED1 GPIOx����*/															   
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_LED1;	
	/*��������ģʽΪͨ���������*/
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   
	/*������������Ϊ50MHz */   
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	/*���ÿ⺯������ʼ��LED1 ��GPIOx*/
  	GPIO_Init(GPIO_LED1_PORT, &GPIO_InitStructure);		  

	/*���� LED2 ��GPIOx������ʱ��*/
	RCC_APB2PeriphClockCmd( RCC_LED2, ENABLE); 
	/*ѡ��Ҫ���Ƶ�LED2 GPIOx����*/															   
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_LED2;	
	/*��������ģʽΪͨ���������*/
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   
	/*������������Ϊ50MHz */   
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	/*���ÿ⺯������ʼ��LED2 ��GPIOx*/
  	GPIO_Init(GPIO_LED2_PORT, &GPIO_InitStructure);	

		/*���� LED3 ��GPIOx������ʱ��*/
	RCC_APB2PeriphClockCmd( RCC_LED3, ENABLE); 
	/*ѡ��Ҫ���Ƶ�LED3 GPIOx����*/															   
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_LED3;	
	/*��������ģʽΪͨ���������*/
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   
	/*������������Ϊ50MHz */   
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	/*���ÿ⺯������ʼ��LED3 ��GPIOx*/
  	GPIO_Init(GPIO_LED3_PORT, &GPIO_InitStructure);	

		/*���� LED4 ��GPIOx������ʱ��*/
	RCC_APB2PeriphClockCmd( RCC_LED4, ENABLE); 
	/*ѡ��Ҫ���Ƶ�LED4 GPIOx����*/															   
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_LED4;	
	/*��������ģʽΪͨ���������*/
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   
	/*������������Ϊ50MHz */   
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	/*���ÿ⺯������ʼ��LED4 ��GPIOx*/
  	GPIO_Init(GPIO_LED4_PORT, &GPIO_InitStructure);	


	/* �ر�����led��	*/
	GPIO_ResetBits(GPIO_LED1_PORT, GPIO_Pin_LED1);
	GPIO_ResetBits(GPIO_LED2_PORT, GPIO_Pin_LED2);
	GPIO_ResetBits(GPIO_LED3_PORT, GPIO_Pin_LED3);
	GPIO_ResetBits(GPIO_LED4_PORT, GPIO_Pin_LED4);
		 
}


/******************* (C) COPYRIGHT 2014 91mcu *****END OF FILE************/
