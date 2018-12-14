#include "main.h"
#include "stm32f1xx_hal.h"
#define led_ctrl(_id, _state)\
do{\
  if(_state)\
    LED##_id##_GPIO_Port->BSRR = LED##_id##_Pin;\
  else\
    LED##_id##_GPIO_Port->BSRR = (uint32_t)LED##_id##_Pin << 16U;\
}while(0)

#define led_blink(_id)\
do{\
    LED##_id##_GPIO_Port->ODR ^= LED##_id##_Pin;\
}while(0)