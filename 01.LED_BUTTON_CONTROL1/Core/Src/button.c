#include "button.h"
#include "main.h"
unsigned char button_status[BUTTON_NUMBER]={
        BUTTON_RELEASE,
        BUTTON_RELEASE,
        BUTTON_RELEASE,
        BUTTON_RELEASE,
        BUTTON_RELEASE
};
int led_mode=0;
void button_check(void)
{
    //버튼을 1번 눌렀다 떼는지 체크
    //BUTTON0 ---> LED0을 toggle
    //BUTTON1 ---> LED1을 toggle
    //BUTTON2 ---> LED2을 toggle
    //BUTTON3 ---> LED3을 toggle
    //BUTTON4 ---> LED4을 toggle
    if (get_button(GPIOC, GPIO_PIN_0, BUTTON0) == BUTTON_PRESS)
        {
            led_mode++;
            if(led_mode>6)led_mode =1;
//            HAL_GPIO_TogglePin(GPIOB,GPIO_PIN_0);
        }
    if (get_button(GPIOC, GPIO_PIN_1, BUTTON1) == BUTTON_PRESS)
        {
            HAL_GPIO_TogglePin(GPIOB,GPIO_PIN_1);
        }
    if (get_button(GPIOC, GPIO_PIN_2, BUTTON2) == BUTTON_PRESS)
        {
            HAL_GPIO_TogglePin(GPIOB,GPIO_PIN_2);
        }
    if (get_button(GPIOC, GPIO_PIN_3, BUTTON3) == BUTTON_PRESS)
        {
            HAL_GPIO_TogglePin(GPIOB,GPIO_PIN_3);
        }
    if (get_button(GPIOC, GPIO_PIN_13, BUTTON4) == BUTTON_PRESS)
        {
            HAL_GPIO_TogglePin(GPIOB,GPIO_PIN_4);
        }

}

int get_button(GPIO_TypeDef* GPIOx, uint16_t GPIO_pin, int button_number)
{
    int state = HAL_GPIO_ReadPin(GPIOx, GPIO_pin);

    // (1) 처음 눌려진 상태 감지
    if(state == BUTTON_PRESS && button_status[button_number] == BUTTON_RELEASE)
    {
        HAL_Delay(20); // 디바운싱
        button_status[button_number] = BUTTON_PRESS; // 현재 번호의 상태 업데이트
        return BUTTON_PRESS; // 눌림 즉시 보고
    }
    // (2) 눌려 있다가 떼어진 상태 감지
    else if(state == BUTTON_RELEASE && button_status[button_number] == BUTTON_PRESS)
    {
        HAL_Delay(20);
        button_status[button_number] = BUTTON_RELEASE; // 현재 번호의 상태 업데이트
        return BUTTON_RELEASE; // 떼어짐 보고
    }

    return BUTTON_RELEASE; // 아무 일 없을 때
}
