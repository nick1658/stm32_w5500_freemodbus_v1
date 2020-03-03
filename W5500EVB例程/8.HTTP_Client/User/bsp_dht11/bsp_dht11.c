
#include "bsp_dht11.h"
#include "utility.h"

/*
**
*@brief��	����DHT11�õ���I/O��
*@param��	��
*@return: 	��
**
*/
void DHT11_GPIO_Config(void)
{			
	GPIO_InitTypeDef GPIO_InitStructure;			 	/*����һ��GPIO_InitTypeDef���͵Ľṹ��*/
	
	RCC_APB2PeriphClockCmd(DHT11_CLK, ENABLE); 		 	/*����DHT11_PORT������ʱ��*/
																   
  	GPIO_InitStructure.GPIO_Pin = DHT11_PIN;		 	/*ѡ��Ҫ���Ƶ�DHT11_PORT����*/
	
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  	/*��������ģʽΪͨ���������*/ 
	   
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 	/*������������Ϊ50MHz*/
	
  	GPIO_Init(DHT11_PORT, &GPIO_InitStructure);		  	/*���ÿ⺯������ʼ��DHT11_PORT*/
	
	GPIO_SetBits(DHT11_PORT, GPIO_Pin_10);	 			/*����GPIOB10*/
}

/*
**
*@brief��	ʹDHT11-DATA���ű�Ϊ��������ģʽ
*@param��	��
*@return: 	��
**
*/
static void DHT11_Mode_IPU(void)
{
 	  GPIO_InitTypeDef GPIO_InitStructure;
	  		
	  GPIO_InitStructure.GPIO_Pin = DHT11_PIN;			/*ѡ��Ҫ���Ƶ�DHT11_PORT����*/
	    
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU ; 	/*��������ģʽΪ��������ģʽ*/
	  
	  GPIO_Init(DHT11_PORT, &GPIO_InitStructure);	 	/*���ÿ⺯������ʼ��DHT11_PORT*/
}

/*
**
*@brief��	ʹDHT11-DATA���ű�Ϊ�������ģʽ
*@param��	��
*@return: 	��
**
*/
static void DHT11_Mode_Out_PP(void)
{
 	GPIO_InitTypeDef GPIO_InitStructure;
	 																   
  	GPIO_InitStructure.GPIO_Pin = DHT11_PIN;		   	/*ѡ��Ҫ���Ƶ�DHT11_PORT����*/
	
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   	/*��������ģʽΪͨ���������*/
	   
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	/*������������Ϊ50MHz*/

  	GPIO_Init(DHT11_PORT, &GPIO_InitStructure);	 	 	/*���ÿ⺯������ʼ��DHT11_PORT*/
}

/*
**
*@brief��	��DHT11��ȡһ���ֽڣ�MSB����
*@param��	��
*@return: 	�¶�
**
*/
static uint8_t Read_Byte(void)
{
	uint8_t i, temp=0;

	for(i=0;i<8;i++)    
	{	 		 
		while(DHT11_DATA_IN()==Bit_RESET);				/*ÿbit��50us�͵�ƽ���ÿ�ʼ����ѯֱ���ӻ�������50us�͵�ƽ����*/ 

		/*DHT11 ��26~28us�ĸߵ�ƽ��ʾ��0������70us�ߵ�ƽ��ʾ��1����
		 *ͨ����� x΢���ĵ�ƽ��������������״ ��x ���������ʱ 
		*/

		delay_us(40); 									/*��ʱx΢��,�����ʱ��Ҫ��������0������ʱ�伴��*/	   	  

		if(DHT11_DATA_IN()==Bit_SET)					/*x΢�����Ϊ�ߵ�ƽ��ʾ���ݡ�1��*/
		{		
			while(DHT11_DATA_IN()==Bit_SET);			/*�ȴ�����1�ĸߵ�ƽ����*/

			temp|=(uint8_t)(0x01<<(7-i));  				/*�ѵ�7-iλ��1��MSB����*/ 
		}
		else	 										/*x΢���Ϊ�͵�ƽ��ʾ���ݡ�0��*/
		{			   
			temp&=(uint8_t)~(0x01<<(7-i)); 				/*�ѵ�7-iλ��0��MSB����*/
		}
	}
	return temp;
}

/*
**
*@brief��	һ�����������ݴ���Ϊ40bit����λ�ȳ�
*			8bitʪ������ + 8bit ʪ��С�� + 8bit �¶����� + 8bit �¶�С�� + 8bit У���
*@param��	��
*@return: 	����
**
*/
uint8_t Read_DHT11(DHT11_Data_TypeDef *DHT11_Data)
{  
	
	DHT11_Mode_Out_PP();						/*���ģʽ*/
	
	DHT11_DATA_OUT(LOW);						/*��������*/
	
	delay_ms(18);						
		
	DHT11_DATA_OUT(HIGH); 						/*�������ߣ�������ʱ*/

	delay_us(30);   
	 
	DHT11_Mode_IPU();							/*������Ϊ���� �жϴӻ���Ӧ�ź�*/

	/*�жϴӻ��Ƿ��е͵�ƽ��Ӧ�ź� �粻��Ӧ����������Ӧ����������*/   
	if(DHT11_DATA_IN()==Bit_RESET)     
	{
		  
		while(DHT11_DATA_IN()==Bit_RESET);	 	/*��ѯֱ���ӻ����� ��80us �͵�ƽ ��Ӧ�źŽ���*/
		
		while(DHT11_DATA_IN()==Bit_SET);		/*��ѯֱ���ӻ������� 80us �ߵ�ƽ �����źŽ���*/

		/*��ʼ��������*/   
		DHT11_Data->humi_int= Read_Byte();

		DHT11_Data->humi_deci= Read_Byte();

		DHT11_Data->temp_int= Read_Byte();

		DHT11_Data->temp_deci= Read_Byte();

		DHT11_Data->check_sum= Read_Byte();


		
		DHT11_Mode_Out_PP();					/*��ȡ���������Ÿ�Ϊ���ģʽ*/
		
		DHT11_DATA_OUT(HIGH);					/*��������*/

		/*����ȡ�������Ƿ���ȷ*/
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

