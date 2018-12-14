#include "multi_button.h"
#include "stm32f1xx_hal.h"
#include "main.h"

New_Button(btn1, 1); // btn1 是名字，1是编号
New_Button(btn2, 2);

int _main()
{
	... // 初始化代码，省略
	button_init_and_start(&btn1, 0);
	button_init_and_start(&btn2, 0);
	HAL_TIM_Base_Start_IT(&htim4);

	//make the timer invoking the button_ticks() interval 5ms.
	//This function is implemented by yourself.
	__timer_start(button_ticks, 0, 5); // 在5ms中断里面调用button_ticks()
	
	while(1) 
	{}
}

// IO电平读取回调函数
/**
  * @brief  Global Read CallBack func
  * @param  button id
  * @retval button level
  */
uint8_t Multi_Button_Read_Callback(Button* handle)
{
	switch(handle->id)
	{
		case 1:
			return HAL_GPIO_ReadPin(KEY1_GPIO_Port, KEY1_Pin);
		case 2:
			return HAL_GPIO_ReadPin(KEY1_GPIO_Port, KEY2_Pin);
    default:
      return 1; //off
	}
}
// 事件处理回调函数
/**
  * @brief  Global Event CallBack func
  * @param  button id & event id
  * @retval None
  */
void Multi_Button_Event_Callback(Button* handle)
{
	// do your own business
	switch(handle->event)
	{
		case PRESS_DOWN:
		{}break;
		case PRESS_UP:
    	{}break;
		case PRESS_REPEAT:
    	{
			switch(handle->id)
			{
				case 1:
				nrf_send(">>>btn1 repeated: ", 18);
				uint8_t i = handle->repeat + 0x30;
				nrf_send(&i, 1);
			led_ctrl(1, 1);
				break;
				case 2:
				nrf_send(">>>btn2 repeated: ", 18);
				uint8_t j = handle->repeat + 0x30;
				nrf_send(&j, 1);
				led_ctrl(2, 1);
				break;
			}

    	}break;
		case SINGLE_CLICK:
    	{
			switch(handle->id)
			{
				case 1:
				nrf_send("button1 single click  ", 22);
				led_ctrl(1, 0);
				break;
				case 2:
				nrf_send("button2 single click  ", 22);
				led_ctrl(2, 0);
				break;
			}
		}break;
		case DOUBLE_CLICK:
    	{
			switch(handle->id)
			{
				case 1:
				nrf_send("button1 double click  ", 22);
				led_ctrl(1, 1);
				break;
				case 2:
				nrf_send("button2 double click  ", 22);
				led_ctrl(2, 1);
				break;
			}
    	}break;
		case LONG_PRESS_HOLD:
    	{}break;
		case LONG_RRESS_START:
    	{
			switch(handle->id)
			{
				case 1:
				nrf_send("button1 long press  ", 20);
				led_blink(1);
				break;
				case 2:
				nrf_send("button2 long press  ", 20);
				led_blink(2);
				break;
			}
    	}break;
		default:
			break;
	}
}