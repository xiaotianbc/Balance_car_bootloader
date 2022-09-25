/**
  ******************************************************************************
  * @file    Project/STM32F4xx_StdPeriph_Templates/main.c 
  * @author  MCD Application Team
  * @version V1.8.1
  * @date    27-January-2022
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2016 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "printf.h"
#include "uart1.h"
#include "stm_flash.h"
/** @addtogroup Template_Project
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static __IO uint32_t uwTimingDelay;
RCC_ClocksTypeDef RCC_Clocks;

/* Private function prototypes -----------------------------------------------*/
static void Delay(__IO uint32_t nTime);

/* Private functions ---------------------------------------------------------*/

#include "lwrb.h"

lwrb_t lwrbuffer;
uint8_t *buffdata[128];

int main(void) {
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);    //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
    GPIO_InitTypeDef GPIO_InitStructure;
    lwrb_init(&lwrbuffer, buffdata, 128);
    lwrb_get_free(&lwrbuffer);
    /*!< At this stage the microcontroller clock setting is already configured,
          this is done through SystemInit() function which is called from startup
          files before to branch to application main.
          To reconfigure the default setting of SystemInit() function,
          refer to system_stm32f4xx.c file */

    /* SysTick end of count event each 10ms */
    RCC_GetClocksFreq(&RCC_Clocks);
    SysTick_Config(RCC_Clocks.HCLK_Frequency / 100);

    /* Add your application code here */
    /* Insert 50 ms delay */
    Delay(5);

    /* Output HSE clock on MCO1 pin(PA8) ****************************************/
    /* Enable the GPIOA peripheral */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
    uart1_init(9600);
    uint16_t update_flag = 0xde;
    printf_("update_flag_read test\n");
    //把update_flag写入到flash里
    mflash_write(FLASH_UPDATE_FLAGE_ADDR, &update_flag, 1);
    //再读出来
    uint16_t update_flag_readed = read_flash_16bit(FLASH_UPDATE_FLAGE_ADDR);
    printf_("update_flag_readed is 0x%X\n", update_flag_readed);
    while (1);
    extern int flash_test(void);
    flash_test();
    while (1) {
        //  printf("Hello, stm32F401_bootloader\n");
        Delay(1000);
    }


    while (1) {
    }
}

/**
  * @brief  Inserts a delay time.
  * @param  nTime: specifies the delay time length, in milliseconds.
  * @retval None
  */
void Delay(__IO uint32_t nTime) {
    uwTimingDelay = nTime / 10;
    if (uwTimingDelay < 1)
        uwTimingDelay++;
    while (uwTimingDelay != 0);
}

/**
  * @brief  Decrements the TimingDelay variable.
  * @param  None
  * @retval None
  */
void TimingDelay_Decrement(void) {
    if (uwTimingDelay != 0x00) {
        uwTimingDelay--;
    }
}

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/**
  * @}
  */


