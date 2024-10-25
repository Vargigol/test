#include "Kod.h"
#include "main.h"
//#include "stdint.h"


#define datasize 10

extern UART_HandleTypeDef huart1;



//функции
void UART_Send_msg(int16_t msg1, int16_t msg2, int16_t msg3, int16_t msg4);
void Parsing();

//переменные
//int16_t Total_Ndot_ADC = Ndot_ADC * Number_CHANNEL_ADC;		//общие количество отчетов
int Flag_print = 0;
uint16_t counter = 0;
uint8_t data[datasize] = {0,};
uint8_t KeyCmd[] = "start";
uint8_t KeySum = 0;

// флаги
//uint8_t Flag_Tim7 = 0;										//Флаг работы таймера АЦП
//uint8_t Full_buffer_ADC_f0_In = 0;							//Флаг заполности буфера АЦП

int main1(void)
{


	while(1) //основной цикл
	{

		KeySum = 0;
		HAL_UART_Receive_IT(&huart1,data, sizeof(data));
		Parsing();

		if (KeySum == 5)
		{
//			HAL_UART_Transmit(&huart1,(uint8_t *)data,sizeof(data),150);
//			HAL_UART_Transmit(&huart1,(uint8_t *)"     start\r",11,15);
			for(uint16_t i = 0; i < 5000; i++)
			{
				UART_Send_msg(counter, i,i,i);
			}
			HAL_UART_Transmit(&huart1,(uint8_t *)"     finish\r",11,15);
			counter++;
		}


	}
}




void UART_Send_msg(int16_t msg1, int16_t msg2, int16_t msg3, int16_t msg4)
{
	uint8_t transmitBuffer1[11] = {0,};						// буфер на отправку по уарту
	transmitBuffer1[0] = 240;
	transmitBuffer1[1] = 176;
	transmitBuffer1[2] = 239;
	transmitBuffer1[3] = msg1 % 255;		//остаток от деления
	transmitBuffer1[4] = msg1 / 255;		// целая часть
	transmitBuffer1[5] = msg2 % 255;
	transmitBuffer1[6] = msg2 / 255;
	transmitBuffer1[7] = msg3 % 255;
	transmitBuffer1[8] = msg3 / 255;
	transmitBuffer1[9] = msg4 % 255;
	transmitBuffer1[10] = msg4 / 255;

	HAL_UART_Transmit(&huart1,  (uint8_t*)transmitBuffer1, sizeof(transmitBuffer1), 10); //отправка  время на отправку в мс, 12байт отправляет примерно за 1мс
//	HAL_Delay(50);
}

void Parsing()
{
	for(int i = 0; i < datasize; i++)
	{
		if (data[i] == KeyCmd[KeySum])
		{
			KeySum++;
		}else
		{
			KeySum = 0;
		}
		if (KeySum == (sizeof(KeyCmd)-1))
		{
			for(int i = 0; i < datasize; i++)
			{
				data[i] = 0;
			}
			break;
		}
	}
}



void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
          if(huart == &huart1)
          {
        	  Flag_print = 1;
          }
}

