/*
 * screen11.c
 *
 *  Created on: 25.01.2017
 *      Author: gorian
 *      Ekran: Ustawienia noży
 */
#include "screen11.h"
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

extern uint8_t key_delay;
extern uint8_t param_number;
extern uint8_t max_size1;
uint8_t dot_char_1, dot_char_2, first_char_1, first_char_2 =0;

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
double tempFloat;

//lista jednokierunkowa
extern wezel *L;
extern uint16_t p;

uint16_t cutting_impulses = 0;
extern uint8_t set_cutting;

void screen11_init() {
	//zmiana prędkości zamykania noży do bazowania
//	htim4.Init.Period = 143;
//	if (HAL_TIM_OnePulse_Init(&htim4, TIM_OPMODE_REPETITIVE) != HAL_OK) {
//		Error_Handler();
//	}
//	base = 0;
	char temp_str[5];

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

//	tempFloat= 3.1;

//	150 – 12
//	900 – 0
//
//	y=ax+b
//
//	12=150a+b
//	0=900a+b
//	b=-900a
//
//	12=150a-900a
//	12=-750a
//	a=-12/750=-0.016
//
//	b=-900*-0.016
//
//	y=-12/750x+14.4
//
//	1=-0.016x+14.4
//	13.4/0.016= x
//	1 [mm]=837.5
//	x = 50 [imp]

	sprintf(temp_str, "%.2f", -0.016*_knife+14.4);
	size = sprintf(data, "UF 3 %d 188 0 %.1f mm\n\r", 141-(strlen(temp_str)+3)*5, -0.016*_knife+14.4);
	HAL_UART_Transmit(&huart2, data, size, 100);

	sprintf(temp_str, "%.2f", _knife_move_back/837.5);
	size = sprintf(data, "UF 3 %d 244 0 %.1f mm\n\r", 141-(strlen(temp_str)+3)*5, _knife_move_back/(900-837.5));
	HAL_UART_Transmit(&huart2, data, size, 100);

	size = sprintf(data, "Zaladowano ekran 11\n\r");
	HAL_UART_Transmit(&huart1, data, size, 100);

	size = sprintf(data, "Wartosc noza: %d  %.1f  %.1f\n\r", _knife, -0.016*_knife+14.4, _knife_move_back/(900-837.5));
	HAL_UART_Transmit(&huart1, data, size, 100);

	dot_char_1 = 0;
	dot_char_2 = 0;
	first_char_1 = 0;
	first_char_2 = 0;
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

	if (((380 < position_x) && (position_x < 425))
			&& ((0 < position_y) && (position_y < 45))) {
		size = sprintf(data, "BUZ 150 2000\n\r");
		HAL_UART_Transmit(&huart2, data, size, 100);
		return 4;
	}

	if (((425 < position_x) && (position_x < 470))
			&& ((0 < position_y) && (position_y < 45))) {
		size = sprintf(data, "BUZ 150 2000\n\r");
		HAL_UART_Transmit(&huart2, data, size, 100);
		return 5;
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
			&& ((182 < position_y) && (position_y < 225))) {
		size = sprintf(data, "BUZ 150 2000\n\r");
		HAL_UART_Transmit(&huart2, data, size, 100);
		return 21;
	}

	if (((82 < position_x) && (position_x < 202))
			&& ((226 < position_y) && (position_y < 258))) {
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
	//4 - plus noże
	//5 - minus noże
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
//		base = 0;
		screen = 4;
		action = 1;
		cutting_impulses = 0;

		param_number = 0;
		dot_char_1 = 0;
		dot_char_2 = 0;
		first_char_1 = 0;
		first_char_2 = 0;
		max_size1 = 0;
		break;

	case 2:
//		base = 0;

		_pcs = atoi(temp_pcs);
		_pcs_done = atoi(temp_pcs_done);
		_length = atoi(temp_length);
		_left_cov = atoi(temp_left_cov);
		_left_eye = atoi(temp_left_eye);
		_right_eye = atoi(temp_right_eye);
		_right_cov = atoi(temp_right_cov);

		_knife = (int)(-750/12*atof(temp_knife_mm)+900);
		_knife_move_back = (int)(atof(temp_knife_move_back_mm)*(900-837.5));
		//_knife = atoi(temp_knife); nie jest potrzebne, bo potrzebujemy cutting_impulses które są liczone w mainie
		if(atof(temp_knife_mm) >= 0.1 && atof(temp_knife_move_back_mm) >= 0.1){
			usun(&L, p);
			wstaw(&L, p, temp_name, _pcs, _pcs_done, _length, _left_cov, _left_eye, _right_eye,
					_right_cov, _knife , _knife_move_back);
	//		cutting_impulses

			screen = 4;
			action = 1;
			cutting_impulses = 0;

			param_number = 0;
			dot_char_1 = 0;
			dot_char_2 = 0;
			first_char_1 = 0;
			first_char_2 = 0;
			max_size1 = 0;

		} else if(atof(temp_knife_mm) < 0.1){

			size = sprintf(data, "BUZ 500 500\n\r");
			HAL_UART_Transmit(&huart2, data, size, 100);

			sprintf(temp_knife_mm, "");
			size = sprintf(data, "CLR 82 187 202 222 65535\n\r");
			HAL_UART_Transmit(&huart2, data, size, 100);

		} else if(atof(temp_knife_move_back_mm) < 0.1){

			size = sprintf(data, "BUZ 500 500\n\r");
			HAL_UART_Transmit(&huart2, data, size, 100);

			sprintf(temp_knife_move_back_mm, "");
			size = sprintf(data, "CLR 82 237 202 270 65535\n\r");
			HAL_UART_Transmit(&huart2, data, size, 100);

		}

		break;

	case 3:
		switch (param_number) {

		case 1:
			sprintf(temp_knife_mm, "");
			size = sprintf(data, "CLR 82 187 202 222 65535\n\r");
			HAL_UART_Transmit(&huart2, data, size, 100);
			dot_char_1 = 0;
			first_char_1 = 0;
			break;

		case 2:
			sprintf(temp_knife_move_back_mm, "");
			size = sprintf(data, "CLR 82 237 202 270 65535\n\r");
			HAL_UART_Transmit(&huart2, data, size, 100);
			dot_char_2 = 0;
			first_char_2 = 0;
			break;
		}


		break;

	case 4:
		if ((cutting_impulses < 900) && base == 1) {
			//zmiana prędkości zamykania noży
			htim4.Init.Period = 143;
			if (HAL_TIM_OnePulse_Init(&htim4, TIM_OPMODE_REPETITIVE)
					!= HAL_OK) {
				Error_Handler();
			}
			//kierunek zamykania
			HAL_GPIO_WritePin(CW_C_GPIO_Port, CW_C_Pin, GPIO_PIN_RESET);
			set_cutting = 1;
		}
		break;


	case 5:
		if ((cutting_impulses > 0) && base == 1) {
			//zmiana prędkości otwierania noży
			htim4.Init.Period = 143;
			if (HAL_TIM_OnePulse_Init(&htim4, TIM_OPMODE_REPETITIVE)
					!= HAL_OK) {
				Error_Handler();
			}
			//kierunek otwierania
			HAL_GPIO_WritePin(CW_C_GPIO_Port, CW_C_Pin, GPIO_PIN_SET);
			set_cutting = 2;
			//action = 1;
		}
		break;

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
			if(strlen(temp_knife_mm) > 0 && dot_char_1 < 1){
				screen11_value_update('.');
				dot_char_1 = 1;
			}
		}

		if(param_number == 2){
			if(strlen(temp_knife_move_back_mm) > 0 && dot_char_2 < 1){
				screen11_value_update('.');
				dot_char_2 = 1;
			}
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
	uint8_t temp_len = 0;
	static bool x = 0;

	if(key_delay < 1){

		switch (param_number) {
		case 1:
			len = strlen(temp_knife_mm);
			if (len < max_size1) {
				//900 imp -> 0mm ; 150imp -> 12mm
				char *str2 = malloc(len + 1 + 1); //allokacja pamięci tla tymczasowego stringa - zwiększenie o jeden plus jeden dla znaku końca
				strcpy(str2, temp_knife_mm);

				str2[len] = c;
				str2[len + 1] = '\0';

				size = sprintf(data, "%s\n\r", str2);
				HAL_UART_Transmit(&huart1, data, size, 1000);
				strcpy(temp_knife_mm, str2);

				temp_len = strlen(temp_knife_mm);
				x = strcmp(temp_knife_mm, "00");
				size = sprintf(data, "mm float: %f; przecinek: %d; nap: %c\n\r", atof(temp_knife_mm), dot_char_1, temp_knife_mm[0] );
				HAL_UART_Transmit(&huart1, data, size, 100);
				size = sprintf(data, "dl: %d ; napisy: %d; dl. akt: %d\n\r", len, (temp_knife_mm[0] == '0')? 1 : 0, temp_len );
				HAL_UART_Transmit(&huart1, data, size, 100);

				if (c == '0' && len == 0) {
					first_char_1 = 1;
				}

				//obsługa niepoprawnych danych - jeśli większa od 12 lub mniejsza od 0.1 i był wpisany przecinek lub pierwszy znak był 0 a drugi jest inny od przecinka
				if (atof(temp_knife_mm) > 12 || (atof(temp_knife_mm) < 0.1 && dot_char_1 > 0 && len > 1) || (first_char_1 == 1 && c != '.' && strlen(temp_knife_mm) == 2)) {
					size = sprintf(data, "BUZ 500 500\n\r");
					HAL_UART_Transmit(&huart2, data, size, 100);

					sprintf(temp_knife_mm, "");
					size = sprintf(data, "CLR 82 187 202 222 65535\n\r");
					HAL_UART_Transmit(&huart2, data, size, 100);
					first_char_1 = 0;
					dot_char_1 = 0;

				} else {
					size = sprintf(data, "CLR 82 187 202 222 65535\n\r");
					HAL_UART_Transmit(&huart2, data, size, 100);

					size = sprintf(data, "UF 3 %d 188 0 %s mm\n\r", 141-(strlen(temp_knife_mm)+3)*5, temp_knife_mm);
					HAL_UART_Transmit(&huart2, data, size, 100);

				}
				free(str2);
			}
			break;

		case 2:
			len = strlen(temp_knife_move_back_mm);
			if (len < max_size1) {

				char *str2 = malloc(len + 1 + 1); //allokacja pamięci tla tymczasowego stringa - zwiększenie o jeden plus jeden dla znaku końca
				strcpy(str2, temp_knife_move_back_mm);

				str2[len] = c;
				str2[len + 1] = '\0';

				size = sprintf(data, "%s\n\r", str2);
				HAL_UART_Transmit(&huart1, data, size, 1000);
				strcpy(temp_knife_move_back_mm, str2);

				if (c == '0' && len == 0) {
					first_char_2 = 1;
				}

				//obsługa niepoprawnych danych - jeśli większa od 12 lub mniejsza od 0.1 i był wpisany przecinek lub pierwszy znak był 0 a drugi jest inny od przecinka
				if(atof(temp_knife_move_back_mm) > 12 || (atof(temp_knife_move_back_mm) < 0.1 && dot_char_2 > 0 && len > 1) || (first_char_2 == 1 && c != '.' && strlen(temp_knife_move_back_mm) == 2)) {
					size = sprintf(data, "BUZ 500 500\n\r");
					HAL_UART_Transmit(&huart2, data, size, 100);

					sprintf(temp_knife_move_back_mm, "");
					size = sprintf(data, "CLR 82 237 202 270 65535\n\r");
					HAL_UART_Transmit(&huart2, data, size, 100);
					first_char_2 = 0;
					dot_char_2 = 0;

				} else {
					size = sprintf(data, "CLR 82 237 202 270 65535\n\r");
					HAL_UART_Transmit(&huart2, data, size, 100);

					size = sprintf(data, "UF 3 %d 244 0 %s mm\n\r", 141-(strlen(temp_knife_move_back_mm)+3)*5, temp_knife_move_back_mm);
					HAL_UART_Transmit(&huart2, data, size, 100);
				}
				free(str2);
			}
			break;

		}
//		_knife = (int)(-750/12*atof(temp_knife_mm)+900);
//		_knife_move_back = (int)(atof(temp_knife_move_back_mm)*(900-837.5));
//
//		size = sprintf(data, "Noże impulsy: %f ; Noże impulsy: %d\n\r", (-750/12*atof(temp_knife_mm)+900), _knife);
//		HAL_UART_Transmit(&huart1, data, size, 100);
//
//		size = sprintf(data, "Odskok impulsy: %f ; Odskok impulsy: %d\n\r", (atof(temp_knife_move_back_mm)*(900-837.5)) , _knife_move_back);
//		HAL_UART_Transmit(&huart1, data, size, 100);

		key_delay= 1;

	}
}
