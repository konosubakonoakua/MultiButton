/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  ** This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * COPYRIGHT(c) 2018 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "tim.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "nrf24l01_reg.h"
#include "nrf24l01.h"
#include "delay.h"
#include "util.h"
#include "led.h"
#include "multi_button.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
extern uint8_t RF_RX_FLAG;
extern uint8_t NRF_rx_buf[];
uint8_t txbuf[8] = "akua";
New_Button(btn1, 1);
New_Button(btn2, 2);
//New_Button(btn3, 3);
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
extern void nrf_send(uint8_t *txbuf, uint8_t size);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */


/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_TIM4_Init();
  /* USER CODE BEGIN 2 */
  led_ctrl(1, 1);
  nRF24L01_Init(nRF_RX_Mode, 0);
  while (!NRF_Check(0)) // check connection with nrf24l01.
  {
    led_blink(2);
    HAL_Delay(20);
  }
  nrf_send(txbuf, 4);
  led_ctrl(1, 0);
  button_init_and_start(&btn1, 0);
  button_init_and_start(&btn2, 0);
  HAL_TIM_Base_Start_IT(&htim4);
//  button_start(&btn3);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
    nRF24L01_Revceive(0); // polling data
    if(RF_RX_FLAG)
    {
      __NOP();
      RF_RX_FLAG = 0;
    }
//    led_blink(1);
//    led_blink(2);
    HAL_Delay(50);
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /**Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /**Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
/**
  * @brief  Global CallBack func
  * @param  button id & event id
  * @retval None
  */
void Multi_Button_Event_Callback(Button* handle)
{
	// do your own business
	switch(handle->event)
	{
		case PRESS_DOWN:
		{
		}break;
		case PRESS_UP:
    {
		}break;
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
    }
			break;
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
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  if(htim->Instance == TIM4)
  {
    button_ticks();
  }
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
