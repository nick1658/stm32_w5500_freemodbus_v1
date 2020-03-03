/******************** (C) COPYRIGHT 2014 ERC3100 ***************************
 * �ļ���  ��led.h
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
#ifndef __LED_H
#define	__LED_H

#include "stm32f10x.h"

/* the macro definition to trigger the led on or off 
 * 1 - off
 - 0 - on
 */
#define ON  0
#define OFF 1

#define RCC_LED1				RCC_APB2Periph_GPIOD
#define GPIO_LED1_PORT			GPIOD
#define GPIO_Pin_LED1			GPIO_Pin_8

#define RCC_LED2				RCC_APB2Periph_GPIOD
#define GPIO_LED2_PORT			GPIOD
#define GPIO_Pin_LED2			GPIO_Pin_9

#define RCC_LED3				RCC_APB2Periph_GPIOD
#define GPIO_LED3_PORT			GPIOD
#define GPIO_Pin_LED3			GPIO_Pin_10

#define RCC_LED4				RCC_APB2Periph_GPIOD
#define GPIO_LED4_PORT			GPIOD
#define GPIO_Pin_LED4			GPIO_Pin_11



//���κ꣬��������������һ��ʹ��
#define LED1(a)	if (a)	\
					GPIO_ResetBits(GPIO_LED1_PORT,GPIO_Pin_LED1);\
					else		\
					GPIO_SetBits(GPIO_LED1_PORT,GPIO_Pin_LED1)

#define LED2(a)	if (a)	\
					GPIO_ResetBits(GPIO_LED2_PORT,GPIO_Pin_LED2);\
					else		\
					GPIO_SetBits(GPIO_LED2_PORT,GPIO_Pin_LED2)

#define LED3(a)	if (a)	\
					GPIO_ResetBits(GPIO_LED3_PORT,GPIO_Pin_LED3);\
					else		\
					GPIO_SetBits(GPIO_LED3_PORT,GPIO_Pin_LED3)

#define LED4(a)	if (a)	\
					GPIO_ResetBits(GPIO_LED4_PORT,GPIO_Pin_LED4);\
					else		\
					GPIO_SetBits(GPIO_LED4_PORT,GPIO_Pin_LED4)

void LED_GPIO_Config(void);

#endif /* __LED_H */
