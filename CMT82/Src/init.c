/*
 * init.c
 *
 *  Created on: 16.11.2016
 *      Author: gorian
 */
#include "init.h"
#include "main.h"
#include "stm32f1xx_hal.h"

uint8_t step_base = 0; // numer kroku bazowania
uint16_t impuls2close = 0;
uint16_t impuls2open = 0;

extern uint8_t base;
extern uint8_t start;
extern TIM_HandleTypeDef htim4;

void wlaczanie( huart1, huart2) {
	int size = 0;
	int progress = 100;
	uint8_t data[50];

	size = sprintf(data, "Start programu\n\r");
	HAL_UART_Transmit_IT(huart1, data, size);

	HAL_Delay(2500); //oczekiwanie aż wyswietlacz się włączy i będzie gotowy do pracy

	// Wyświetlanie logo
	size = sprintf(data, "LOAD 0 0 1.bmp\n\r");
	HAL_UART_Transmit_IT(huart2, data, size);
	HAL_Delay(1000);

	//pasek postępu
	for (int i = 0; i <= 13; i++) {
		progress += 20;
		size = sprintf(data, "CLR 100 215 %d 225 61958\n\r", progress);
		HAL_UART_Transmit_IT(huart2, data, size);
		HAL_Delay(50);
	}
	HAL_Delay(300);
}

int * XY_position(char *str[]) {
	static uint8_t position[2];
	uint16_t size = 0;
	uint8_t data[50];
	char *test0;
	char *test = "123";
	//if(str[0]==84 && str[1]==88 && str[2]==89 && str[3]==32)
	//{
	//if(str[4]!=2){
	//strncpy(test0, test, 2);
	//position[0]=atoi(test0);
	//}

	//}
	return position;
}

void bazowanie() {

	switch (step_base) {
	case 0:
		//size = sprintf(data, "Zamykanie noży\n\r");
		//HAL_UART_Transmit_IT(huart1, data, size);

		//kierunek zamykania
		HAL_GPIO_WritePin(CW_C_GPIO_Port, CW_C_Pin, GPIO_PIN_RESET);
		//włączenie silników
		HAL_GPIO_WritePin(ENA_GPIO_Port, ENA_Pin, GPIO_PIN_RESET);
		impuls2close = 0;
		++step_base;
		break;

	case 1:
		//zamykanie noży (ucięcie ew. przewodu)
		if (impuls2close < 950) {
			HAL_GPIO_TogglePin(CP_C_GPIO_Port, CP_C_Pin);
			++impuls2close;
		} else {
			//kierunek otwierania
			HAL_GPIO_WritePin(CW_C_GPIO_Port, CW_C_Pin, GPIO_PIN_SET);
			impuls2open = 0;
			++step_base;
		}
		break;

	case 2:
		//otwieranie noży dopóki nie zadziała czujnik
		if (HAL_GPIO_ReadPin(knife_GPIO_Port, knife_Pin) < 1) {
			HAL_GPIO_TogglePin(CP_C_GPIO_Port, CP_C_Pin);
			++impuls2open;
		} else {
			++step_base;
		}
		break;

	case 3:
		//wyłączenie silników
		HAL_GPIO_WritePin(ENA_GPIO_Port, ENA_Pin, GPIO_PIN_RESET);
		//zmiana prędkości zamykania noży
		htim4.Init.Period = 11;
		if (HAL_TIM_OnePulse_Init(&htim4, TIM_OPMODE_REPETITIVE) != HAL_OK) {
			Error_Handler();
		}

		start = 0;
		base = 1;
		step_base = 0;
		break;

	default:
		start = 0;
		base = 1;
		break;
	}
}
