/*
 * screen11.c
 *
 *  Created on: 25.01.2017
 *      Author: gorian
 *      Ekran: Ustawienia noży
 */
#include "screen4.h"
#include "lista.h"
#include <stdlib.h>

extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;
extern TIM_HandleTypeDef htim4;
extern uint8_t screen;
extern uint16_t size;
extern uint16_t position_x;
extern uint16_t position_y;
extern uint8_t action;

extern uint8_t data[50];
extern uint8_t base;

extern char *temp_str[1];

extern uint8_t param_number;
extern uint8_t max_size1;
uint8_t dot_char;

char temp_name[18];
char *temp_pcs[5];
char *temp_pcs_done[5];
char *temp_length[4];
char *temp_left_cov[2];
char *temp_left_eye[2];
char *temp_right_eye[2];
char *temp_right_cov[2];
char *temp_knife[5];
char *temp_knife_move_back[5];

char *temp_knife_mm[4];
char *temp_knife_move_back_mm[4];

char _name[18];
uint16_t _pcs;
uint16_t _pcs_done;
uint16_t _length;
uint8_t _left_cov;
uint8_t _left_eye;
uint8_t _right_eye;
uint8_t _right_cov;
uint16_t _knife;
uint16_t _knife_move_back;

//lista jednokierunkowa
extern wezel *L;
extern uint16_t p;

uint16_t cutting_impulses = 0;
extern uint8_t set_cutting;

void screen11_init() {
	//zmiana prędkości zamykania noży
	htim4.Init.Period = 143;
	if (HAL_TIM_OnePulse_Init(&htim4, TIM_OPMODE_REPETITIVE) != HAL_OK) {
		Error_Handler();
	}
	base = 0;
	zwroc(L, p, &_name, &_pcs, &_pcs_done, &_length, &_left_cov, &_left_eye, &_right_eye,
			&_right_cov, &_knife, &_knife_move_back);
	sprintf(temp_name, "%s", _name);
	sprintf(temp_pcs, "%d", _pcs);
	sprintf(temp_pcs_done, "%d", _pcs_done);
	sprintf(temp_length, "%d", _length);

	sprintf(temp_left_cov, "%d", _left_cov);
	sprintf(temp_left_eye, "%d", _left_eye);
	sprintf(temp_right_eye, "%d", _right_eye);
	sprintf(temp_right_cov, "%d", _right_cov);
	sprintf(temp_knife, "%d", _knife);
	sprintf(temp_knife_move_back, "%d", _knife_move_back);

	size = sprintf(data, "LOAD 0 0 11.bmp\n\r");
	HAL_UART_Transmit(&huart2, data, size, 100);

	size = sprintf(data, "Zaladowano ekran 11\n\r");
	HAL_UART_Transmit(&huart1, data, size, 1000);

	size = sprintf(data, "Wartosc noza: %d\n\r", _knife);
	HAL_UART_Transmit(&huart1, data, size, 1000);
}

int screen11_button() {
	if (((0 < position_x) && (position_x < 45))
			&& ((0 < position_y) && (position_y < 45))) {
		size = sprintf(data, "BUZ 150 2000\n\r");
		HAL_UART_Transmit(&huart2, data, size, 100);
		return 1;
	}

	if (((45 < position_x) && (position_x < 90))
			&& ((0 < position_y) && (position_y < 45))) {
		size = sprintf(data, "BUZ 150 2000\n\r");
		HAL_UART_Transmit(&huart2, data, size, 100);
		return 2;
	}

	if (((410 < position_x) && (position_x < 460))
			&& ((205 < position_y) && (position_y < 250))) {
		size = sprintf(data, "BUZ 150 2000\n\r");
		HAL_UART_Transmit(&huart2, data, size, 100);
		return 3;
	}

//	if (((255 < position_x) && (position_x < 300))
//			&& ((130 < position_y) && (position_y < 175))) {
//		size = sprintf(data, "BUZ 150 2000\n\r");
//		HAL_UART_Transmit(&huart2, data, size, 100);
//		return 3;
//	}
//
//	if (((340 < position_x) && (position_x < 385))
//			&& ((130 < position_y) && (position_y < 175))) {
//		size = sprintf(data, "BUZ 150 2000\n\r");
//		HAL_UART_Transmit(&huart2, data, size, 100);
//		return 4;
//	}

//	if (((90 < position_x) && (position_x < 190))
//			&& ((110 < position_y) && (position_y < 210))) {
//		size = sprintf(data, "BUZ 150 2000\n\r");
//		HAL_UART_Transmit(&huart2, data, size, 100);
//		return 5;
//	}

	if (((365 < position_x) && (position_x < 410))
			&& ((205 < position_y) && (position_y < 250))) {
		size = sprintf(data, "BUZ 150 2000\n\r");
		HAL_UART_Transmit(&huart2, data, size, 100);
		return 10;
	}

	if (((320 < position_x) && (position_x < 365))
			&& ((70 < position_y) && (position_y < 115))) {
		size = sprintf(data, "BUZ 150 2000\n\r");
		HAL_UART_Transmit(&huart2, data, size, 100);
		return 11;
	}

	if (((365 < position_x) && (position_x < 410))
			&& ((70 < position_y) && (position_y < 115))) {
		size = sprintf(data, "BUZ 150 2000\n\r");
		HAL_UART_Transmit(&huart2, data, size, 100);
		return 12;
	}

	if (((410 < position_x) && (position_x < 460))
			&& ((70 < position_y) && (position_y < 115))) {
		size = sprintf(data, "BUZ 150 2000\n\r");
		HAL_UART_Transmit(&huart2, data, size, 100);
		return 13;
	}

	if (((320 < position_x) && (position_x < 365))
			&& ((115 < position_y) && (position_y < 160))) {
		size = sprintf(data, "BUZ 150 2000\n\r");
		HAL_UART_Transmit(&huart2, data, size, 100);
		return 14;
	}

	if (((365 < position_x) && (position_x < 410))
			&& ((115 < position_y) && (position_y < 160))) {
		size = sprintf(data, "BUZ 150 2000\n\r");
		HAL_UART_Transmit(&huart2, data, size, 100);
		return 15;
	}

	if (((410 < position_x) && (position_x < 460))
			&& ((115 < position_y) && (position_y < 160))) {
		size = sprintf(data, "BUZ 150 2000\n\r");
		HAL_UART_Transmit(&huart2, data, size, 100);
		return 16;
	}

	if (((320 < position_x) && (position_x < 365))
			&& ((160 < position_y) && (position_y < 205))) {
		size = sprintf(data, "BUZ 150 2000\n\r");
		HAL_UART_Transmit(&huart2, data, size, 100);
		return 17;
	}

	if (((365 < position_x) && (position_x < 410))
			&& ((160 < position_y) && (position_y < 205))) {
		size = sprintf(data, "BUZ 150 2000\n\r");
		HAL_UART_Transmit(&huart2, data, size, 100);
		return 18;
	}

	if (((410 < position_x) && (position_x < 460))
			&& ((160 < position_y) && (position_y < 205))) {
		size = sprintf(data, "BUZ 150 2000\n\r");
		HAL_UART_Transmit(&huart2, data, size, 100);
		return 19;
	}

	if (((320 < position_x) && (position_x < 365))
			&& ((205 < position_y) && (position_y < 250))) {
		size = sprintf(data, "BUZ 150 2000\n\r");
		HAL_UART_Transmit(&huart2, data, size, 100);
		return 20;
	}

	if (((82 < position_x) && (position_x < 202))
			&& ((182 < position_y) && (position_y < 215))) {
		size = sprintf(data, "BUZ 150 2000\n\r");
		HAL_UART_Transmit(&huart2, data, size, 100);
		return 21;
	}

	if (((82 < position_x) && (position_x < 202))
			&& ((216 < position_y) && (position_y < 248))) {
		size = sprintf(data, "BUZ 150 2000\n\r");
		HAL_UART_Transmit(&huart2, data, size, 100);
		return 22;
	}


	position_x = 0;
	position_y = 0;
	return 0;
	//0 - puste miejsce
	//1 - zamknij/powrót
	//2 - zapisz
	//3 - usuwanie wartości
	//5 - pozycja zapisana (wyjściowa)
	//10 - zero
	//11 - jeden
	//12 - dwa
	//13 - trzy
	//14 - cztery
	//15 - pięć
	//16 - sześć
	//17 - siedem
	//18 - osiem
	//19 - dziewięć
	//20 - przecinek
	//21 - średnica rdzenia przewodu
	//22 - odskok noży
}

void screen11_action(int button) {

	switch (button) {

	case 1:
		base = 0;
		screen = 4;
		action = 1;
		cutting_impulses = 0;

		param_number = 0;
		dot_char = 0;
		max_size1 = 0;
		break;

	case 2:
		base = 0;

		_pcs = atoi(temp_pcs);
		_pcs_done = atoi(temp_pcs_done);
		_length = atoi(temp_length);
		_left_cov = atoi(temp_left_cov);
		_left_eye = atoi(temp_left_eye);
		_right_eye = atoi(temp_right_eye);
		_right_cov = atoi(temp_right_cov);
		_knife_move_back = 50; //TODO: Dopisać ustawianie odskoku noży na ekranie
		//_knife = atoi(temp_knife); nie jest potrzebne, bo potrzebujemy cutting_impulses które są liczone w mainie

		usun(&L, p);
		wstaw(&L, p, temp_name, _pcs, _pcs_done, _length, _left_cov, _left_eye, _right_eye,
				_right_cov, cutting_impulses, _knife_move_back);


		screen = 4;
		action = 1;
		cutting_impulses = 0;

		param_number = 0;
		dot_char = 0;
		max_size1 = 0;
		break;

	case 3:
		switch (param_number) {

		case 1:
			sprintf(temp_knife_mm, "");
			size = sprintf(data, "CLR 82 177 202 212 65535\n\r");
			HAL_UART_Transmit(&huart2, data, size, 100);
			break;

		case 2:
			sprintf(temp_knife_move_back_mm, "");
			size = sprintf(data, "CLR 82 227 202 248 65535\n\r");
			HAL_UART_Transmit(&huart2, data, size, 100);
			break;
		}

		dot_char = 0;
		break;
//		if ((cutting_impulses < 900) && base == 1) {
//			//zmiana prędkości zamykania noży
//			htim4.Init.Period = 143;
//			if (HAL_TIM_OnePulse_Init(&htim4, TIM_OPMODE_REPETITIVE)
//					!= HAL_OK) {
//				Error_Handler();
//			}
//			//kierunek zamykania
//			HAL_GPIO_WritePin(CW_C_GPIO_Port, CW_C_Pin, GPIO_PIN_RESET);
//			set_cutting = 1;
//		}


//	case 4:
//		if ((cutting_impulses > 0) && base == 1) {
//			//zmiana prędkości otwierania noży
//			htim4.Init.Period = 143;
//			if (HAL_TIM_OnePulse_Init(&htim4, TIM_OPMODE_REPETITIVE)
//					!= HAL_OK) {
//				Error_Handler();
//			}
//			//kierunek otwierania
//			HAL_GPIO_WritePin(CW_C_GPIO_Port, CW_C_Pin, GPIO_PIN_SET);
//			set_cutting = 2;
//			//action = 1;
//		}
//		break;

	case 10:
		screen11_value_update('0');
		break;

	case 11:
		screen11_value_update('1');
		break;

	case 12:
		screen11_value_update('2');
		break;

	case 13:
		screen11_value_update('3');
		break;

	case 14:
		screen11_value_update('4');
		break;

	case 15:
		screen11_value_update('5');
		break;

	case 16:
		screen11_value_update('6');
		break;

	case 17:
		screen11_value_update('7');
		break;

	case 18:
		screen11_value_update('8');
		break;

	case 19:
		screen11_value_update('9');
		break;

	case 20:
		if(param_number == 1){
			screen11_value_update('.');
			dot_char = 1;
		}

		if(param_number == 2){
			screen11_value_update('.');
			dot_char = 2;
		}
		break;

	case 21:
		param_number = 1;
		max_size1 = 4;
		break;

	case 22:
		param_number = 2;
		max_size1 = 4;
		break;

	}
}

void screen11_value_update(char c) {
	uint8_t len = 0;

//	switch (param_number) {
//	case 1:
//		len = strlen(temp_knife_mm);
//		if (len < max_size1 && dot_char < 1) {
//
//			char *str2 = malloc(len + 1 + 1); //allokacja pamięci tla tymczasowego stringa - zwiększenie o jeden plus jeden dla znaku końca
//			strcpy(str2, temp_knife_mm);
//
//			str2[len] = c;
//			str2[len + 1] = '\0';
//
//			size = sprintf(data, "%s\n\r", str2);
//			HAL_UART_Transmit(&huart1, data, size, 1000);
//			strcpy(temp_knife_mm, str2);
//			if (atof(temp_knife_mm) > 12) {
//				size = sprintf(data, "BUZ 500 500\n\r");
//				HAL_UART_Transmit(&huart2, data, size, 100);
//
//				sprintf(temp_knife_mm, "");
//				size = sprintf(data, "CLR 205 74 253 90 65535\n\r");
//				HAL_UART_Transmit(&huart2, data, size, 100);
//
//			} else {
//				size = sprintf(data, "CLR 205 74 253 90 65535\n\r");
//				HAL_UART_Transmit(&huart2, data, size, 100);
//
//				size = sprintf(data, "UF 3 %d 178 0 %s mm\n\r", 141-(strlen(temp_knife_mm)+3)*5, temp_knife_mm);
//				HAL_UART_Transmit(&huart2, data, size, 100);
//			}
//			free(str2);
//		}
//		break;
//
//	case 2:
//		len = strlen(temp_knife_move_back_mm);
//		if (len < max_size1 && dot_char < 2) {
//
//			char *str2 = malloc(len + 1 + 1); //allokacja pamięci tla tymczasowego stringa - zwiększenie o jeden plus jeden dla znaku końca
//			strcpy(str2, temp_knife_move_back_mm);
//
//			str2[len] = c;
//			str2[len + 1] = '\0';
//
//			size = sprintf(data, "%s\n\r", str2);
//			HAL_UART_Transmit(&huart1, data, size, 1000);
//			strcpy(temp_knife_move_back_mm, str2);
//			if (atof(temp_knife_move_back_mm) > 12) {
//				size = sprintf(data, "BUZ 500 500\n\r");
//				HAL_UART_Transmit(&huart2, data, size, 100);
//
//				sprintf(temp_knife_move_back_mm, "");
//				size = sprintf(data, "CLR 205 228 253 248 65535\n\r");
//				HAL_UART_Transmit(&huart2, data, size, 100);
//
//			} else {
//				size = sprintf(data, "CLR 205 228 253 248 65535\n\r");
//				HAL_UART_Transmit(&huart2, data, size, 100);
//
//				size = sprintf(data, "UF 3 %d 234 0 %s mm\n\r", 141-(strlen(temp_knife_move_back_mm)+3)*5, temp_knife_move_back_mm);
//				HAL_UART_Transmit(&huart2, data, size, 100);
//			}
//			free(str2);
//		}
//		break;
//
//	}
}
