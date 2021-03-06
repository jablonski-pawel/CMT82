/*
 * screen9.c
 *
 *  Created on: 18.11.2016
 *      Author: gorian
 *      Ekran: otwarte drzwiczki
 */
#include "screen9.h"

extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;
extern uint8_t screen;
extern uint16_t size;
extern uint16_t position_x;
extern uint16_t position_y;
extern uint8_t action;

extern uint8_t data[50];

void screen9_init() {

	size = sprintf(data, "LOAD 0 0 9.bmp\n\r");
	HAL_UART_Transmit(&huart2, data, size, 100);

	size = sprintf(data, "Zaladowano ekran 9\n\r");
	HAL_UART_Transmit(&huart1, data, size, 1000);

	size = sprintf(data, "BUZ 10000 2000\n\r");
	HAL_UART_Transmit(&huart2, data, size, 100);

	screen = 9;
}

int screen9_button() {
	if (((0 < position_x) && (position_x < 45))
			&& ((0 < position_y) && (position_y < 45))) {
		size = sprintf(data, "BUZ 150 2000\n\r");
		HAL_UART_Transmit(&huart2, data, size, 100);
		return 1;
	}

	position_x = 0;
	position_y = 0;
	return 0;
	//0 - puste miejsce
	//1 - powrót do ekranu 2 (wyjście)

}

void screen9_action(int button) {

	switch (button) {

	case 1:
		screen = 2;
		action = 1;
		break;

	default:
		break;
	}
}
