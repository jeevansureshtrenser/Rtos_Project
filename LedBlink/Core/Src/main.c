/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "cmsis_os.h"
#include "queue.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include <stddef.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
#define _USING_QUEUE_
//#define _USING_SEMAPHORE_
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* Definitions for Blinkled */
osThreadId_t BlinkledHandle;
const osThreadAttr_t Blinkled_attributes = {
  .name = "Blinkled",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for UserInput */
osThreadId_t UserInputHandle;
const osThreadAttr_t UserInput_attributes = {
  .name = "UserInput",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityBelowNormal1,
};
/* Definitions for Buttonqueue */
osMessageQueueId_t ButtonqueueHandle;
const osMessageQueueAttr_t Buttonqueue_attributes = {
  .name = "Buttonqueue"
};
/* Definitions for messagequeue */
osMessageQueueId_t messagequeueHandle;
const osMessageQueueAttr_t messagequeue_attributes = {
  .name = "messagequeue"
};
/* Definitions for Button_Sempahore */
osSemaphoreId_t Button_SempahoreHandle;
const osSemaphoreAttr_t Button_Sempahore_attributes = {
  .name = "Button_Sempahore"
};
/* Definitions for Led_Sempahore */
osSemaphoreId_t Led_SempahoreHandle;
const osSemaphoreAttr_t Led_Sempahore_attributes = {
  .name = "Led_Sempahore"
};
/* USER CODE BEGIN PV */
typedef enum
{
	EVENT_BUTTON_PRESSED,
	EVENT_BUTTON_RELEASED
}EVENT_TYPE;

typedef struct
{
	EVENT_TYPE event_type;
	uint16_t usGpio;
}BUTTON_EVENT;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
void BlinkTask(void *argument);
void UserInputTask(void *argument);

/* USER CODE BEGIN PFP */

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


  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* Configure the system clock */
  SystemClock_Config();


  /* Initialize all configured peripherals */
  MX_GPIO_Init();

  /* Init scheduler */
  osKernelInitialize();

  /* Create the semaphores(s) */
  /* creation of Button_Sempahore */
  Button_SempahoreHandle = osSemaphoreNew(1, 1, &Button_Sempahore_attributes);


  /* Create the queue(s) */
  /* creation of messagequeue */
  messagequeueHandle = osMessageQueueNew (16, sizeof(BUTTON_EVENT*), &messagequeue_attributes);



  /* Create the thread(s) */
  /* creation of Blinkled */
  BlinkledHandle = osThreadNew(BlinkTask, NULL, &Blinkled_attributes);

  /* creation of UserInput */
  UserInputHandle = osThreadNew(UserInputTask, NULL, &UserInput_attributes);


  /* Start scheduler */
  osKernelStart();

  /* We should never get here as control is now taken by the scheduler */

  /* Infinite loop */
  while (1)
  {

  }

}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI_DIV2;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL16;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
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

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  /* USER CODE BEGIN MX_GPIO_Init_1 */

  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);

  /*Configure GPIO pin : Button_Pin */
  GPIO_InitStruct.Pin = Button_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(Button_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : USART_TX_Pin USART_RX_Pin */
  GPIO_InitStruct.Pin = USART_TX_Pin|USART_RX_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : PA5 */
  GPIO_InitStruct.Pin = GPIO_PIN_5;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI15_10_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if (GPIO_Pin == GPIO_PIN_13)
	{
		osSemaphoreRelease(Button_SempahoreHandle);
	}
}

/**
  * @brief  Function implementing the Blinkled thread.
  * @param  argument: Not used
  * @retval None
  */
void BlinkTask(void *argument)
{
	BUTTON_EVENT *event = NULL;
	uint32_t uiTaskCounter = 0;
	for(;;)
	{
		if(osMessageQueueGet(messagequeueHandle, &event, 0, osWaitForever) == osOK)
		{
			if (event != NULL)
			{
				HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
				printf("Led Toggle\n");
				vPortFree(event);
			}
		}
		else
		{
			printf(" Data Read from Message Queue failed\n");
		}
		uiTaskCounter++;
		printf("BlinkTask Executed : %ld\n", uiTaskCounter);
		osDelay(1000);
	}
}
/**
* @brief Function implementing the UserInput thread.
* @param argument: Not used
* @retval None
*/
void UserInputTask(void *argument)
{

	uint32_t uiTaskCounter = 0;
	for(;;)
	{
		if(osSemaphoreAcquire(Button_SempahoreHandle,osWaitForever) == osOK)
		{

			printf("Button Pressed\n");
			BUTTON_EVENT *event = pvPortMalloc(sizeof(BUTTON_EVENT));
			if (event != NULL)
			{
			    event->event_type = EVENT_BUTTON_PRESSED;
			    event->usGpio = GPIO_PIN_5;
				if(osMessageQueuePut(messagequeueHandle, &event, 0, osWaitForever) == osOK)
				{
					__NOP();
				}
				else
				{
					printf(" Data Send to Message Queue failed\n");
				}

			}

		}
		uiTaskCounter++;
		printf("UserInputTask Executed : %ld\n", uiTaskCounter);
		osDelay(1000);
	}
}

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM4 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM4)
  {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */

  /* USER CODE END Callback 1 */
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
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
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
