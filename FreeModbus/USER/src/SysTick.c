/******************** (C) COPYRIGHT 2014 91mcu **************************
 * 文件名  ：SysTick.c
 * 描述    ：SysTick 系统滴答定时器配置文件      
 * 实验平台：STM32F103VCT6
 * 库版本  ：ST3.5.0
 *
 * 编写日期：2014-01-20
 * 修改日期：2014-03-11
 * 作者    ：zhangsz 
****************************************************************************/
#include "SysTick.h"

__IO uint32_t TimingDelay;

/*
 * 函数名：SysTick_Init
 * 描述	 ：启动系统滴答定时器 SysTick
 * 输入  : 无
 * 输出  ：无
 * 调用  : 外部调用
 */
void SysTick_Init(void)
{
	if(SysTick_Config(SystemCoreClock/1000))	 //1ms定时器
	{
		while(1);
	}
	//SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;			 //若无法启动则关闭
}

/*
 * 函数名：Delay_ms
 * 描述	 ：ms延时程序，1ms为一个单位
 * 输入  : - nTime
 * 输出  ：无
 * 示例  : Delay_us(1) 实现的延时为：1*1ms=1ms
 * 调用  ：外部调用
 */
void Delay_ms(uint16_t nTime)
{
	TimingDelay = nTime;
	//使能系统滴答定时器
	while(TimingDelay !=0);
}





