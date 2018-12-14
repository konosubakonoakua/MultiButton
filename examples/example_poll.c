#include "multi_button.h"

New_Button(btn1, 1);

uint8_t Multi_Button_Read_Callback(uint8_t id) 
{
	switch(id)
	{
		case 1:
			return HAL_GPIO_ReadPin(B2_GPIO_Port, B2_Pin);
	}
}


int _main()
{
	static uint8_t btn1_event_val;
	button_init_and_start(&btn1,0);
	
	//make the timer invoking the button_ticks() interval 5ms.
	//This function is implemented by yourself.
	__timer_start(button_ticks, 0, 5); 
	
	while(1) 
	{
		if(btn1_event_val != get_button_event(&btn1)) {
			btn1_event_val = get_button_event(&btn1);
			
			if(btn1_event_val == PRESS_DOWN) {
				//do something
			} else if(btn1_event_val == PRESS_UP) {
				//do something
			} else if(btn1_event_val == LONG_PRESS_HOLD) {
				//do something
			}
		}
	}
}

