/*
 * screen6.c
 *
 *  Created on: 17.11.2016
 *      Author: gorian
 *      Ekran: ekran wykonywania procesu (zielony) i końca procesu
 */
#include "screen6.h"
#include "lista.h"

extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;
extern uint8_t screen;
extern uint16_t size;
extern uint16_t position_x;
extern uint16_t position_y;
extern uint8_t action;

extern uint8_t data[50];
extern wezel *L;
extern uint16_t p;
extern uint16_t p_max;

extern uint8_t param_number;
extern uint8_t max_size1;

extern uint8_t start;
extern uint8_t start_begin;

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

void screen6_init(uint8_t hours, uint8_t minutes) {


	zwroc(L, p, &_name, &_pcs, &_pcs_done, &_length, &_left_cov, &_left_eye, &_right_eye,
			&_right_cov, &_knife, &_knife_move_back);
	sprintf(temp_name, "%s", _name);
	sprintf(temp_pcs, "%d", _pcs);
	sprintf(temp_pcs_done, "%d", _pcs_done);


	if(_pcs_done < _pcs){

		if(start_begin == 1){
			start_begin = 0;

			sprintf(temp_length, "%d", _length);

			sprintf(temp_left_cov, "%d", _left_cov);
			sprintf(temp_left_eye, "%d", _left_eye);
			sprintf(temp_right_eye, "%d", _right_eye);
			sprintf(temp_right_cov, "%d", _right_cov);
			sprintf(temp_knife, "%d", _knife);
			sprintf(temp_knife_move_back, "%d", _knife_move_back);
			size = sprintf(data, "LOAD 0 0 6.bmp\n\r");
			HAL_UART_Transmit(&huart2, data, size, 1);

			size = sprintf(data, "Zaladowano ekran 6\n\r");
			HAL_UART_Transmit(&huart1, data, size, 1);

			size = sprintf(data, "CLR 0 0 479 45 405441\n\r");
			HAL_UART_Transmit(&huart2, data, size, 1);

			size = sprintf(data, "UF 5 405 10 65535 %0.2d:%0.2d\n\r", hours, minutes);
			HAL_UART_Transmit(&huart2, data, size, 1);

			size = sprintf(data, "UF 3 10 15 65535 Program \"%s\" in progress\n\r", temp_name);
			HAL_UART_Transmit(&huart2, data, size, 1);



			size = sprintf(data, "UF 2 220 114 0 %s\n\r", temp_length);
			HAL_UART_Transmit(&huart2, data, size, 1);

			size = sprintf(data, "UF 2 174 152 0 %s\n\r", temp_left_eye);
			HAL_UART_Transmit(&huart2, data, size, 1);

			size = sprintf(data, "UF 2 277 152 0 %s\n\r", temp_right_eye);
			HAL_UART_Transmit(&huart2, data, size, 1);

			size = sprintf(data, "UF 2 137 243 0 %s\n\r", temp_left_cov);
			HAL_UART_Transmit(&huart2, data, size, 1);

			size = sprintf(data, "UF 2 317 243 0 %s\n\r", temp_right_cov);
			HAL_UART_Transmit(&huart2, data, size, 1);
		}

		size = sprintf(data, "CLR 200 73 210 90 65535\n\rUF 2 200 72 0 %s\/%s\n\r", temp_pcs_done, temp_pcs);
		HAL_UART_Transmit_IT(&huart2, data, size);

	} else {
		size = sprintf(data, "LOAD 0 0 6a.bmp\n\r");
		HAL_UART_Transmit(&huart2, data, size, 1);

		size = sprintf(data, "Zaladowano ekran 6a\n\r");
		HAL_UART_Transmit(&huart1, data, size, 1);


		size = sprintf(data, "UF 5 %d 160 12710 Program \"%s\" finished\n\r", 240-(strlen(temp_name)+19)*7, temp_name);
		HAL_UART_Transmit(&huart2, data, size, 1);

		size = sprintf(data, "UF 2 %d 195 40179 %s segments at %0.2d:%0.2d\n\r",240-(strlen(temp_pcs)+18)*5, temp_pcs, hours, minutes);
		HAL_UART_Transmit(&huart2, data, size, 1);

//		size = sprintf(data, "Wynik 1: %d\n\r", 240-(strlen(temp_pcs)+18)*5);
//		HAL_UART_Transmit_IT(&huart1, data, size);
//
//		size = sprintf(data, "Wynik 2: %d\n\r", 240-(strlen(temp_pcs)+33)*5);
//		HAL_UART_Transmit_IT(&huart1, data, size);
	}

	screen = 6;
}

int screen6_button() {

	if (((0 < position_x) && (position_x < 45))
			&& ((0 < position_y) && (position_y < 45))) {
		size = sprintf(data, "BUZ 150 2000\n\r");
		HAL_UART_Transmit(&huart2, data, size, 100);
		return 1;
	}
/*
	if (((45 < position_x) && (position_x < 90))
			&& ((0 < position_y) && (position_y < 45))) {
		size = sprintf(data, "BUZ 150 2000\n\r");
		HAL_UART_Transmit(&huart2, data, size, 100);
		return 2;
	}

	if (((110 < position_x) && (position_x < 155))
			&& ((0 < position_y) && (position_y < 45))) {
		size = sprintf(data, "BUZ 150 2000\n\r");
		HAL_UART_Transmit(&huart2, data, size, 100);
		return 3;
	}

	if (((325 < position_x) && (position_x < 370))
			&& ((0 < position_y) && (position_y < 45))) {
		size = sprintf(data, "BUZ 150 2000\n\r");
		HAL_UART_Transmit(&huart2, data, size, 100);
		return 4;
	}

	if (((0 < position_x) && (position_x < 45))
			&& ((137 < position_y) && (position_y < 182))) {
		size = sprintf(data, "BUZ 150 2000\n\r");
		HAL_UART_Transmit(&huart2, data, size, 100);
		return 5;
	}

	if (((0 < position_x) && (position_x < 45))
			&& ((182 < position_y) && (position_y < 227))) {
		size = sprintf(data, "BUZ 150 2000\n\r");
		HAL_UART_Transmit(&huart2, data, size, 100);
		return 6;
	}

	if (((0 < position_x) && (position_x < 45))
			&& ((227 < position_y) && (position_y < 272))) {
		size = sprintf(data, "BUZ 150 2000\n\r");
		HAL_UART_Transmit(&huart2, data, size, 100);
		return 7;
	}

	if (((390 < position_x) && (position_x < 479))
			&& ((0 < position_y) && (position_y < 45))) {
		size = sprintf(data, "BUZ 150 2000\n\r");
		HAL_UART_Transmit(&huart2, data, size, 100);
		return 8;
	}

	if (((155 < position_x) && (position_x < 325))
			&& ((0 < position_y) && (position_y < 45))) {
		size = sprintf(data, "BUZ 150 2000\n\r");
		HAL_UART_Transmit(&huart2, data, size, 100);
		return 10;
	}

	if (((200 < position_x) && (position_x < 280))
			&& ((65 < position_y) && (position_y < 90))) {
		size = sprintf(data, "BUZ 150 2000\n\r");
		HAL_UART_Transmit(&huart2, data, size, 100);
		return 11;
	}

	if (((210 < position_x) && (position_x < 260))
			&& ((110 < position_y) && (position_y < 130))) {
		size = sprintf(data, "BUZ 150 2000\n\r");
		HAL_UART_Transmit(&huart2, data, size, 100);
		return 12;
	}

	if (((125 < position_x) && (position_x < 167))
			&& ((235 < position_y) && (position_y < 255))) {
		size = sprintf(data, "BUZ 150 2000\n\r");
		HAL_UART_Transmit(&huart2, data, size, 100);
		return 13;
	}

	if (((163 < position_x) && (position_x < 205))
			&& ((150 < position_y) && (position_y < 170))) {
		size = sprintf(data, "BUZ 150 2000\n\r");
		HAL_UART_Transmit(&huart2, data, size, 100);
		return 14;
	}

	if (((266 < position_x) && (position_x < 307))
			&& ((150 < position_y) && (position_y < 170))) {
		size = sprintf(data, "BUZ 150 2000\n\r");
		HAL_UART_Transmit(&huart2, data, size, 100);
		return 15;
	}

	if (((308 < position_x) && (position_x < 346))
			&& ((235 < position_y) && (position_y < 255))) {
		size = sprintf(data, "BUZ 150 2000\n\r");
		HAL_UART_Transmit(&huart2, data, size, 100);
		return 16;
	}

	position_x = 0;
	position_y = 0;
	return 0;
	//0 - puste miejsce
	//1 - przycisk menu
	//2 - dodaj program
	//3 - w lewo <
	//4 - w prawo >
	//5 - regulacja noży
	//6 - AWG tabela
	//7 - info o producencie
	//8 - ustawianie zegara i daty
	//10 - nazwa programu
	//11 - ilość sztuk
	//12 - długość
	//13 - lewy odpad
	//14 - lewe oczko
	//15 - prawe oczko
	//16 - prawy odpad
	 */
}

void screen6_action(int button) {

	switch (button) {

	case 1:
		screen = 2;
		action = 1;
		break;
/*
	case 2:
		screen = 4;
		action = 1;
		break;

	case 3:
		if (p > 0) {
			p--;
			screen1_program_update();
		}
		break;

	case 4:
		if (p < (p_max - 1)) {
			p++;
			screen1_program_update();
		}
		break;

	case 5:

		start = 1;
		break;

	case 6:
		screen = 12;
		action = 1;
		break;

	case 7:
		screen = 13;
		action = 1;
		break;

	case 8:
		screen = 10;
		action = 1;
		break;

	case 10:
		param_number = 1;
		max_size1 = 18;
		screen = 4;
		action = 1;
		break;

	case 11:
		param_number = 2;
		max_size1 = 4;
		screen = 4;
		action = 1;
		break;

	case 12:
		param_number = 3;
		max_size1 = 3;
		screen = 4;
		action = 1;
		break;

	case 13:
		param_number = 4;
		max_size1 = 2;
		screen = 4;
		action = 1;
		break;

	case 14:
		param_number = 5;
		max_size1 = 2;
		screen = 4;
		action = 1;
		break;

	case 15:
		param_number = 6;
		max_size1 = 2;
		screen = 4;
		action = 1;
		break;

	case 16:
		param_number = 7;
		max_size1 = 2;
		screen = 4;
		action = 1;
		break;
*/
	default:
		action = 0;
		break;
	}

}
/*
void screen6_program_update() {
	zwroc(L, p, &_name, &_pcs, &_length, &_left_cov, &_left_eye, &_right_eye,
			&_right_cov, &_knife, &_knife_move_back);

	sprintf(temp_name, "%s", _name);
	sprintf(temp_pcs, "%d", _pcs);
	sprintf(temp_length, "%d", _length);

	sprintf(temp_left_cov, "%d", _left_cov);
	sprintf(temp_left_eye, "%d", _left_eye);
	sprintf(temp_right_eye, "%d", _right_eye);
	sprintf(temp_right_cov, "%d", _right_cov);
	sprintf(temp_knife, "%d", _knife);

	size = sprintf(data, "CLR 200 14 340 35 19481\n\r");
	HAL_UART_Transmit(&huart2, data, size, 1000);

	size = sprintf(data, "UF 3 200 15 65535 %s\n\r", temp_name);
	HAL_UART_Transmit(&huart2, data, size, 1000);

	size = sprintf(data, "CLR 200 76 240 87 65535\n\r");
	HAL_UART_Transmit(&huart2, data, size, 1000);

	size = sprintf(data, "UF 2 200 72 0 %s\n\r", temp_pcs);
	HAL_UART_Transmit(&huart2, data, size, 1000);

	size = sprintf(data, "CLR 220 118 250 130 65535\n\r");
	HAL_UART_Transmit(&huart2, data, size, 1000);

	size = sprintf(data, "UF 2 220 114 0 %s\n\r", temp_length);
	HAL_UART_Transmit(&huart2, data, size, 1000);

	size = sprintf(data, "CLR 174 156 194 167 65535\n\r");
	HAL_UART_Transmit(&huart2, data, size, 1000);

	size = sprintf(data, "UF 2 174 152 0 %s\n\r", temp_left_eye);
	HAL_UART_Transmit(&huart2, data, size, 1000);

	size = sprintf(data, "CLR 277 156 297 167 65535\n\r");
	HAL_UART_Transmit(&huart2, data, size, 1000);

	size = sprintf(data, "UF 2 277 152 0 %s\n\r", temp_right_eye);
	HAL_UART_Transmit(&huart2, data, size, 1000);

	size = sprintf(data, "CLR 137 246 156 258 65535\n\r");
	HAL_UART_Transmit(&huart2, data, size, 1000);

	size = sprintf(data, "UF 2 137 243 0 %s\n\r", temp_left_cov);
	HAL_UART_Transmit(&huart2, data, size, 1000);

	size = sprintf(data, "CLR 317 246 336 258 65535\n\r");
	HAL_UART_Transmit(&huart2, data, size, 1000);

	size = sprintf(data, "UF 2 317 243 0 %s\n\r", temp_right_cov);
	HAL_UART_Transmit(&huart2, data, size, 1000);
}
*/
