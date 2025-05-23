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
/**
  * Modifications for the Wearable System for Health Monitoring Project:
  * Authors: Carmine Vardaro, Alessia Parente, Antonio D'Arienzo
  *
  * This project involves the development of a wearable health monitoring system designed to track and analyze various physiological parameters, including:
  * - Step counting using Interlink FSR 402 sensors embedded in footwear, with LEDs indicating activity states (resting, walking, running).
  * - Body temperature measurement via an NTC Thermistor MF52 10K, with the data displayed when activated by a button.
  * - Heart rate (HR) and heart rate variability (HRV) monitoring using the AD8232 module, activated by a separate button.
  *
  * The system aggregates the data and provides real-time warnings for abnormal readings, such as elevated heart rate during rest periods, to ensure optimal health management.
  * Licensed under the GNU General Public License v3.0 (GPL v3).
  * For more details, see: https://www.gnu.org/licenses/gpl-3.0.html
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "usart.h"
#include "gpio.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>                   // Standard I/O functions
#include "interface.h"               // User interface functions
#include "button.h"                  // Button input handling
#include "led.h"                     // LED control functions
#include "switch_channel.h"          // Channel switching logic
#include "activity_tracking.h"       // Activity tracking (step counting)
#include "read_temperature.h"        // Body temperature measurement
#include "ECG_module.h"              // ECG with HR and HRV monitoring
#include "alarms.h"                  // Alarm system for anomalies
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
// Sampling intervals in milliseconds for the 3 tasks
#define STEP_SAMPLING_INTERVAL 10  // Step counting interval (10 ms)
#define ECG_SAMPLING_INTERVAL 5    // ECG sampling interval (5 ms)
#define TEMP_SAMPLING_INTERVAL 500 // Temperature sampling interval (500 ms)
// FOR ACTIVITY TRACKING
#define STEP_BUFFER_SIZE 5 // Size of the step buffer
// FOR ECG
#define SAMPLING_FREQUENCY_ECG 200 // ECG sampling frequency
#define ECG_BUF_LENGHT 4 // Buffer length for the ECG filter
#define ELAB_ECG_BUF_SIZE 1000 // Window size for ECG values (ECG_SAMPLING_INTERVAL * SAMPLING_FREQUENCY_ECG)
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
// Handles the step counter logic, updating the user activity and LED states
void handle_step_counter(ADC_HandleTypeDef *hadc, UserActivity *userActivity, StepBuffer *stepBuffer, LedArray *leds);
// Handles the temperature measurement, updating the temperature data
void handle_temperature(ADC_HandleTypeDef *hadc, TempParam *temp, UserState state);
// Releases the temperature measurement resources after use
void handle_temperature_release(TempParam *temp);
// Handles the ECG data processing, applying filtering and updating the ECG parameters and allarm
void handle_ECG(ADC_HandleTypeDef *hadc, FilterECGParam *filter, ECGParam *ECGparam, uint8_t *message, UserState state);
// Releases the ECG resources when the button is released, updating the ECG parameters
void handle_ECG_button_release(FilterECGParam *filter, ECGParam *ECGparam, uint8_t *message);
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
  MX_USART2_UART_Init();
  MX_ADC1_Init();
  /* USER CODE BEGIN 2 */

  clear_screen();  // Clears the screen display (interface.c)
  init_interface();  // Initializes the user interface (interface.c)

  // Global variables to manage the sampling intervals
  uint32_t last_step_time = 0;  // Time of last step counting update
  uint32_t last_ecg_time = 0;   // Time of last ECG data update
  uint32_t last_temp_time = 0;  // Time of last temperature reading
  uint32_t current_time = 0;    // Holds the current time for interval checks

  uint8_t message_ECG = 0;  // Stores the ECG message for display or processing

  /* Button configuration and initialization (using button.h and button.c) */
  buttonConfig temp_Button, ECG_Button;  // Declare variables for button configuration
  button_init(&temp_Button, PUSH_BUTTON_1_Pin, GPIOB, GPIO_PIN_SET);  // Initialize temperature button (label, port, initial state)
  button_init(&ECG_Button, PUSH_BUTTON_2_Pin, GPIOA, GPIO_PIN_SET);   // Initialize ECG button (label, port, initial state)
  GPIO_PinState b_state;  // Variable to store the current state of the buttons

  /* LED configuration and initialization (using led.h and led.c) */
  LedArray leds;  // Create structure to hold all three LEDs
  led_init(&leds.green_Led, GREEN_LED_Pin, GPIOA, GPIO_PIN_RESET);   // Initialize green LED
  led_init(&leds.yellow_Led, YELLOW_LED_Pin, GPIOB, GPIO_PIN_RESET); // Initialize yellow LED
  led_init(&leds.red_Led, RED_LED_Pin, GPIOC, GPIO_PIN_RESET);       // Initialize red LED

  /* Activity tracking initialization (using activity_tracking.h and activity_tracking.c) */
  UserActivity userActivity;  // Declare the UserActivity structure
  StepBuffer stepBuffer;      // Declare the StepBuffer structure
  uint32_t STEP_buf[STEP_BUFFER_SIZE];  // Buffer to store step data
  UserActivity_init(&userActivity, &leds);  // Initialize user activity tracking with LED states
  StepBuffer_init(&stepBuffer, STEP_buf, STEP_BUFFER_SIZE);  // Initialize step buffer for step tracking

  /* Thermistor structure initialization (using read_temperature.h and read_temperature.c) */
  TempParam tempParam;  // Declare the temperature parameter structure
  init_TemperatureParams(&tempParam);  // Initialize the temperature parameter structure

  /* ECG structure initialization (using ECG_module.h and ECG_module.c) */
  FilterECGParam filterECG;  // Declare the ECG filter structure
  ECGParam ECGparam;        // Declare the ECG parameter structure
  uint16_t ECG_buf[ECG_BUF_LENGHT], elab_ECG_buf[ELAB_ECG_BUF_SIZE];  // Buffers for ECG data and filtered results
  ECG_init_Filter(&filterECG, ECG_buf, ECG_BUF_LENGHT);  // Initialize ECG filter with buffer
  ECG_init_ECG(&ECGparam, elab_ECG_buf, ELAB_ECG_BUF_SIZE);  // Initialize ECG parameters with buffer

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {

	  current_time = HAL_GetTick();  // Get the current system time

	  // Step counter management: only if it's the right time to sample
	  if ((current_time - last_step_time) >= STEP_SAMPLING_INTERVAL) {
	      handle_step_counter(&hadc1, &userActivity, &stepBuffer, &leds);  // Call the function to handle step counting
	      last_step_time = current_time;  // Update the last step sampling time
	  }

	  // Temperature button management
	  b_state = read_button(&temp_Button);  // Read the state of the temperature button, debounce implemented (using button.c)

	  if (b_state == GPIO_PIN_RESET) {  // Temperature button pressed

	      // Only sample if the time interval has passed
	      if ((current_time - last_temp_time) >= TEMP_SAMPLING_INTERVAL) {
	          handle_temperature(&hadc1, &tempParam, userActivity.state);  // Call the function to handle temperature measurement
	          last_temp_time = current_time;  // Update the last temperature sampling time
	      }

	  } else if (temp_Button.previous_state == GPIO_PIN_RESET && b_state == GPIO_PIN_SET) {
	      // Button was released (transitioned from RESET to SET)
	      handle_temperature_release(&tempParam);  // Call the function to handle the temperature button release
	  }

	  // ECG button management
	  b_state = read_button(&ECG_Button);  // Read the state of the ECG button, debounce implemented ((using button.c)

	  if (b_state == GPIO_PIN_RESET) {  // ECG button pressed

	      // Only sample if the time interval has passed
	      if ((current_time - last_ecg_time) >= ECG_SAMPLING_INTERVAL) {
	          handle_ECG(&hadc1, &filterECG, &ECGparam, &message_ECG, userActivity.state);  // Call the function to handle ECG measurement
	          last_ecg_time = current_time;  // Update the last ECG sampling time
	      }

	  } else if (ECG_Button.previous_state == GPIO_PIN_RESET && b_state == GPIO_PIN_SET) {
	      // ECG button was released (transitioned from RESET to SET)
	      handle_ECG_button_release(&filterECG, &ECGparam, &message_ECG);  // Call the function to handle the ECG button release
	  }


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

int _write(int file, char *ptr, int len)
{
	HAL_UART_Transmit(&huart2, (uint8_t *)ptr, len, HAL_MAX_DELAY);
    return len;
}

/*
 * Handles step counting by reading force sensors from both feet.
 * Updates user activity, step count, and LEDs. Displays an error message if sensor readings fail.
 */
void handle_step_counter(ADC_HandleTypeDef *hadc, UserActivity *userActivity, StepBuffer *stepBuffer, LedArray *leds) {

    uint16_t data_out_1, data_out_2;

    // Read the force sensors for the left and right foot (switch_channel.c)
    // If reading from either sensor fails, show an error message
    if (switch_channel_and_read(hadc, ADC_CHANNEL_1, &data_out_1) != HAL_OK ||
        switch_channel_and_read(hadc, ADC_CHANNEL_4, &data_out_2) != HAL_OK) {
        update_activity_message("Error with ADC");  // Display error message on interface (interface.c)
        HAL_Delay(1000);  // Wait for 1 second to show error and stop other interaction
        update_activity_message("              ");  // Clear the error message
    } else {
        // If readings are successful, update the sensor data for both feet
        read_forceSensor(&data_out_1, &userActivity->left_foot);  // Read left foot sensor data
        read_forceSensor(&data_out_2, &userActivity->right_foot); // Read right foot sensor data

        // Update the step count and the LEDs based on the current activity
        // The update are integrated in the file activity_tracking.c for a real time and only necessary change of interface
        update_step_count(userActivity, HAL_GetTick(), stepBuffer, leds);
    }
}
/*
 * Handles temperature reading by switching to the temperature sensor channel.
 * If the reading is successful, it processes the temperature data.
 * If there is an error in the ADC reading, it displays an error message.
 */
void handle_temperature(ADC_HandleTypeDef *hadc, TempParam *temp, UserState state) {

	uint16_t data_out;
    // Read temperature sensor data (switch_cannel.c)
    if (switch_channel_and_read(hadc, ADC_CHANNEL_0, &data_out) != HAL_OK) {
    	update_temperature_message("Error with ADC"); // Display error message on interface (interface.c)
    	HAL_Delay(1000); // Wait for 1 second to show error and stop other interaction
    } else {
    	// Processes the temperature data and updates the state accordingly. (read_temperature.c)
    	// Updates are handled in the 'read_temperature.c' file to ensure real-time processing with minimal interface changes.
    	read_Temperature(temp, data_out, state);
    }
}
/*
 * Resets the temperature parameters and clears the temperature interface
 * when the button is released.
 */
void handle_temperature_release(TempParam *temp){
    // Reset the temperature parameters to default values (read_temperature.c)
    reset_TemperatureParams(temp);
    // Clears the temperature message and progress display. (interface.c)
    clear_temp_interface();
}
/*
 * Function to handle ECG signal acquisition and processing
 * It reads ECG data, processes the signal, calculates HR and HRV, and updates the user interface
 */
void handle_ECG(ADC_HandleTypeDef *hadc, FilterECGParam *filter, ECGParam *ECGparam, uint8_t *message, UserState state) {

    uint16_t data_out;

    // Update the message only once
    if (!(*message)) {
        update_ECG_message("calculating HR..analyzing HRV...hold steady..");  // Display message indicating ECG analysis is in progress (interface.c)
        *message = 1;  // Set the flag to 1 after updating the message to prevent repetition
    }

    // Change the ADC channel, read the value, and check for success (switch_channel.c)
    if (switch_channel_and_read(hadc, ADC_CHANNEL_8, &data_out) != HAL_OK) {
        update_ECG_message("Error with ADC");  // Display error message if ADC reading fails (interface.c)
        *message = 0;  // Reset the message flag
        HAL_Delay(1000);  // Wait for 1 second before retrying to show error and stop other interaction
        return;  // Exit early in case of error
    }

    // Filter the raw signal using the provided filter parameters (ECG_module.c)
    filter_signal(filter, ECGparam, data_out);

    // Calculate the progress of data collection and update the interface
    uint8_t progress = (ECGparam->count * 100) / ECGparam->length;
    update_ECG_progress(progress);  // Update the progress display with the percentage of data collected (interface.c)

    // Check if all the necessary samples have been collected
    if (ECGparam->count >= ECGparam->length) {
        // Process the ECG signal to calculate HR and HRV ((ECG_module.c)
        if (elab_ECG(ECGparam, SAMPLING_FREQUENCY_ECG) != HAL_OK) {
            update_ECG_message("Error ECG elaboration, Retry..");  // Display error message if ECG processing fails (interface.c)
            *message = 0;  // Reset the message flag
            HAL_Delay(1000);  // Wait for 1 second before retrying to show Error
        } else {
            update_ECG(ECGparam->HR, ECGparam->HRV); // Update HR and HRV values in the user interface (interface.c)
            checkECG_Alarm(ECGparam->HR, ECGparam->HRV, state);  // Check if an alarm should be triggered based on HR and HRV (alarm.c)
            *message = 0;  // Reset the message flag after processing
        }
        // Reset the counter to allow new data acquisition
        ECGparam->count = 0;
    }
}
/*
 * Function to handle the release of the ECG button
 * It resets ECG parameters and clears the ECG interface after the process is interrupted or finished
 */
void handle_ECG_button_release(FilterECGParam *filter, ECGParam *ECGparam, uint8_t *message) {

    // Reset the filter parameters and the ECG structure to prepare for the next reading (ECG_module.c)
    reset_ECG(filter, ECGparam);
    // Clear the ECG interface to indicate that the processing has ended or was interrupted (interface.c)
    clear_ECG_interface();
    // Reset the message flag to allow a new message to be shown if necessary
    *message = 0;
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
