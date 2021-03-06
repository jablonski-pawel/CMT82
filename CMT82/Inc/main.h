/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H__
#define __MAIN_H__

/* Includes ------------------------------------------------------------------*/

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private define ------------------------------------------------------------*/

#define ENA_Pin GPIO_PIN_13
#define ENA_GPIO_Port GPIOC
#define C2_Pin GPIO_PIN_4
#define C2_GPIO_Port GPIOA
#define C1_Pin GPIO_PIN_5
#define C1_GPIO_Port GPIOA
#define wire_sensor_Pin GPIO_PIN_6
#define wire_sensor_GPIO_Port GPIOA
#define wire_sensor_EXTI_IRQn EXTI9_5_IRQn
#define wire_LED_Pin GPIO_PIN_7
#define wire_LED_GPIO_Port GPIOA
#define door_Pin GPIO_PIN_0
#define door_GPIO_Port GPIOB
#define door_EXTI_IRQn EXTI0_IRQn
#define LED_Pin GPIO_PIN_1
#define LED_GPIO_Port GPIOB
#define knife_Pin GPIO_PIN_8
#define knife_GPIO_Port GPIOA
#define knife_EXTI_IRQn EXTI9_5_IRQn
#define USBDISC_Pin GPIO_PIN_15
#define USBDISC_GPIO_Port GPIOA
#define CP_R_Pin GPIO_PIN_4
#define CP_R_GPIO_Port GPIOB
#define CW_R_Pin GPIO_PIN_5
#define CW_R_GPIO_Port GPIOB
#define CP_C_Pin GPIO_PIN_6
#define CP_C_GPIO_Port GPIOB
#define CW_C_Pin GPIO_PIN_7
#define CW_C_GPIO_Port GPIOB
#define CW_L_Pin GPIO_PIN_8
#define CW_L_GPIO_Port GPIOB
#define CP_L_Pin GPIO_PIN_9
#define CP_L_GPIO_Port GPIOB

/* ########################## Assert Selection ############################## */
/**
  * @brief Uncomment the line below to expanse the "assert_param" macro in the 
  *        HAL drivers code
  */
/* #define USE_FULL_ASSERT    1U */

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
 extern "C" {
#endif
void _Error_Handler(char *, int);

#define Error_Handler() _Error_Handler(__FILE__, __LINE__)
#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H__ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
