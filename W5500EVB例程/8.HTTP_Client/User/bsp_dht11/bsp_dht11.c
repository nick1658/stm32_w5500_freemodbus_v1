
#include "bsp_dht11.h"
#include "utility.h"

/*
**
*@brief：	配置DHT11用到的I/O口
*@param：	无
*@return: 	无
**
*/
void DHT11_GPIO_Config(void)
{			
	GPIO_InitTypeDef GPIO_InitStructure;			 	/*定义一个GPIO_InitTypeDef类型的结构体*/
	
	RCC_APB2PeriphClockCmd(DHT11_CLK, ENABLE); 		 	/*开启DHT11_PORT的外设时钟*/
																   
  	GPIO_InitStructure.GPIO_Pin = DHT11_PIN;		 	/*选择要控制的DHT11_PORT引脚*/
	
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  	/*设置引脚模式为通用推挽输出*/ 
	   
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 	/*设置引脚速率为50MHz*/
	
  	GPIO_Init(DHT11_PORT, &GPIO_InitStructure);		  	/*调用库函数，初始化DHT11_PORT*/
	
	GPIO_SetBits(DHT11_PORT, GPIO_Pin_10);	 			/*拉高GPIOB10*/
}

/*
**
*@brief：	使DHT11-DATA引脚变为上拉输入模式
*@param：	无
*@return: 	无
**
*/
static void DHT11_Mode_IPU(void)
{
 	  GPIO_InitTypeDef GPIO_InitStructure;
	  		
	  GPIO_InitStructure.GPIO_Pin = DHT11_PIN;			/*选择要控制的DHT11_PORT引脚*/
	    
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU ; 	/*设置引脚模式为浮空输入模式*/
	  
	  GPIO_Init(DHT11_PORT, &GPIO_InitStructure);	 	/*调用库函数，初始化DHT11_PORT*/
}

/*
**
*@brief：	使DHT11-DATA引脚变为推挽输出模式
*@param：	无
*@return: 	无
**
*/
static void DHT11_Mode_Out_PP(void)
{
 	GPIO_InitTypeDef GPIO_InitStructure;
	 																   
  	GPIO_InitStructure.GPIO_Pin = DHT11_PIN;		   	/*选择要控制的DHT11_PORT引脚*/
	
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   	/*设置引脚模式为通用推挽输出*/
	   
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	/*设置引脚速率为50MHz*/

  	GPIO_Init(DHT11_PORT, &GPIO_InitStructure);	 	 	/*调用库函数，初始化DHT11_PORT*/
}

/*
**
*@brief：	从DHT11读取一个字节，MSB先行
*@param：	无
*@return: 	温度
**
*/
static uint8_t Read_Byte(void)
{
	uint8_t i, temp=0;

	for(i=0;i<8;i++)    
	{	 		 
		while(DHT11_DATA_IN()==Bit_RESET);				/*每bit以50us低电平标置开始，轮询直到从机发出的50us低电平结束*/ 

		/*DHT11 以26~28us的高电平表示“0”，以70us高电平表示“1”，
		 *通过检测 x微秒后的电平即可区别这两个状 ，x 即下面的延时 
		*/

		delay_us(40); 									/*延时x微秒,这个延时需要大于数据0持续的时间即可*/	   	  

		if(DHT11_DATA_IN()==Bit_SET)					/*x微秒后仍为高电平表示数据“1”*/
		{		
			while(DHT11_DATA_IN()==Bit_SET);			/*等待数据1的高电平结束*/

			temp|=(uint8_t)(0x01<<(7-i));  				/*把第7-i位置1，MSB先行*/ 
		}
		else	 										/*x微秒后为低电平表示数据“0”*/
		{			   
			temp&=(uint8_t)~(0x01<<(7-i)); 				/*把第7-i位置0，MSB先行*/
		}
	}
	return temp;
}

/*
**
*@brief：	一次完整的数据传输为40bit，高位先出
*			8bit湿度整数 + 8bit 湿度小数 + 8bit 温度整数 + 8bit 温度小数 + 8bit 校验和
*@param：	无
*@return: 	报错
**
*/
uint8_t Read_DHT11(DHT11_Data_TypeDef *DHT11_Data)
{  
	
	DHT11_Mode_Out_PP();						/*输出模式*/
	
	DHT11_DATA_OUT(LOW);						/*主机拉低*/
	
	delay_ms(18);						
		
	DHT11_DATA_OUT(HIGH); 						/*总线拉高，主机延时*/

	delay_us(30);   
	 
	DHT11_Mode_IPU();							/*主机设为输入 判断从机响应信号*/

	/*判断从机是否有低电平响应信号 如不响应则跳出，响应则向下运行*/   
	if(DHT11_DATA_IN()==Bit_RESET)     
	{
		  
		while(DHT11_DATA_IN()==Bit_RESET);	 	/*轮询直到从机发出 的80us 低电平 响应信号结束*/
		
		while(DHT11_DATA_IN()==Bit_SET);		/*轮询直到从机发出的 80us 高电平 标置信号结束*/

		/*开始接收数据*/   
		DHT11_Data->humi_int= Read_Byte();

		DHT11_Data->humi_deci= Read_Byte();

		DHT11_Data->temp_int= Read_Byte();

		DHT11_Data->temp_deci= Read_Byte();

		DHT11_Data->check_sum= Read_Byte();


		
		DHT11_Mode_Out_PP();					/*读取结束，引脚改为输出模式*/
		
		DHT11_DATA_OUT(HIGH);					/*主机拉高*/

		/*检查读取的数据是否正确*/
		if(DHT11_Data->check_sum == DHT11_Data->humi_int + DHT11_Data->humi_deci + DHT11_Data->temp_int+ DHT11_Data->temp_deci)
			return SUCCESS;
		else 
			return ERROR;
	}
	else
	{		
		return ERROR;
	}   
}

