/**
 ******************************************************************************
 * File Name          : main.c
 * Description        : Main program body
 ******************************************************************************
 *
 * COPYRIGHT(c) 2016 STMicroelectronics
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
#include "stm32f1xx_hal.h"

/* USER CODE BEGIN Includes */
#include "init.h"
#include "lista.h"
#include "process.h"
#include "screen1.h"
#include "screen3.h"
#include "screen4.h"
#include "screen6.h"
#include "screen7.h"
#include "screen9.h"
#include "screen10.h"
#include "screen11.h"
#include "screen12.h"
#include "screen13.h"
/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/
RTC_HandleTypeDef hrtc;

TIM_HandleTypeDef htim1;
TIM_HandleTypeDef htim3;
TIM_HandleTypeDef htim4;

UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;

PCD_HandleTypeDef hpcd_USB_FS;

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/
uint8_t data[50]; // Tablica przechowujaca wysylana wiadomosc.
uint16_t size = 0; // Rozmiar wysylanej wiadomosci ++cnt; // Zwiekszenie licznika wyslanych wiadomosci.
uint8_t screen = 0; //numer wyświetlanego ekranu (zgodnie z nazwą pliku na karcie pamięci)
uint8_t action = 0; //flaga, czy została podjęta akcja do wykonania

//zmienne dla odbioru danych po uart1
uint8_t Rx_UART1;	//zmienna do przechowywania znaku odebranego przez UART1

//zmienne dla odbioru danych po uart2
char Rx_indx, Rx_data[2], Rx_Buffer[4], Rx_Buffer_X[4], Rx_Buffer_Y[4],
		Transfer_cplt;
uint8_t Rx_Space = 0;
uint16_t position_x, position_y;
uint8_t Rx_active_count = 0; //bufor liczby cykli timer'a; po przekroczeniu wartości w przerwaniu timera zostanie z powrotem uruchomiony nasłuch

__IO ITStatus UartReady = RESET; //status UART'a

//struktury dla godziny i daty
RTC_DateTypeDef sdatestructureget;
RTC_TimeTypeDef stimestructureget;

//zmienne timera dla klawiatury - litery
uint8_t keyboard_timer = 0;
char last_char = '~';

//struktura i zmienne dla listy z programami
wezel *L = NULL;
uint16_t p = 3;
uint16_t p_max = 0;

uint8_t start = 0; //start procesu
uint8_t base = 0; //bazowanie noży - 1 - zbazowane; 0 - nie
uint8_t cut = 0; //cięcie - 1 - tnie; 0 - nie
extern uint16_t impuls2open;
uint16_t cutting_deph = 0;
uint8_t set_cutting = 0;
extern uint16_t cutting_impulses;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void Error_Handler(void);
static void MX_GPIO_Init(void);
static void MX_RTC_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_USB_PCD_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_TIM1_Init(void);
static void MX_TIM3_Init(void);
static void MX_TIM4_Init(void);

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
	if (GPIO_Pin == door_Pin) {
		if(HAL_GPIO_ReadPin(door_GPIO_Port, door_Pin) < 1 && start == 1){
			start = 0;
			screen9_init();
		}
		size = sprintf((char*) data, "Drzwi otwarte !!! %d !!!\n\r",
				impuls2open);
		HAL_UART_Transmit(&huart1, data, size, 100);
	}
	if (GPIO_Pin == wire_sensor_Pin) {
		if(HAL_GPIO_ReadPin(wire_sensor_GPIO_Port, wire_sensor_Pin) < 1 && start == 1){
			start = 0;
			screen7_init();
		}
		size = sprintf((char*) data, "Brak przewodu\n\r");
		HAL_UART_Transmit(&huart1, data, size, 100);
	}
	if (GPIO_Pin == knife_Pin) {
		size = sprintf((char*) data, "Noz interrupt\n\r");
		HAL_UART_Transmit(&huart1, data, size, 100);
	}
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart) {
	//HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_SET);
	UartReady = SET;

}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
	uint8_t i;

	if(huart->Instance == USART1){
		switch (atoi(&Rx_UART1)) {

				case 0: // Jezeli odebrany zostanie znak 0
					//size = sprintf(Data, "STOP\n\r");
					HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_RESET);
					HAL_GPIO_WritePin(wire_LED_GPIO_Port, wire_LED_Pin, GPIO_PIN_RESET);
					break;

				case 1: // Jezeli odebrany zostanie znak 1
					HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_SET);
					HAL_GPIO_WritePin(wire_LED_GPIO_Port, wire_LED_Pin, GPIO_PIN_SET);
					break;

				case 2: //uruchomienie procesu cięcia na długość i odizolowywania
					start = 1;
					//process_run();
					break;

				case 3: //zatrzymanie procesu cięcia
					start = 0;
					break;

				case 4: //bazowanie
					base = 0;
					break;

				case 5: //test cewki
					HAL_GPIO_TogglePin(C2_GPIO_Port, C2_Pin);
					break;

				case 6:
					HAL_GPIO_TogglePin(C1_GPIO_Port, C1_Pin);
					break;


				default: // Jezeli odebrano nieobslugiwany znak
					//size = sprintf(Data, "Odebrano nieznany znak: %c\n\r", Received);
					break;
				}
	}

	if (huart->Instance == USART2) {
		if (Rx_indx == 0) {
			for (i = 0; i < 4; i++)
				Rx_Buffer[i] = 0;
		}

		if (Rx_data[0] != 10) {
			if (Rx_data[0] != 84 && Rx_data[0] != 88 && Rx_data[0] != 89) {
				if (Rx_data[0] != 32) {
					if (Rx_Space == 1) {
						Rx_Buffer[Rx_indx++] = Rx_data[0];
						strcpy(Rx_Buffer_X, Rx_Buffer);
					}

					if (Rx_Space == 2) {
						Rx_Buffer[Rx_indx++] = Rx_data[0];
						strcpy(Rx_Buffer_Y, Rx_Buffer);
					}
				} else {
					Rx_Space++;
					Rx_indx = 0;
				}
			}
		} else {
			Rx_Space = 0;
			Rx_indx = 0;
			Transfer_cplt = 1;
		}

	}

	//UartReady = SET;
	if (Transfer_cplt == 1) {
		position_x = atoi((char*) &Rx_Buffer_X);
		position_y = atoi((char*) &Rx_Buffer_Y);
		//size = sprintf(data, "X:%d,Y:%dN\n\r", position_x, position_y);
		//HAL_UART_Transmit_IT(&huart1, data, size);

		switch (screen) {
		case 2:
			screen1_action(screen1_button());
			break;

		case 3:
			screen3_action(screen3_button());
			break;

		case 4:
			screen4_action(screen4_button());
			break;

		case 6:
			//brak akcji do wykonania, nie ma takiej funkcji, bo nie ma przycisków
			break;

		case 7:
			screen7_action(screen7_button());
			break;

		case 9:
			screen9_action(screen9_button());
			break;

		case 10:
			screen10_action(screen10_button());
			break;

		case 11:
			screen11_action(screen11_button());
			break;

		case 12:
			screen12_action(screen12_button());
			break;

		case 13:
			screen13_action(screen13_button());
			break;

		default:
			break;
		}
		Transfer_cplt = 0;
	}

	if (action == 1) {

		switch (screen) {
		case 2:
			HAL_RTC_GetTime(&hrtc, &stimestructureget, FORMAT_BIN);
			screen1_init(stimestructureget.Hours, stimestructureget.Minutes);
			//action = 0;
			break;

		case 3:
			screen3_init();
			//action = 0;
			break;

		case 4:
			screen4_init();
			//action = 0;
			break;

		case 6:
			HAL_RTC_GetTime(&hrtc, &stimestructureget, FORMAT_BIN);
			screen6_init(stimestructureget.Hours, stimestructureget.Minutes);
			break;

		case 7:
			screen7_init();
			//action = 0;
			break;

		case 9:
			screen9_init();
			//action = 0;
			break;

		case 10:
			screen10_init();
			//action = 0;
			break;

		case 11:
			screen11_init();
			//action = 0;
			break;

		case 12:
			screen12_init();
			//action = 0;
			break;

		case 13:
			screen13_init();
			//action = 0;
			break;

		default:
			//action = 0;
			break;
		}

		action = 0;
	}

	HAL_UART_Receive_IT(&huart1, &Rx_UART1, 1); // Ponowne włączenie nasłuchiwania na UART1 w przerwaniu
	HAL_UART_Receive_IT(&huart2, &Rx_data, 1); // Ponowne włączenie nasłuchiwania na UART2 w przerwaniu
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
	if (htim->Instance == TIM1) { // Jeżeli przerwanie pochodzi od timera 1
		//pobiera godzinę
		HAL_RTC_GetTime(&hrtc, &stimestructureget, FORMAT_BIN);

		//jeśli minuta się zmieniła to zmienia godzinę wyświetlaną
		if (stimestructureget.Seconds == 0) {
			if (screen == 2) {
				size = sprintf(data, "CLR 400 1 479 45 19481\n\r");
				HAL_UART_Transmit(&huart2, data, size, 100);
				size = sprintf(data, "UF 5 405 10 65535 %0.2d:%0.2d\n\r",
						stimestructureget.Hours, stimestructureget.Minutes);
				HAL_UART_Transmit(&huart2, data, size, 100);
			}

			if (screen == 6) {
				size = sprintf(data, "CLR 400 1 479 45 405441\n\r");
				HAL_UART_Transmit(&huart2, data, size, 100);
				size = sprintf(data, "UF 5 405 10 65535 %0.2d:%0.2d\n\r",
						stimestructureget.Hours, stimestructureget.Minutes);
				HAL_UART_Transmit(&huart2, data, size, 100);
			}

			if (screen == 10) {

				size = sprintf(data, "CLR 45 84 90 124 65535\n\r");
				HAL_UART_Transmit(&huart2, data, size, 100);

				size = sprintf(data, "UF 5 55 94 0 %0.2d\n\r",
						stimestructureget.Hours);
				HAL_UART_Transmit(&huart2, data, size, 100);

				size = sprintf(data, "CLR 100 84 150 124 65535\n\r");
				HAL_UART_Transmit(&huart2, data, size, 100);

				size = sprintf(data, "UF 5 112 94 0 %0.2d\n\r",
						stimestructureget.Minutes);
				HAL_UART_Transmit(&huart2, data, size, 100);

			}

			/* Display time Format : hh:mm:ss */
			size = sprintf((char*) data, "%0.2d:%0.2d:%0.2d\n\r",
					stimestructureget.Hours, stimestructureget.Minutes,
					stimestructureget.Seconds);
			HAL_UART_Transmit(&huart1, data, size, 100);

			/* Get the RTC current Date */
			//HAL_RTC_GetDate(&hrtc, &sdatestructureget, FORMAT_BIN);
			/* Display date Format : mm-dd-yy */
			//size = sprintf((char*) data, "%0.2d-%0.2d-%0.2d\n\r",
			//		sdatestructureget.Month, sdatestructureget.Date,
			//		2000 + sdatestructureget.Year);
			//HAL_UART_Transmit_IT(&huart1, data, size);
		}

		//HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);

		if (keyboard_timer > 0) {
			keyboard_timer++;

			if (keyboard_timer == 3) {
				last_char = '~';
			}

		}
	}

	if (htim->Instance == TIM4) { // Jeżeli przerwanie pochodzi od timera 4
		if (base == 0) {
			bazowanie();
		}

		if ((cut > 0) && (start > 0)) {
			process_cut();
		}

		if ((set_cutting > 0) && (cutting_deph < 25)) {
			HAL_GPIO_TogglePin(CP_C_GPIO_Port, CP_C_Pin);
			++cutting_deph;
		} else {
			if(set_cutting == 1) cutting_impulses += 25;
			if(set_cutting == 2) cutting_impulses -= 25;
			if(set_cutting > 0){
				size = sprintf(data, "Ilosc impulsow noza: %d\n\r\n\r", cutting_impulses);
				HAL_UART_Transmit(&huart1, data, size, 100);
			}
			set_cutting = 0;
			cutting_deph = 0;
		}

	}

	if (htim->Instance == TIM3) { // Jeżeli przerwanie pochodzi od timera 3
		if (start > 0) {
			process_run();
		}
	}
}

/* USER CODE END PFP */

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

int main(void) {

	/* USER CODE BEGIN 1 */

	/* USER CODE END 1 */

	/* MCU Configuration----------------------------------------------------------*/

	/* Reset of all peripherals, Initializes the Flash interface and the Systick. */
	HAL_Init();

	/* Configure the system clock */
	SystemClock_Config();

	/* Initialize all configured peripherals */
	MX_GPIO_Init();
	MX_RTC_Init();
	MX_USART2_UART_Init();
	MX_USB_PCD_Init();
	MX_USART1_UART_Init();
	MX_TIM1_Init();
	MX_TIM3_Init();
	MX_TIM4_Init();

	/* USER CODE BEGIN 2 */
	HAL_UART_Receive_IT(&huart1, &Rx_UART1, 1); //start nasłuchu dla UART1 co 1 znak w przerwaniu
	HAL_UART_Receive_IT(&huart2, &Rx_data, 1); //start odbioru dla UART2 co 1 znak w przerwaniu
	HAL_TIM_Base_Init(&htim1);
	HAL_TIM_Base_Start_IT(&htim1); //start timera generującego przerwanie co sekundę
	HAL_TIM_Base_Init(&htim3);
	HAL_TIM_Base_Start_IT(&htim3); //start timera dla silników
	HAL_TIM_Base_Start_IT(&htim1); //start timera generującego przerwanie co sekundę
	HAL_TIM_Base_Init(&htim4);
	HAL_TIM_Base_Start_IT(&htim4); //start timera dla silnika noża
	HAL_RTC_GetTime(&hrtc, &stimestructureget, FORMAT_BIN); // pobranie godziny

	//tworzenie listy jednokierunkowej. TODO: WSTAWIĆ TUTAJ EEPROM
	wstawPocz(&L, "aaa", 999, 0, 999, 1, 2, 3, 4, 800, 250);
	//wstawPocz(&L, 33);
	wstaw(&L, 1, "bbb", 1000, 0, 200, 7, 6, 7, 8, 810, 250);
	wstaw(&L, 2, "ccc", 2000, 0, 300, 8, 7, 8, 9, 820, 250);
	wstaw(&L, 3, "ddd", 300, 0, 900, 9, 8, 1, 2, 825, 50);
	wstaw(&L, 4, "eee", 4000, 0, 500, 4, 3, 4, 5, 840, 250);

//ekran startowy - logo+pasek ładowania
	wlaczanie(&huart1, &huart2);

//pierwszy ekran - główny programu
	screen1_init(stimestructureget.Hours, stimestructureget.Minutes);

	/* USER CODE END 2 */

	/* Infinite loop */
	/* USER CODE BEGIN WHILE */
	while (1) {
		/* USER CODE END WHILE */

		/* USER CODE BEGIN 3 */
//		HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
//		HAL_GPIO_TogglePin(wire_LED_GPIO_Port, wire_LED_Pin);
//		HAL_Delay(500);
		//size = sprintf((char*) data, "Noz: %d \n\r", HAL_GPIO_ReadPin(knife_GPIO_Port, knife_Pin));
		//HAL_UART_Transmit(&huart1, data, size, 100);
	}
	/* USER CODE END 3 */

}

/** System Clock Configuration
 */
void SystemClock_Config(void) {

	RCC_OscInitTypeDef RCC_OscInitStruct;
	RCC_ClkInitTypeDef RCC_ClkInitStruct;
	RCC_PeriphCLKInitTypeDef PeriphClkInit;

	/**Initializes the CPU, AHB and APB busses clocks
	 */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSI
			| RCC_OSCILLATORTYPE_HSE;
	RCC_OscInitStruct.HSEState = RCC_HSE_ON;
	RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
	RCC_OscInitStruct.HSIState = RCC_HSI_ON;
	RCC_OscInitStruct.LSIState = RCC_LSI_ON;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
	RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
		Error_Handler();
	}

	/**Initializes the CPU, AHB and APB busses clocks
	 */
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
			| RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK) {
		Error_Handler();
	}

	PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_RTC | RCC_PERIPHCLK_USB;
	PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_LSI;
	PeriphClkInit.UsbClockSelection = RCC_USBCLKSOURCE_PLL_DIV1_5;
	if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK) {
		Error_Handler();
	}

	/**Configure the Systick interrupt time
	 */
	HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq() / 1000);

	/**Configure the Systick
	 */
	HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

	/* SysTick_IRQn interrupt configuration */
	HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

/* RTC init function */
static void MX_RTC_Init(void) {

	RTC_TimeTypeDef sTime;
	RTC_DateTypeDef DateToUpdate;

	/**Initialize RTC Only
	 */
	hrtc.Instance = RTC;
	hrtc.Init.AsynchPrediv = RTC_AUTO_1_SECOND;
	hrtc.Init.OutPut = RTC_OUTPUTSOURCE_ALARM;
	if (HAL_RTC_Init(&hrtc) != HAL_OK) {
		Error_Handler();
	}

	/**Initialize RTC and set the Time and Date
	 */
	sTime.Hours = 0x1;
	sTime.Minutes = 0x0;
	sTime.Seconds = 0x0;

	if (HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BCD) != HAL_OK) {
		Error_Handler();
	}

	DateToUpdate.WeekDay = RTC_WEEKDAY_TUESDAY;
	DateToUpdate.Month = RTC_MONTH_NOVEMBER;
	DateToUpdate.Date = 0x15;
	DateToUpdate.Year = 0x16;

	if (HAL_RTC_SetDate(&hrtc, &DateToUpdate, RTC_FORMAT_BCD) != HAL_OK) {
		Error_Handler();
	}

}

/* TIM1 init function */
static void MX_TIM1_Init(void) {

	htim1.Instance = TIM1;
	htim1.Init.Prescaler = 9999;
	htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim1.Init.Period = 3599;
	htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htim1.Init.RepetitionCounter = 0;
	if (HAL_TIM_OnePulse_Init(&htim1, TIM_OPMODE_REPETITIVE) != HAL_OK) {
		Error_Handler();
	}

}

/* TIM3 init function */
static void MX_TIM3_Init(void) {

	htim3.Instance = TIM3;
	htim3.Init.Prescaler = 999;
	htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim3.Init.Period = 17;
	htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	if (HAL_TIM_OnePulse_Init(&htim3, TIM_OPMODE_REPETITIVE) != HAL_OK) {
		Error_Handler();
	}

}

/* TIM4 init function */
static void MX_TIM4_Init(void) {

	htim4.Instance = TIM4;
	htim4.Init.Prescaler = 999;
	htim4.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim4.Init.Period = 143;
	htim4.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	if (HAL_TIM_OnePulse_Init(&htim4, TIM_OPMODE_REPETITIVE) != HAL_OK) {
		Error_Handler();
	}

}

/* USART1 init function */
static void MX_USART1_UART_Init(void) {

	huart1.Instance = USART1;
	huart1.Init.BaudRate = 9600;
	huart1.Init.WordLength = UART_WORDLENGTH_8B;
	huart1.Init.StopBits = UART_STOPBITS_1;
	huart1.Init.Parity = UART_PARITY_NONE;
	huart1.Init.Mode = UART_MODE_TX_RX;
	huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart1.Init.OverSampling = UART_OVERSAMPLING_16;
	if (HAL_UART_Init(&huart1) != HAL_OK) {
		Error_Handler();
	}

}

/* USART2 init function */
static void MX_USART2_UART_Init(void) {

	huart2.Instance = USART2;
	huart2.Init.BaudRate = 9600;
	huart2.Init.WordLength = UART_WORDLENGTH_8B;
	huart2.Init.StopBits = UART_STOPBITS_1;
	huart2.Init.Parity = UART_PARITY_NONE;
	huart2.Init.Mode = UART_MODE_TX_RX;
	huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart2.Init.OverSampling = UART_OVERSAMPLING_16;
	if (HAL_UART_Init(&huart2) != HAL_OK) {
		Error_Handler();
	}

}

/* USB init function */
static void MX_USB_PCD_Init(void) {

	hpcd_USB_FS.Instance = USB;
	hpcd_USB_FS.Init.dev_endpoints = 8;
	hpcd_USB_FS.Init.speed = PCD_SPEED_FULL;
	hpcd_USB_FS.Init.ep0_mps = DEP0CTL_MPS_8;
	hpcd_USB_FS.Init.low_power_enable = DISABLE;
	hpcd_USB_FS.Init.lpm_enable = DISABLE;
	hpcd_USB_FS.Init.battery_charging_enable = DISABLE;
	if (HAL_PCD_Init(&hpcd_USB_FS) != HAL_OK) {
		Error_Handler();
	}

}

/** Configure pins as 
 * Analog
 * Input
 * Output
 * EVENT_OUT
 * EXTI
 */
static void MX_GPIO_Init(void) {

	GPIO_InitTypeDef GPIO_InitStruct;

	/* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOC_CLK_ENABLE()
	;
	__HAL_RCC_GPIOD_CLK_ENABLE()
	;
	__HAL_RCC_GPIOA_CLK_ENABLE()
	;
	__HAL_RCC_GPIOB_CLK_ENABLE()
	;

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(ENA_GPIO_Port, ENA_Pin, GPIO_PIN_RESET);

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOA, C2_Pin | C1_Pin | wire_LED_Pin | USBDISC_Pin,
			GPIO_PIN_RESET);

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOB,
	LED_Pin | CP_R_Pin | CW_R_Pin | CP_C_Pin | CW_C_Pin | CW_L_Pin | CP_L_Pin,
			GPIO_PIN_RESET);

	/*Configure GPIO pin : ENA_Pin */
	GPIO_InitStruct.Pin = ENA_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(ENA_GPIO_Port, &GPIO_InitStruct);

	/*Configure GPIO pins : C2_Pin C1_Pin wire_LED_Pin USBDISC_Pin */
	GPIO_InitStruct.Pin = C2_Pin | C1_Pin | wire_LED_Pin | USBDISC_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	/*Configure GPIO pins : wire_sensor_Pin knife_Pin */
	GPIO_InitStruct.Pin = wire_sensor_Pin | knife_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	/*Configure GPIO pin : door_Pin */
	GPIO_InitStruct.Pin = door_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(door_GPIO_Port, &GPIO_InitStruct);

	/*Configure GPIO pins : LED_Pin CP_R_Pin CW_R_Pin CP_C_Pin
	 CW_C_Pin CW_L_Pin CP_L_Pin */
	GPIO_InitStruct.Pin = LED_Pin | CP_R_Pin | CW_R_Pin | CP_C_Pin | CW_C_Pin
			| CW_L_Pin | CP_L_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	/* EXTI interrupt init*/
	HAL_NVIC_SetPriority(EXTI0_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(EXTI0_IRQn);

	HAL_NVIC_SetPriority(EXTI9_5_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
 * @brief  This function is executed in case of error occurrence.
 * @param  None
 * @retval None
 */
void Error_Handler(void) {
	/* USER CODE BEGIN Error_Handler */
	/* User can add his own implementation to report the HAL error return state */
	while (1) {
	}
	/* USER CODE END Error_Handler */
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
