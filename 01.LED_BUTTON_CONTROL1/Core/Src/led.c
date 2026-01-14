#include "led.h"
#include "main.h"
void led_main(void);
extern int led_mode;
extern volatile int TIM10_1ms_counter;

// led_all_on
void led_all_on(void)
{
     HAL_GPIO_WritePin(GPIOB, 0xff, 1);
//     HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 |
//                                GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7 ,1);
}


void led_all_off(void)
{
     HAL_GPIO_WritePin(GPIOB, 0xff, 0);
//     HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 |
//                                GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7 ,1);
}

// 7 6 5 4 3 2 1 0
// 100ms주기로 위 동작을 실행 (기존에 on된 led는 off로 처리)
void led_up_on(void)
{
   for(int i=0; i<8; i++) //or 조건이 i<=7; 인 경우 비교를 두번 하게 됨
   {
      led_all_off();
      HAL_GPIO_WritePin(GPIOB,0x01<<i, 1);
      HAL_Delay(100);
   }
}

// 0 1 2 3 4 5 6 7
void led_up_on_reverse(void)
{
   for(int i=0; i<8; i++) //or 조건이 i<=7; 인 경우 비교를 두번 하게 됨
   {
      led_all_off();
      HAL_GPIO_WritePin(GPIOB,0x80>>i, 1);
      HAL_Delay(100);
   }
}
//keep led on
void led_keepon_up(void)
{
    for(int i=0; i < 8; i++)
        {
            HAL_GPIO_WritePin(GPIOB, 0x01 << i, 1);
            HAL_Delay(100);
        }
    led_all_off();
    HAL_Delay(100);
}
//led_flower
void led_flower(void)
{
   for(int i=0; i < 4; i++)
               {
                   HAL_GPIO_WritePin(GPIOB,0x10 << i | 0x08 >> i,1);
                   HAL_Delay(100);
               }

   for(int i=0; i < 4; i++)
               {
                   HAL_GPIO_WritePin(GPIOB,0x01 << i|0x80 >> i,0);
                   HAL_Delay(100);
               }
}

//keep led off
void led_keepon_down(void)
{
    for(int i=0; i < 8; i++)
            {
                HAL_GPIO_WritePin(GPIOB, 0x80 >> i, 1);
                HAL_Delay(100);
            }
        led_all_off();
        HAL_Delay(100);
}



void led_main(void)
{
   while(1)
   {
        if(TIM10_1ms_counter >= 100)
        {
            TIM10_1ms_counter = 0;
            HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
        }
	   button_check();
//      led_all_on();
//      HAL_Delay(50);//ms
//
//      led_all_off();
//       HAL_Delay(50);//ms
//       led_up_on();
//       led_up_on_reverse();
	   switch (led_mode){
	   case 1:	led_all_on();	break;
	   case 2:	led_all_off();	break;
	   case 3:	led_up_on();	break;
	   case 4:	led_up_on_reverse();	break;
	   case 5:	led_flower();	break;
	   case 6:	led_all_off();	break;
	   default: break;
	   }
   }
}
