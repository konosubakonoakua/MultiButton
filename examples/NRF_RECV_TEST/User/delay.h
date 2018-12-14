#ifndef __delay_h__
#define __delay_h__

 #include "stm32f1xx_hal.h"
//#include "cmsis_os.h"
//#define delay_ms(t) osDelay(t)
#define delay_ms(t) for(uint32_t i = t*1000; i > 0; i--){/* for(uint8_t j = 72; j > 0; i--) */ __NOP();}
#endif // !__delay_h__

