/**
  ******************************************************************************
  * File Name          : main.c
  * Description        : Main program body
  ******************************************************************************
  ** This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * COPYRIGHT(c) 2017 STMicroelectronics
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f4xx_hal.h"
#include "dma2d.h"
#include "i2c.h"
#include "ltdc.h"
#include "spi.h"
#include "gpio.h"
#include "fmc.h"

/* USER CODE BEGIN Includes */
#include "stm32f429i_discovery.h"
#include "stm32f429i_discovery_lcd.h"
#include "lcd_log.h"
#include "bme280.h"
/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/
  struct bme280_t sensor;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/
static void LCD_Config(void);
uint8_t* Hex2str(uint32_t*, uint32_t, uint8_t*);




/* USER CODE END PFP */

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration----------------------------------------------------------*/

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
 // MX_LTDC_Init();
  MX_SPI4_Init();
  MX_SPI5_Init();
  MX_FMC_Init();
  MX_I2C3_Init();
  MX_DMA2D_Init();

  /* USER CODE BEGIN 2 */
  LCD_Config();

  BSP_LCD_SetFont (&LCD_LOG_TEXT_FONT);
  uint8_t array[50]={0};
  uint8_t outbuf[50]={0};


  /* The variable used to assign the standby time*/
  	u8 v_stand_by_time_u8 = BME280_INIT_VALUE;
  	/* The variable used to read uncompensated temperature*/
  	s32 v_data_uncomp_temp_s32 = BME280_INIT_VALUE;
  	/* The variable used to read uncompensated pressure*/
  	s32 v_data_uncomp_pres_s32 = BME280_INIT_VALUE;
  	/* The variable used to read uncompensated pressure*/
  	s32 v_data_uncomp_hum_s32 = BME280_INIT_VALUE;
  	/* The variable used to read compensated temperature*/
  	s32 v_comp_temp_s32[2] = {BME280_INIT_VALUE, BME280_INIT_VALUE};
  	/* The variable used to read compensated pressure*/
  	u32 v_comp_press_u32[2] = {BME280_INIT_VALUE, BME280_INIT_VALUE};
  	/* The variable used to read compensated humidity*/
  	u32 v_comp_humidity_u32[2] = {BME280_INIT_VALUE, BME280_INIT_VALUE};



  BME280_RETURN_FUNCTION_TYPE ret_rslt = ERROR_BME280;

/*
  sensor.bus_write = BME280_I2C_bus_write;
  sensor.bus_read = BME280_I2C_bus_read;
  sensor.dev_addr = BME280_I2C_ADDRESS1;
  sensor.delay_msec = BME280_delay_msek;
*/
  ret_rslt = I2C_routine(&sensor);
  ret_rslt += bme280_init(&sensor);
  /*
  uint32_t id=sensor.chip_id;
  Hex2str((&id), 1, outbuf);
  BSP_LCD_DisplayStringAt(0, 80, outbuf, LEFT_MODE);
  *outbuf='/0';
*/
  ret_rslt += bme280_set_power_mode(BME280_NORMAL_MODE);
  ret_rslt += bme280_set_oversamp_humidity(BME280_OVERSAMP_1X);
  ret_rslt += bme280_set_oversamp_pressure(BME280_OVERSAMP_1X);
  ret_rslt += bme280_set_oversamp_temperature(BME280_OVERSAMP_1X);

  ret_rslt += bme280_set_standby_durn(BME280_STANDBY_TIME_1_MS);
  ret_rslt += bme280_get_standby_durn(&v_stand_by_time_u8);






/*
  HAL_I2C_Mem_Read(&hi2c3, (uint16_t) 0xec, (uint16_t) BME280_HUMIDITY_MSB_REG, I2C_MEMADD_SIZE_8BIT, &array[0], 1, 100) ;
  HAL_I2C_Mem_Read(&hi2c3, (uint16_t) 0xec, (uint16_t) BME280_HUMIDITY_LSB_REG, I2C_MEMADD_SIZE_8BIT, &array[1], 1, 100) ;
 // BME280_I2C_bus_read(0xec,0xd0,array,1);
  Hex2str((uint8_t*)&array[0], 1, outbuf);
   BSP_LCD_DisplayStringAt(0, 0, outbuf, LEFT_MODE);
   Hex2str((uint8_t*)&array[1], 1, outbuf);
    BSP_LCD_DisplayStringAt(0, 20, outbuf, LEFT_MODE);


*/
  if (!ret_rslt) //all ok
	  BSP_LED_On(LED3);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
  /* USER CODE END WHILE */

  /* USER CODE BEGIN 3 */
	  ret_rslt += bme280_read_uncomp_temperature(&v_data_uncomp_temp_s32);
	     ret_rslt += bme280_read_uncomp_pressure(&v_data_uncomp_pres_s32);
	    	ret_rslt += bme280_read_uncomp_humidity(&v_data_uncomp_hum_s32);
	   	ret_rslt += bme280_read_uncomp_pressure_temperature_humidity(
	    	&v_data_uncomp_temp_s32, &v_data_uncomp_pres_s32, &v_data_uncomp_hum_s32);


	   	v_comp_temp_s32[0] = bme280_compensate_temperature_int32(
	   				v_data_uncomp_temp_s32);
	   		v_comp_press_u32[0] = bme280_compensate_pressure_int32(
	   				v_data_uncomp_pres_s32);
	   		v_comp_humidity_u32[0] = bme280_compensate_humidity_int32(
	   				v_data_uncomp_hum_s32);
	   		ret_rslt += bme280_read_pressure_temperature_humidity(
	   		&v_comp_press_u32[1], &v_comp_temp_s32[1],  &v_comp_humidity_u32[1]);


	   	LCD_LOG_ClearTextZone();
	   	/*
	    Hex2str((uint32_t*)&v_comp_temp_s32[1], 1, outbuf);
	    BSP_LCD_DisplayStringAt(0, 0, outbuf, LEFT_MODE);

	    Hex2str((uint32_t*)&v_comp_press_u32[1], 1, outbuf);
	     BSP_LCD_DisplayStringAt(0, 20, outbuf, LEFT_MODE);

	     Hex2str((uint32_t*)&v_comp_humidity_u32[1], 1, outbuf);
	      BSP_LCD_DisplayStringAt(0, 40, outbuf, LEFT_MODE);
*/
	    sprintf(outbuf,"t=%+ld C",v_comp_temp_s32[1]);
        BSP_LCD_DisplayStringAt(0, 0, outbuf, LEFT_MODE);
	    sprintf(outbuf,"p=%-.3ld mm Hg",v_comp_press_u32[1]*75/1000);
        BSP_LCD_DisplayStringAt(0, 20, outbuf, LEFT_MODE);
	    sprintf(outbuf,"h=%ld %%",v_comp_humidity_u32[1]);
        BSP_LCD_DisplayStringAt(0, 40, outbuf, LEFT_MODE);

	      HAL_Delay(1000);
  }
  /* USER CODE END 3 */

}

/** System Clock Configuration
*/
void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct;

    /**Configure the main internal regulator output voltage 
    */
  __HAL_RCC_PWR_CLK_ENABLE();

  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 180;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Activate the Over-Drive mode 
    */
  if (HAL_PWREx_EnableOverDrive() != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_LTDC;
  PeriphClkInitStruct.PLLSAI.PLLSAIN = 60;
  PeriphClkInitStruct.PLLSAI.PLLSAIR = 5;
  PeriphClkInitStruct.PLLSAIDivR = RCC_PLLSAIDIVR_4;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Configure the Systick interrupt time 
    */
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

    /**Configure the Systick 
    */
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

/* USER CODE BEGIN 4 */
static void LCD_Config(void)
{
  /* LCD Initialization */
  BSP_LCD_Init();

  /* LCD Layers Initialization */
  BSP_LCD_LayerDefaultInit(LCD_FOREGROUND_LAYER, (LCD_FRAME_BUFFER + BUFFER_OFFSET));

  /* Configure the transparency for foreground : Increase the transparency */
  BSP_LCD_SetTransparency(LCD_BACKGROUND_LAYER, 0);
  BSP_LCD_SelectLayer(LCD_FOREGROUND_LAYER);

  /* LCD Log initialization */

  LCD_LOG_Init();

 // LCD_LOG_SetHeader((uint8_t *)"meteo Application");


  LCD_LOG_SetFooter ((uint8_t *)"     Meteo Application" );

}

//____________________________________________________________________________/
uint8_t* Hex2str(uint32_t* inarr, uint32_t size, uint8_t* out)
{
	uint8_t symbol_count = 2*sizeof(*inarr);

	for(uint32_t i=0; i < size; i++)
	{
		for(int8_t j = symbol_count-1, k = 0; j >= 0 ; j--, k++)
		{
			uint32_t temp = 0;
			temp = *(inarr+i) >> (4*j);
			temp &= 0xf;
			if(temp < 0xa)
			{
				*(out+i*symbol_count+k)= '0' + temp; 	  //numbers
			}
			else
			{
				*(out+i*symbol_count+k)= 'a' + (temp - 0xa); //letters
			}

		}
	}
	out[size*symbol_count]='\0';
	return out;
}





/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
void _Error_Handler(char * file, int line)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
	BSP_LED_On(LED3);
	BSP_LED_On(LED4);
  while(1) 
  {
  }
  /* USER CODE END Error_Handler_Debug */ 
}

#ifdef USE_FULL_ASSERT

/**
   * @brief Reports the name of the source file and the source line number
   * where the assert_param error has occurred.
   * @param file: pointer to the source file name
   * @param line: assert_param error line source number
   * @retval None
   */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
    ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */

}

#endif

/**
  * @}
  */ 

/**
  * @}
*/ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
