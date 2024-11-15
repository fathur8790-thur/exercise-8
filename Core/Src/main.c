/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

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
/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for FlashGreenLed */
osThreadId_t FlashGreenLedHandle;
const osThreadAttr_t FlashGreenLed_attributes = {
  .name = "FlashGreenLed",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for FlashRedLed */
osThreadId_t FlashRedLedHandle;
const osThreadAttr_t FlashRedLed_attributes = {
  .name = "FlashRedLed",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for FlashOrangeLed */
osThreadId_t FlashOrangeLedHandle;
const osThreadAttr_t FlashOrangeLed_attributes = {
  .name = "FlashOrangeLed",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityAboveNormal,
};
/* Definitions for CriticalResourceSemaphore */
osSemaphoreId CriticalResourceSemaphoreHandle;
const osSemaphoreAttr_t CriticalResourceSemaphore_attributes = {
  .name = "CriticalResourceSemaphore"
};
/* USER CODE BEGIN PV */
osSemaphoreDef(CriticalResourceSemaphore);


volatile int StartFlag = 1;

void AccessSharedResource() {
    // Try to acquire the semaphore, but do not block
    if (osSemaphoreAcquire(CriticalResourceSemaphoreHandle, osWaitForever) == osOK) {
        // Resource is available, proceed with accessing the shared resource
        if (StartFlag == 1) {
            StartFlag = 0;  // Mark the resource as being accessed
        } else {
            HAL_GPIO_WritePin(LED_Indicator_GPIO_Port, LED_Indicator_Pin, GPIO_PIN_SET); // Blue LED ON, indicating contention
        }

        osDelay(1000);  // Simulate working with the resource

        StartFlag = 1;  // Mark the resource as available again
        HAL_GPIO_WritePin(LED_Indicator_GPIO_Port, LED_Indicator_Pin, GPIO_PIN_RESET);  // Blue LED OFF

        // Release the semaphore after finishing with the resource
        osSemaphoreRelease(CriticalResourceSemaphoreHandle);  // Release the semaphore
    } else {
        // Semaphore is already taken by another task
        // Handle the case when resource is unavailable (optional)
        HAL_GPIO_WritePin(LED_Indicator_GPIO_Port, LED_Indicator_Pin, GPIO_PIN_SET);  // Optionally, show contention by turning on Blue LED
    }
}

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
void StartDefaultTask(void *argument);
void FlashGreenLedTask(void *argument);
void FlashRedLedTask(void *argument);
void FlashOrangeLedTask(void *argument);

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
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Init scheduler */
  osKernelInitialize();

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* Create the semaphores(s) */
  /* creation of CriticalResourceSemaphore */
  CriticalResourceSemaphoreHandle = osSemaphoreNew(1, 1, &CriticalResourceSemaphore_attributes);

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of defaultTask */
  defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

  /* creation of FlashGreenLed */
  FlashGreenLedHandle = osThreadNew(FlashGreenLedTask, NULL, &FlashGreenLed_attributes);

  /* creation of FlashRedLed */
  FlashRedLedHandle = osThreadNew(FlashRedLedTask, NULL, &FlashRedLed_attributes);

  /* creation of FlashOrangeLed */
  FlashOrangeLedHandle = osThreadNew(FlashOrangeLedTask, NULL, &FlashOrangeLed_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

  /* Start scheduler */
  osKernelStart();

  /* We should never get here as control is now taken by the scheduler */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
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

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
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
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(Orange_LED_GPIO_Port, Orange_LED_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, Green_LED_Pin|Red_LED_Pin|LED_Indicator_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : Orange_LED_Pin */
  GPIO_InitStruct.Pin = Orange_LED_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(Orange_LED_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : Green_LED_Pin Red_LED_Pin LED_Indicator_Pin */
  GPIO_InitStruct.Pin = Green_LED_Pin|Red_LED_Pin|LED_Indicator_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument)
{
  /* USER CODE BEGIN 5 */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END 5 */
}

/* USER CODE BEGIN Header_FlashGreenLedTask */
/**
* @brief Function implementing the FlashGreenLed thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_FlashGreenLedTask */
void FlashGreenLedTask(void *argument)
{
  /* USER CODE BEGIN FlashGreenLedTask */
  /* Infinite loop */
  for(;;)
  {
	  HAL_GPIO_WritePin(Green_LED_GPIO_Port, Green_LED_Pin, GPIO_PIN_SET); // Turn Blue LED on
	  AccessSharedResource();
	  osDelay(200);
	  HAL_GPIO_WritePin(Green_LED_GPIO_Port, Green_LED_Pin,	 GPIO_PIN_RESET);
	  osDelay(200);
  }
  /* USER CODE END FlashGreenLedTask */
}

/* USER CODE BEGIN Header_FlashRedLedTask */
/**
* @brief Function implementing the FlashRedLed thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_FlashRedLedTask */
void FlashRedLedTask(void *argument)
{
  /* USER CODE BEGIN FlashRedLedTask */
  /* Infinite loop */
  for(;;)
  {
	  HAL_GPIO_WritePin(Red_LED_GPIO_Port, Red_LED_Pin, GPIO_PIN_SET); // Turn Blue LED on
	  AccessSharedResource();
	  osDelay(550);
	  HAL_GPIO_WritePin(Red_LED_GPIO_Port, Red_LED_Pin, GPIO_PIN_RESET);
	  osDelay(550);
  }
  /* USER CODE END FlashRedLedTask */
}

/* USER CODE BEGIN Header_FlashOrangeLedTask */
/**
* @brief Function implementing the FlashOrangeLed thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_FlashOrangeLedTask */
void FlashOrangeLedTask(void *argument)
{
  /* USER CODE BEGIN FlashOrangeLedTask */
  /* Infinite loop */
  for(;;)
  {
	  HAL_GPIO_WritePin(Orange_LED_GPIO_Port, Orange_LED_Pin, GPIO_PIN_SET); // Turn Blue LED on
	  osDelay(50);
	  HAL_GPIO_WritePin(Orange_LED_GPIO_Port, Orange_LED_Pin, GPIO_PIN_RESET);
	  osDelay(50);
  }
  /* USER CODE END FlashOrangeLedTask */
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
