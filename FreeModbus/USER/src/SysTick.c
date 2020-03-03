/******************** (C) COPYRIGHT 2014 91mcu **************************
 * �ļ���  ��SysTick.c
 * ����    ��SysTick ϵͳ�δ�ʱ�������ļ�      
 * ʵ��ƽ̨��STM32F103VCT6
 * ��汾  ��ST3.5.0
 *
 * ��д���ڣ�2014-01-20
 * �޸����ڣ�2014-03-11
 * ����    ��zhangsz 
****************************************************************************/
#include "SysTick.h"

__IO uint32_t TimingDelay;

/*
 * ��������SysTick_Init
 * ����	 ������ϵͳ�δ�ʱ�� SysTick
 * ����  : ��
 * ���  ����
 * ����  : �ⲿ����
 */
void SysTick_Init(void)
{
	if(SysTick_Config(SystemCoreClock/1000))	 //1ms��ʱ��
	{
		while(1);
	}
	//SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;			 //���޷�������ر�
}

/*
 * ��������Delay_ms
 * ����	 ��ms��ʱ����1msΪһ����λ
 * ����  : - nTime
 * ���  ����
 * ʾ��  : Delay_us(1) ʵ�ֵ���ʱΪ��1*1ms=1ms
 * ����  ���ⲿ����
 */
void Delay_ms(uint16_t nTime)
{
	TimingDelay = nTime;
	//ʹ��ϵͳ�δ�ʱ��
	while(TimingDelay !=0);
}





