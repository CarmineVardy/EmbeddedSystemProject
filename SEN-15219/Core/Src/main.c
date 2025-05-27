/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
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
#include "i2c.h"
#include "usart.h"
#include "gpio.h"

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

/* USER CODE BEGIN PV */
SparkFun_Bio_Sensor_Hub bioHub;
bioData body;
char msg[64];

// Reset pin, MFIO pin
int resPin = 4;
int mfioPin = 5;

// Possible widths: 69, 118, 215, 411us
int width = 411;
// Possible samples: 50, 100, 200, 400, 800, 1000, 1600, 3200 samples/second
// Not every sample amount is possible with every width; check out our hookup
// guide for more information.
int samples = 400;
int pulseWidthVal;
int sampleVal;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void print(const char *msg);

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
  MX_I2C1_Init();
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */

  SparkFun_Bio_Sensor_Hub_init(&bioHub, resPin, mfioPin);

  int result = SparkFun_Bio_Sensor_Hub_begin(&bioHub);

  if (result == 0)
      print("Sensor started!\r\n");

  print("Configuring Sensor....\r\n");

  int error = SparkFun_Bio_Sensor_Hub_configSensorBpm(&bioHub, MODE_ONE);

  if (error == 0)
      print("Sensor configured.\r\n");
  else {
      print("Error configuring sensor.\r\n");
      snprintf(msg, sizeof(msg), "Error: %d\r\n", error);
      print(msg);
  }

  error = SparkFun_Bio_Sensor_Hub_setPulseWidth(&bioHub, width);
  if (error == 0) // Zero errors.
      print("Pulse Width Set.\r\n");
  else {
      print("Could not set Pulse Width.\r\n");
      snprintf(msg, sizeof(msg), "Error: %d\r\n", error);
      print(msg);
  }

  // Check that the pulse width was set.
  pulseWidthVal = SparkFun_Bio_Sensor_Hub_readPulseWidth(&bioHub);
  snprintf(msg, sizeof(msg), "Pulse Width: %u\r\n", pulseWidthVal);
  print(msg);

  // Set sample rate per second. Remember that not every sample rate is
  // available with every pulse width. Check hookup guide for more information.
  error = SparkFun_Bio_Sensor_Hub_setSampleRate(&bioHub, samples);
  if (error == 0) { // Zero errors.
      print("Sample Rate Set.\r\n");
  }
  else {
      print("Could not set Sample Rate!\r\n");
      snprintf(msg, sizeof(msg), "Error: %d\r\n", error);
      print(msg);
  }

  // Check sample rate.
  sampleVal = SparkFun_Bio_Sensor_Hub_readSampleRate(&bioHub);
  snprintf(msg, sizeof(msg), "Sample rate is set to: %u\r\n", sampleVal);
  print(msg);


  // Data lags a bit behind the sensor, if you're finger is on the sensor when
  // it's being configured this delay will give some time for the data to catch
  // up.
  print("Loading up the buffer with data....\r\n");
  HAL_Delay(4000);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  // Information from the readSensor function will be saved to our "body"
	  // variable.
	  body = SparkFun_Bio_Sensor_Hub_readSensorBpm(&bioHub);

	  // Infrared LED counts
	  snprintf(msg, sizeof(msg), "Infrared LED counts: %lu\r\n", body.irLed);
	  print(msg);

	  // Red LED counts
	  snprintf(msg, sizeof(msg), "Red LED counts: %lu\r\n", body.redLed);
	  print(msg);

	  // Heartrate
	  snprintf(msg, sizeof(msg), "Heartrate: %u.%u bpm\r\n", body.heartRate / 10,body.heartRate % 10);
	  print(msg);

	  // Confidence
	  snprintf(msg, sizeof(msg), "Confidence: %u\r\n", body.confidence);
	  print(msg);

	  // Blood Oxygen
	  snprintf(msg, sizeof(msg), "Blood Oxygen: %u\r\n", body.oxygen);
	  print(msg);

	  // Status
	  snprintf(msg, sizeof(msg), "Status: %u\r\n", body.status);
	  print(msg);

	  // Slow it down or your heart rate will go up trying to keep up
	  // with the flow of numbers
	  HAL_Delay(250);

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

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 16;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
  RCC_OscInitStruct.PLL.PLLQ = 7;
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

/* USER CODE BEGIN 4 */
void print(const char *msg)
{
    HAL_UART_Transmit(&huart2, (uint8_t *)msg, strlen(msg), HAL_MAX_DELAY);
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
