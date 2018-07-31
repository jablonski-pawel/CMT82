/*
 * screen15.c
 *
 *  Created on: 18.11.2016
 *      Author: gorian
 *      Ekran: usuwanie programu - potwierdzenie
 */
#include "screen15.h"
#include "lista.h"

extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;
extern uint8_t screen;
extern uint16_t size;
extern uint16_t position_x;
extern uint16_t position_y;
extern uint8_t action;

extern uint8_t data[50];

//lista jednokierunkowa
extern wezel *L;
extern uint16_t p;

uint8_t node_to_delete = 0;

void screen15_init() {

	size = sprintf(data, "LOAD 0 0 15.bmp\n\r");
	HAL_UART_Transmit(&huart2, data, size, 100);

	size = sprintf(data, "Zaladowano ekran 15\n\r");
	HAL_UART_Transmit(&huart1, data, size, 1000);

	size = sprintf(data, "BUZ 500 2000\n\r");
	HAL_UART_Transmit(&huart2, data, size, 100);

	screen = 15;
}

int screen15_button() {
	if (((240 < position_x) && (position_x < 310))
			&& ((140 < position_y) && (position_y < 190))) {
		size = sprintf(data, "BUZ 150 2000\n\r");
		HAL_UART_Transmit(&huart2, data, size, 100);
		return 1;
	}

	if (((310 < position_x) && (position_x < 380))
			&& ((140 < position_y) && (position_y < 190))) {
		size = sprintf(data, "BUZ 150 2000\n\r");
		HAL_UART_Transmit(&huart2, data, size, 100);
		return 2;
	}

	position_x = 0;
	position_y = 0;
	return 0;
	//0 - puste miejsce
	//1 - nie - powrót do ekranu 3 (wyjście)
	//2 - tak - usuwanie
}

void screen15_action(int button) {

	switch (button) {

	case 1:
		screen = 2;
		action = 1;
		break;

	case 2:
		size = sprintf(data, "NODE VAL: %d\n\r", node_to_delete);
		HAL_UART_Transmit(&huart1, data, size, 100);
		if(node_to_delete>0){
			usun(&L, node_to_delete);
			screen = 2;
			action = 1;
		}
		break;

	default:
		break;
	}
}
