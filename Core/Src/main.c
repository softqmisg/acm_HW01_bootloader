/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under Ultimate Liberty license
 * SLA0044, the "License"; You may not use this file except in compliance with
 * the License. You may obtain a copy of the License at:
 *                             www.st.com/SLA0044
 *
 ******************************************************************************
 */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "crc.h"
#include "dma.h"
#include "fatfs.h"
#include "rtc.h"
#include "sdio.h"
#include "usb_device.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "usb_device.h"
#include "usbd_cdc_if.h"
#include "bootloader.h"
#include "bootloader_user.h"
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
uint8_t BTNcounter = 0;
uint8_t keystate=0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

#ifdef __GNUC__
/* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
 set to 'Yes') calls __io_putchar() */
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */

/**
 * @brief Retargets the C library printf function to the USB.
 * @param None
 * @retval None
 */
extern  USBD_HandleTypeDef hUsbDeviceFS;

PUTCHAR_PROTOTYPE
{
	/* Place your implementation of fputc here */
	/* e.g. write a character to the USB VCP and Loop until the end of transmission */
	//USBD_CDC_SetTxBuffer(&hUsbDeviceFS, &ch, 1);
	if(hUsbDeviceFS.dev_state==USBD_STATE_CONFIGURED)
		while(!(CDC_Transmit_FS(&ch, 1)==USBD_OK));
	//CDC_Transmit_FS(&ch, 1);
	return ch;
}
void GPIO_DeInit(void)
{
	HAL_GPIO_DeInit(BTN_1_GPIO_Port, BTN_1_Pin);
	HAL_GPIO_DeInit(SEG_DIG1_GPIO_Port, SEG_DIG1_Pin);
	HAL_GPIO_DeInit(SEG_A_GPIO_Port, SEG_A_Pin);
	HAL_GPIO_DeInit(SEG_B_GPIO_Port, SEG_B_Pin);
	HAL_GPIO_DeInit(SEG_C_GPIO_Port, SEG_C_Pin);

	__HAL_RCC_GPIOE_CLK_DISABLE();
	__HAL_RCC_GPIOC_CLK_DISABLE();
	__HAL_RCC_GPIOH_CLK_DISABLE();
	__HAL_RCC_GPIOA_CLK_DISABLE();
	__HAL_RCC_GPIOB_CLK_DISABLE();
	__HAL_RCC_GPIOD_CLK_DISABLE();
}
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
	MX_DMA_Init();
	MX_SDIO_SD_Init();
	MX_FATFS_Init();
	MX_USB_DEVICE_Init();
	MX_RTC_Init();
	MX_CRC_Init();
	/* USER CODE BEGIN 2 */
	HAL_GPIO_WritePin(SEG_DIG1_GPIO_Port,SEG_DIG1_Pin,GPIO_PIN_RESET);

	LED_ALL_ON();
	HAL_Delay(5000);
	printf("Power up, Boot started.\n\r");
	HAL_Delay(500);
	LED_ALL_OFF();
	//  //////////////////////test /////////////////
	//  SD_DeInit();
	//  MX_DMA_DeInit();
	//  MX_USB_DEVICE_DeInit();
	//  HAL_RTC_MspDeInit(&hrtc);
	//  HAL_CRC_MspDeInit(&hcrc);
	//  GPIO_DeInit();
	//  uint32_t  JumpAddress = *(__IO uint32_t*)(APP_ADDRESS + 4);
	//  pFunction Jump        = (pFunction)JumpAddress;
	//
	//  HAL_RCC_DeInit();
	//  HAL_DeInit();
	//
	//  SysTick->CTRL = 0;
	//  SysTick->LOAD = 0;
	//  SysTick->VAL  = 0;
	//
	//#if(SET_VECTOR_TABLE)
	//  SCB->VTOR = APP_ADDRESS;
	//#endif
	//
	//  __set_MSP(*(__IO uint32_t*)APP_ADDRESS);
	//  Jump();
	//  while(1);
	//  /* Launch application */
	//  Bootloader_JumpToApplication();
	//  while(1);
	///////////////////////////////////////
	//  if(__HAL_RCC_GET_FLAG(RCC_FLAG_OBLRST))
	//  {
	//      print("OBL flag is active.");
	//#if(CLEAR_RESET_FLAGS)
	//      /* Clear system reset flags */
	//      __HAL_RCC_CLEAR_RESET_FLAGS();
	//      print("Reset flags cleared.");
	//#endif
	//  }

	/* USER CODE END 2 */

	/* Infinite loop */
	/* USER CODE BEGIN WHILE */
	HAL_FLASH_Unlock();

	uint32_t Address=AUTOPROG_ADDRESS;
	if((*(__IO uint16_t*)Address)==0x0001) //autoprogram
	{
		while(__HAL_FLASH_GET_FLAG(FLASH_FLAG_BSY) != 0);
		__HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_EOP |FLASH_FLAG_WRPERR |FLASH_FLAG_OPERR |FLASH_FLAG_PGAERR|FLASH_FLAG_PGPERR|FLASH_FLAG_PGSERR);;
		HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, Address, 0x0000);
		HAL_FLASH_Lock();
		HAL_Delay(100);

		printf("Entering Bootloader...\n\r");
		Enter_Bootloader(CONF_FILENAME);
	}
	else
	{
		while(IS_BTN_PRESSED() && BTNcounter < 90)
		{
			if(BTNcounter == 10)
			{
				printf("Release button to enter Bootloader.\n\r");
			}
			if(BTNcounter == 40)
			{
				printf("Release button to enter System Memory.\n\r");
			}

			if(BTNcounter < 10)
			{
				LED_ALL_ON();
			}
			else if(BTNcounter == 10)
			{
				LED_ALL_OFF();
			}
			else if(BTNcounter < 40)
			{
				LED_G_TG();
			}
			else if(BTNcounter == 40)
			{
				LED_G_OFF();
				LED_Y_ON();
			}
			else
			{
				LED_Y_TG();
			}

			BTNcounter++;
			HAL_Delay(100);
		}

		LED_ALL_OFF();
		/* Perform required actions based on button press duration */
		if(BTNcounter < 90)
		{
			if(BTNcounter > 40)
			{
				printf("Entering System Memory...\n\r");
				HAL_Delay(1000);
				Bootloader_JumpToSysMem();
			}
			else if(BTNcounter > 10)
			{
				printf("Entering Bootloader...\n\r");
				Enter_Bootloader(CONF_FILENAME);
			}
		}
	}

	/* Check if there is application in user flash area */
	if(Bootloader_CheckForApplication() == BL_OK)
	{
#if(USE_CHECKSUM)
		/* Verify application checksum */
		if(Bootloader_VerifyChecksum() != BL_OK)
		{
			print("Checksum Error.\n\r");
			Error_Handler();
		}
		else
		{
			print("Checksum OK.\n\r");
		}
#endif

		printf("Launching Application.\n\r");
		LED_G_ON();
		HAL_Delay(1000);
		LED_G_OFF();

		/* De-initialize bootloader hardware & peripherals */
		SD_DeInit();
		GPIO_DeInit();
		//      MX_DMA_DeInit();
		//      MX_USB_DEVICE_DeInit();
		//      HAL_RTC_MspDeInit(&hrtc);
		//      HAL_CRC_MspDeInit(&hcrc);

		/* Launch application */
		Bootloader_JumpToApplication();
	}

	/* No application found */
	printf("No application in flash.\n\r");
	while (1)
	{
		LED_R_ON();
		HAL_Delay(150);
		LED_R_OFF();
		HAL_Delay(150);
		LED_R_ON();
		HAL_Delay(150);
		LED_R_OFF();
		HAL_Delay(1050);
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
	RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};

	/** Configure the main internal regulator output voltage
	 */
	__HAL_RCC_PWR_CLK_ENABLE();
	__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
	/** Initializes the CPU, AHB and APB busses clocks
	 */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE|RCC_OSCILLATORTYPE_LSE;
	RCC_OscInitStruct.HSEState = RCC_HSE_ON;
	RCC_OscInitStruct.LSEState = RCC_LSE_ON;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
	RCC_OscInitStruct.PLL.PLLM = 25;
	RCC_OscInitStruct.PLL.PLLN = 144;
	RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
	RCC_OscInitStruct.PLL.PLLQ = 3;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
	{
		Error_Handler();
	}
	/** Initializes the CPU, AHB and APB busses clocks
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
	PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_RTC;
	PeriphClkInitStruct.RTCClockSelection = RCC_RTCCLKSOURCE_LSE;
	if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
	{
		Error_Handler();
	}
}

/* USER CODE BEGIN 4 */

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
