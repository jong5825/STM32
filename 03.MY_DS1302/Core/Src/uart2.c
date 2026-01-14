#include "uart2.h"
#include "main.h"   //
#include <string.h> // strcpy, strncmp
#include <stdio.h>  // printf

#define COMMAND_NUMBER 20
#define COMMAND_LENGTH 40

volatile char rx_buffer[COMMAND_NUMBER][COMMAND_LENGTH];
volatile int input_index = 0; // rx interupt에서 사용
volatile int output_index = 0; // user pram에서 사용

extern void DS1302_SetTimeFromStr(char *cmd_buffer);

extern UART_HandleTypeDef huart2;
extern uint8_t rx_data;
extern int func_index;
void pc_command_processing(void);

//1 byte가 수신 될때마다 이곳으로 자동진입
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    volatile static int i = 0;
    if(huart == &huart2)
    {
        uint8_t data;

        data = rx_data;
        if(data == '\n')
        {
            rx_buffer[input_index][i] = '\0'; // 문장의 끝을 알리는 null
            i=0;
            input_index++;
            input_index %= COMMAND_NUMBER;
            // 주의 : queue overflow 체크하는 logic을 넣어야한다.
        }
        else
        {
            rx_buffer[input_index][i++] = data;
        } 
        HAL_UART_Receive_IT(&huart2, &rx_data, 1); //반드시 집어 넣기
    }
}

void pc_command_processing(void)
{
    if(input_index != output_index)
    {
        printf("%s\n", rx_buffer[output_index]); //rx_buffer[output_index][0] 과 동일
        if(strncmp(rx_buffer[output_index], "led_all_on", strlen("led_all_on"))==0)
        {
            func_index = 1;
        }
        else if(strncmp(rx_buffer[output_index], "led_all_off", strlen("led_all_off"))==0)
        {
            func_index = 2;
        }
        else if(strncmp(rx_buffer[output_index], "led_up_on", strlen("led_up_on"))==0)
            {
                func_index = 3;
            }
        else if(strncmp(rx_buffer[output_index], "led_down_off", strlen("led_down_off"))==0)
            {
                func_index = 4;
            }
        else if(strncmp(rx_buffer[output_index], "led_flower_on", strlen("led_flower_on"))==0)
                 {
                     func_index = 5;
                 }
        else if(strncmp(rx_buffer[output_index], "led_flower_off", strlen("led_flower_off"))==0)
                 {
                     func_index = 6;
                 }
        else if(strncmp(rx_buffer[output_index], "setrtc", 6) == 0)
                {
                    DS1302_write((char *)rx_buffer[output_index]);
                }
        output_index++;
        output_index %= COMMAND_NUMBER;
        //주의 : queue full check logic missing
    }
}
