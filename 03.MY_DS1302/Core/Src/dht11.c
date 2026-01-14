
#include "main.h"
#include <stdio.h> //printf. gets, fgets
#include <stdlib.h>
#include <string.h>

extern void delay_us(unsigned int us);

void dht11_output(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  GPIO_InitStruct.Pin = DHT11_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(DHT11_GPIO_Port, &GPIO_InitStruct);
}

void dht11_input(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  GPIO_InitStruct.Pin = DHT11_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(DHT11_GPIO_Port, &GPIO_InitStruct);
}

void dht11_main(void)
{
	enum state_t {OK, TIME_OUT, VALUE_ERROR, TRANS_ERROR};
	enum state_t state = OK;
	uint32_t us_counter = 0;

	int data[6] = {0};

	while(1)
	{
		for(int i=0; i<6; i++)
			data[i] = 0;
		state = OK;
		//======request signal======
		dht11_output();
		HAL_GPIO_WritePin(DHT11_GPIO_Port, DHT11_Pin, 0);
		HAL_Delay(20); //20ms

		HAL_GPIO_WritePin(DHT11_GPIO_Port, DHT11_Pin, 1);
		delay_us(30); //30us
		dht11_input();

		us_counter = 0;
		while (HAL_GPIO_ReadPin(DHT11_GPIO_Port,DHT11_Pin)==1)
		{
			delay_us(2);
			us_counter += 2;
			if(us_counter > 50)
			{
				state = TIME_OUT;
				break;
			}
		}
		//---------- response start signal from DHT11-----
		if(state == OK)
		{
			us_counter = 0;
			while (HAL_GPIO_ReadPin(DHT11_GPIO_Port,DHT11_Pin)==0)
				{
					delay_us(2);
					us_counter += 2;
					if(us_counter > 100)
					{
						state = TIME_OUT;
						break;
					}
				}
		}

		if(state == OK)
		{
			us_counter = 0;
			while (HAL_GPIO_ReadPin(DHT11_GPIO_Port,DHT11_Pin)==1)
				{
					delay_us(2);
					us_counter += 2;
					if(us_counter > 100)
					{
						state = TIME_OUT;
						break;
					}
				}
		}

		//---- data read port (총 40비트 = 5바이트 ) -----
		        if(state == OK)
		        {
		            // 5바이트
		            for(int i = 0; i < 5; i++)
		            {
		                for(int j = 0; j < 8; j++) // 8비트 반복 (한 바이트 만들기)
		                {
		                    // 50us 동안의 Low 신호 지나가기 (비트 시작 알림)

		                    us_counter = 0;
		                    while (HAL_GPIO_ReadPin(DHT11_GPIO_Port, DHT11_Pin) == 0)
		                    {
		                        delay_us(1);
		                        us_counter++;
		                        if(us_counter > 100) { state = TIME_OUT; break; }
		                    }

		                    // High 신호 판정

		                    us_counter = 0;
		                    while (HAL_GPIO_ReadPin(DHT11_GPIO_Port, DHT11_Pin) == 1)
		                    {
		                        delay_us(1);
		                        us_counter++;
		                        if(us_counter > 100) { state = TIME_OUT; break; }
		                    }

		                    //
		                    // 일단 기존 데이터를 왼쪽으로 한 칸 밀기
		                    data[i] = data[i] << 1;


		                    if(us_counter > 30)
		                    {
		                        data[i] = data[i] | 1;
		                    }

		                }
		            }
		        }


		        if(state == OK)
		        {

		            uint8_t check_sum = data[0] + data[1] + data[2] + data[3];

		            if(data[4] == check_sum)
		            {
		                printf("[TEMP]: %d.%dC\n", data[2], data[3]);
		                printf("[HUMI]: %d.%d%%\n", data[0], data[1]);
		            }
		            else
		            {
		                printf("Error: Checksum mismatch!\r\n");
		            }
		        }
		        else
		        {
		            printf("Error: Sensor Time out\r\n");
		        }

		HAL_Delay(2000); // 안정화 시간

}
}
