/*
 * screen4.c
 *
 *  Created on: 25.11.2016
 *      Author: gorian
 *      Ekran: Zmiana ustawień długości przewodów do cięcia
 */
#include "screen4.h"
#include "lista.h"
#include <stdlib.h>
#include <string.h>

extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;
extern uint8_t screen;
extern uint16_t size;
extern uint16_t position_x;
extern uint16_t position_y;
extern uint8_t action;

extern uint8_t data[50];
extern uint8_t keyboard_timer;

uint8_t param_number = 0;
uint8_t max_size1 = 4;
uint8_t error = 0;
uint16_t temp1 = 0;
char *temp_str[1];
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
extern char last_char;
extern uint8_t interrupt;
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
extern uint8_t node_to_delete;

uint8_t button_calls = 0;

void screen4_init() {

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

	size = sprintf(data, "LOAD 0 0 4.bmp\n\r");
	HAL_UART_Transmit(&huart2, data, size, 100);

	if(p==0){	//zasloniecie kosza dla zerowego programu
		size = sprintf(data, "CLR 0 235 45 270 65535\n\r");
		HAL_UART_Transmit(&huart2, data, size, 1000);
	}

	size = sprintf(data, "Zaladowano ekran 4\n\r");
	HAL_UART_Transmit(&huart1, data, size, 1000);

	size = sprintf(data, "UF 3 100 15 65535 %s\n\r", temp_name);
	HAL_UART_Transmit(&huart2, data, size, 1000);

	size = sprintf(data, "UF 2 %d 72 0 %s PCS\n\r", 214-(strlen(temp_pcs)+5)*5, temp_pcs);
	HAL_UART_Transmit(&huart2, data, size, 1000);

	size = sprintf(data, "UF 2 %d 114 0 %s\n\r", 178-(strlen(temp_length)*5), temp_length);
	HAL_UART_Transmit(&huart2, data, size, 1000);

	size = sprintf(data, "UF 2 %d 152 0 %s\n\r", 126-strlen(temp_left_eye)*5, temp_left_eye);
	HAL_UART_Transmit(&huart2, data, size, 1000);

	size = sprintf(data, "UF 2 %d 152 0 %s\n\r", 230-strlen(temp_right_eye)*5, temp_right_eye);
	HAL_UART_Transmit(&huart2, data, size, 1000);

	size = sprintf(data, "UF 2 %d 243 0 %s\n\r", 89-strlen(temp_left_cov)*5, temp_left_cov);
	HAL_UART_Transmit(&huart2, data, size, 1000);

	size = sprintf(data, "UF 2 %d 243 0 %s\n\r", 268-strlen(temp_right_cov)*5,temp_right_cov);
	HAL_UART_Transmit(&huart2, data, size, 1000);

	screen = 4;
}

int screen4_button() {
	if (((0 < position_x) && (position_x < 45))
			&& ((0 < position_y) && (position_y < 45))) {
		size = sprintf(data, "BUZ 150 2000\n\r");
		HAL_UART_Transmit(&huart2, data, size, 100);
		if (error == 0) {
			return 1;
		} else {
			error = 0;
			return 8;
		}
	}

	if (((45 < position_x) && (position_x < 90))
			&& ((0 < position_y) && (position_y < 45)) && (error == 0)) {
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

	if (((290 < position_x) && (position_x < 335))
			&& ((0 < position_y) && (position_y < 45)) && (error == 0)) {
		size = sprintf(data, "BUZ 150 2000\n\r");
		HAL_UART_Transmit(&huart2, data, size, 100);
		return 4;
	}

	if (((335 < position_x) && (position_x < 380))
			&& ((0 < position_y) && (position_y < 45)) && (error == 0)) {
		size = sprintf(data, "BUZ 150 2000\n\r");
		HAL_UART_Transmit(&huart2, data, size, 100);
		return 5;
	}

	if (((380 < position_x) && (position_x < 425))
			&& ((0 < position_y) && (position_y < 45)) && (error == 0)) {
		size = sprintf(data, "BUZ 150 2000\n\r");
		HAL_UART_Transmit(&huart2, data, size, 100);
		return 6;
	}

	if (((425 < position_x) && (position_x < 470))
			&& ((0 < position_y) && (position_y < 45)) && (error == 0)) {
		size = sprintf(data, "BUZ 150 2000\n\r");
		HAL_UART_Transmit(&huart2, data, size, 100);
		return 7;
	}

	if (((0 < position_x) && (position_x < 45))
			&& ((180 < position_y) && (position_y < 225)) && (error == 0)) {
		size = sprintf(data, "BUZ 150 2000\n\r");
		HAL_UART_Transmit(&huart2, data, size, 100);
		return 9;
	}

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

	if (((90 < position_x) && (position_x < 275))
			&& ((0 < position_y) && (position_y < 45))) {
		size = sprintf(data, "BUZ 150 2000\n\r");
		HAL_UART_Transmit(&huart2, data, size, 100);
		return 20;
	}

	if (((150 < position_x) && (position_x < 300))
			&& ((65 < position_y) && (position_y < 100))) {
		size = sprintf(data, "BUZ 150 2000\n\r");
		HAL_UART_Transmit(&huart2, data, size, 100);
		return 21;
	}

	if (((90 < position_x) && (position_x < 270))
			&& ((110 < position_y) && (position_y < 140))) {
		size = sprintf(data, "BUZ 150 2000\n\r");
		HAL_UART_Transmit(&huart2, data, size, 100);
		return 22;
	}

	if (((70 < position_x) && (position_x < 110))
			&& ((230 < position_y) && (position_y < 270))) {
		size = sprintf(data, "BUZ 150 2000\n\r");
		HAL_UART_Transmit(&huart2, data, size, 100);
		return 23;
	}

	if (((105 < position_x) && (position_x < 150))
			&& ((150 < position_y) && (position_y < 175))) {
		size = sprintf(data, "BUZ 150 2000\n\r");
		HAL_UART_Transmit(&huart2, data, size, 100);
		return 24;
	}

	if (((210 < position_x) && (position_x < 250))
			&& ((150 < position_y) && (position_y < 175))) {
		size = sprintf(data, "BUZ 150 2000\n\r");
		HAL_UART_Transmit(&huart2, data, size, 100);
		return 25;
	}

	if (((250 < position_x) && (position_x < 290))
			&& ((230 < position_y) && (position_y < 270))) {
		size = sprintf(data, "BUZ 150 2000\n\r");
		HAL_UART_Transmit(&huart2, data, size, 100);
		return 26;
	}

	if (((0 < position_x) && (position_x < 45) && p>0)
			&& ((235 < position_y) && (position_y < 270))) {
		size = sprintf(data, "BUZ 150 2000\n\r");
		HAL_UART_Transmit(&huart2, data, size, 100);
		return 27;
	}

	position_x = 0;
	position_y = 0;
	return 0;
	//0 - puste miejsce
	//1 - powrót do ekranu 2 (wyjście)
	//2 - zapisanie ustawień i wyjście do ekranu 2
	//3 - usuwanie wartości
	//4 - -1
	//5 - +1
	//6 - -10
	//7 - +10
	//8 - wyjście z komunikatu o awarii przy nieprawidłowych wartościach
	//9 - zerowanie noży dla nacięcia opony
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
	//20 - nazwa programu
	//21 - ilość sztuk
	//22 - długość max
	//23 - lewe skórowanie
	//24 - lewe oczko
	//25 - prawe oczko
	//26 - prawe skórowanie
	//27 - kosz - usuwanie programu
	// Y 70; 115; 160; 205; 250
	//X 320; 365; 410; 460

}

void screen4_action(int button) {
	switch (button) {

	case 1:
		sprintf(temp_name, "");
		sprintf(temp_str, "");
		sprintf(temp_pcs_done, "");
		sprintf(temp_pcs, "");
		sprintf(temp_length, "");
		sprintf(temp_left_cov, "");
		sprintf(temp_left_eye, "");
		sprintf(temp_right_eye, "");
		sprintf(temp_right_cov, "");
		sprintf(temp_knife, "");
		sprintf(temp_knife_move_back, "");
		param_number = 0;
		screen = 2;
		action = 1;
		break;

		// XXX: zapisywanie wartości
	case 2:
		screen4_save();
		if(error <1){
			screen = 2;
			action = 1;
		}
		break;

	case 3:

		switch (param_number) {
		case 1:
			//usuwa od końca znak po znaku
			temp_name[strlen(temp_name) - 1] = '\0';

			size = sprintf(data, "CLR 90 14 300 40 62882\n\r");
			HAL_UART_Transmit(&huart2, data, size, 100);
			size = sprintf(data, "UF 3 100 15 65535 %s\n\r", temp_name);
			HAL_UART_Transmit(&huart2, data, size, 100);
			break;
		case 2:
			sprintf(temp_pcs, "");
			size = sprintf(data, "CLR 160 74 270 90 65535\n\r");
			HAL_UART_Transmit(&huart2, data, size, 100);
			break;
		case 3:
			sprintf(temp_length, "");
			size = sprintf(data, "CLR 140 114 220 131 65535\n\r");
			HAL_UART_Transmit(&huart2, data, size, 100);
			break;
		case 4:
			sprintf(temp_left_cov, "");
			size = sprintf(data, "CLR 77 243 100 260 65535\n\r");
			HAL_UART_Transmit(&huart2, data, size, 100);
			break;
		case 5:
			sprintf(temp_left_eye, "");
			size = sprintf(data, "CLR 114 152 135 168 65535\n\r");
			HAL_UART_Transmit(&huart2, data, size, 100);
			break;
		case 6:
			sprintf(temp_right_eye, "");
			size = sprintf(data, "CLR 217 154 240 168 65535\n\r");
			HAL_UART_Transmit(&huart2, data, size, 100);
			break;
		case 7:
			sprintf(temp_right_cov, "");
			size = sprintf(data, "CLR 257 243 280 260 65535\n\r");
			HAL_UART_Transmit(&huart2, data, size, 100);
			break;
		default:
			sprintf(temp_str, "");
			break;
		}
		size = sprintf(data, "Clear\n\r");
		HAL_UART_Transmit(&huart1, data, size, 1000);
		break;

	case 4:
		screen4_plus_minus(1);
		size = sprintf(data, "Minus 1\n\r");
		HAL_UART_Transmit(&huart1, data, size, 1000);
		break;

	case 5:
		screen4_plus_minus(2);
		size = sprintf(data, "Plus 1\n\r");
		HAL_UART_Transmit(&huart1, data, size, 1000);
		break;

	case 6:
		screen4_plus_minus(3);
		size = sprintf(data, "Minus 10\n\r");
		HAL_UART_Transmit(&huart1, data, size, 1000);
		break;

	case 7:
		screen4_plus_minus(4);
		size = sprintf(data, "Plus 10\n\r");
		HAL_UART_Transmit(&huart1, data, size, 1000);
		break;

	case 8:
		size = sprintf(data, "LOAD 0 0 edit.bmp\n\r");
		HAL_UART_Transmit(&huart2, data, size, 1000);

		size = sprintf(data, "UF 3 100 15 65535 %s\n\r", temp_name);
		HAL_UART_Transmit(&huart2, data, size, 100);
		break;

	case 9:
		//przełącz na ekran 11 - ustawianie noży
		screen4_save();
		if(error < 1){
			screen = 11;
			action = 1;
		}
		break;

	case 10:
		screen4_value_update('0');
		temp1 = atoi(temp_str);
		size = sprintf(data, "Zero Val: %d\n\r", temp1);
		HAL_UART_Transmit(&huart1, data, size, 1000);
		break;

	case 11:
		screen4_value_update('1');
		temp1 = atoi(temp_str);
		size = sprintf(data, "Jeden Val: %d\n\r", temp1);
		HAL_UART_Transmit(&huart1, data, size, 1000);
		break;

	case 12:
		screen4_value_update('2');
		temp1 = atoi(temp_str);
		size = sprintf(data, "Dwa Val: %d\n\r", temp1);
		HAL_UART_Transmit(&huart1, data, size, 1000);
		break;

	case 13:
		screen4_value_update('3');
		temp1 = atoi(temp_str);
		size = sprintf(data, "Trzy Val: %d\n\r", temp1);
		HAL_UART_Transmit(&huart1, data, size, 1000);
		break;

	case 14:
		screen4_value_update('4');
		temp1 = atoi(temp_str);
		size = sprintf(data, "Cztery Val: %d\n\r", temp1);
		HAL_UART_Transmit(&huart1, data, size, 1000);
		break;

	case 15:
		screen4_value_update('5');
		temp1 = atoi(temp_str);
		size = sprintf(data, "Piec Val: %d\n\r", temp1);
		HAL_UART_Transmit(&huart1, data, size, 1000);
		break;

	case 16:
		screen4_value_update('6');
		temp1 = atoi(temp_str);
		size = sprintf(data, "Szesc Val: %d\n\r", temp1);
		HAL_UART_Transmit(&huart1, data, size, 1000);
		break;

	case 17:
		screen4_value_update('7');
		temp1 = atoi(temp_str);
		size = sprintf(data, "Siedem Val: %d\n\r", temp1);
		HAL_UART_Transmit(&huart1, data, size, 1000);
		break;

	case 18:
		screen4_value_update('8');
		temp1 = atoi(temp_str);
		size = sprintf(data, "Osiem Val: %d\n\r", temp1);
		HAL_UART_Transmit(&huart1, data, size, 1000);
		break;

	case 19:
		screen4_value_update('9');
		temp1 = atoi(temp_str);
		size = sprintf(data, "Dziewiec Val: %d\n\r", temp1);
		HAL_UART_Transmit(&huart1, data, size, 1000);
		break;

	case 20:
		param_number = 1;
		max_size1 = 13;	//Maksymalna ilość znaków możliwa do wpisania w nazwie programu
		//sprintf(temp_name, "");
		size = sprintf(data, "Nazwa programu: \n\r");
		HAL_UART_Transmit(&huart1, data, size, 1000);
		break;

	case 21:
		param_number = 2;
		max_size1 = 4;
		//sprintf(temp_pcs, "");
		size = sprintf(data, "Ilosc sztuk\n\r");
		HAL_UART_Transmit(&huart1, data, size, 1000);
		break;

	case 22:
		param_number = 3;
		max_size1 = 3;
		//sprintf(temp_length, "");
		size = sprintf(data, "Dlugosc\n\r");
		HAL_UART_Transmit(&huart1, data, size, 1000);
		break;

	case 23:
		param_number = 4;
		max_size1 = 2;
		//sprintf(temp_left_cov, "");
		size = sprintf(data, "Lewa oslonka\n\r");
		HAL_UART_Transmit(&huart1, data, size, 1000);
		break;

	case 24:
		param_number = 5;
		max_size1 = 2;
		//sprintf(temp_left_eye, "");
		size = sprintf(data, "Lewe oczko\n\r");
		HAL_UART_Transmit(&huart1, data, size, 1000);
		break;

	case 25:
		param_number = 6;
		max_size1 = 2;
		//sprintf(temp_right_eye, "");
		size = sprintf(data, "Prawe oczko\n\r");
		HAL_UART_Transmit(&huart1, data, size, 1000);
		break;

	case 26:
		param_number = 7;
		max_size1 = 2;
		//sprintf(temp_right_cov, "");
		size = sprintf(data, "Prawa oslonka\n\r");
		HAL_UART_Transmit(&huart1, data, size, 1000);
		break;

	case 27:
		screen = 15;
		action = 1;
		node_to_delete = p;
		break;

	default:
		break;
	}
}

void screen4_value_update(char c) {
	uint8_t len = 0;
	uint8_t add = 0;

	switch (param_number) {
	case 1:
		len = strlen(temp_name);
		keyboard_timer = 1;

		if (c == last_char) {
			button_calls++;

			switch (c) {
			case '1':
				if (button_calls == 1) {
					temp_name[strlen(temp_name) - 1] = ',';
				}
				if ((1 < button_calls) && (button_calls < 5)) {
					temp_name[strlen(temp_name) - 1]++;
				}
				if (button_calls == 5) {
					temp_name[strlen(temp_name) - 1] = '1';
					button_calls = 0;
				}
				break;
			case '2':
				if (button_calls == 1) {
					temp_name[strlen(temp_name) - 1] = 'a';
				}
				if ((1 < button_calls) && (button_calls < 4)) {
					temp_name[strlen(temp_name) - 1]++;
				}
				if (button_calls == 4) {
					temp_name[strlen(temp_name) - 1] = '2';
					button_calls = 0;
				}
				break;

			case '3':
				if (button_calls == 1) {
					temp_name[strlen(temp_name) - 1] = 'd';
				}
				if ((1 < button_calls) && (button_calls < 4)) {
					temp_name[strlen(temp_name) - 1]++;
				}
				if (button_calls == 4) {
					temp_name[strlen(temp_name) - 1] = '3';
					button_calls = 0;
				}
				break;

			case '4':
				if (button_calls == 1) {
					temp_name[strlen(temp_name) - 1] = 'g';
				}
				if ((1 < button_calls) && (button_calls < 4)) {
					temp_name[strlen(temp_name) - 1]++;
				}
				if (button_calls == 4) {
					temp_name[strlen(temp_name) - 1] = '4';
					button_calls = 0;
				}
				break;

			case '5':
				if (button_calls == 1) {
					temp_name[strlen(temp_name) - 1] = 'j';
				}
				if ((1 < button_calls) && (button_calls < 4)) {
					temp_name[strlen(temp_name) - 1]++;
				}
				if (button_calls == 4) {
					temp_name[strlen(temp_name) - 1] = '5';
					button_calls = 0;
				}
				break;

			case '6':
				if (button_calls == 1) {
					temp_name[strlen(temp_name) - 1] = 'm';
				}
				if ((1 < button_calls) && (button_calls < 4)) {
					temp_name[strlen(temp_name) - 1]++;
				}
				if (button_calls == 4) {
					temp_name[strlen(temp_name) - 1] = '6';
					button_calls = 0;
				}
				break;

			case '7':
				if (button_calls == 1) {
					temp_name[strlen(temp_name) - 1] = 'p';
				}
				if ((1 < button_calls) && (button_calls < 5)) {
					temp_name[strlen(temp_name) - 1]++;
				}
				if (button_calls == 5) {
					temp_name[strlen(temp_name) - 1] = '7';
					button_calls = 0;
				}
				break;

			case '8':
				if (button_calls == 1) {
					temp_name[strlen(temp_name) - 1] = 't';
				}
				if ((1 < button_calls) && (button_calls < 4)) {
					temp_name[strlen(temp_name) - 1]++;
				}
				if (button_calls == 4) {
					temp_name[strlen(temp_name) - 1] = '8';
					button_calls = 0;
				}
				break;

			case '9':
				if (button_calls == 1) {
					temp_name[strlen(temp_name) - 1] = 'w';
				}
				if ((1 < button_calls) && (button_calls < 5)) {
					temp_name[strlen(temp_name) - 1]++;
				}
				if (button_calls == 5) {
					temp_name[strlen(temp_name) - 1] = '9';
					button_calls = 0;
				}
				break;

			case '0':
				if (button_calls == 1) {
					temp_name[strlen(temp_name) - 1] = ' ';
				}
				if ((1 < button_calls) && (button_calls < 5)) {
					temp_name[strlen(temp_name) - 1]++;
				}
				if (button_calls == 5) {
					temp_name[strlen(temp_name) - 1] = '0';
					button_calls = 0;
				}
				break;

			default:
				break;
			}

			size = sprintf(data, "CLR 90 14 300 40 62882\n\r");
			HAL_UART_Transmit(&huart2, data, size, 100);
			size = sprintf(data, "UF 3 100 15 65535 %s\n\r", temp_name);
			HAL_UART_Transmit(&huart2, data, size, 100);

		} else {
			if (len < max_size1) {
				//	keyboard_timer = 0;
				last_char = c;
				switch (c) {
				case '1':
					c = ',';
					break;
				case '2':
					c = 'a';
					break;
				case '3':
					c = 'd';
					break;
				case '4':
					c = 'g';
					break;
				case '5':
					c = 'j';
					break;
				case '6':
					c = 'm';
					break;
				case '7':
					c = 'p';
					break;
				case '8':
					c = 't';
					break;
				case '9':
					c = 'w';
					break;
				case '0':
					c = ' ';
					break;
				default:
					break;
				}
				button_calls = 1;

				char *str2 = malloc(len + 1 + 1);
				strcpy(str2, temp_name);
				str2[len] = c;
				str2[len + 1] = '\0';

				size = sprintf(data, "%s\n\r", str2);
				HAL_UART_Transmit(&huart1, data, size, 1000);

				strcpy(temp_name, str2);
				size = sprintf(data, "CLR 90 14 300 40 62882\n\r");
				HAL_UART_Transmit(&huart2, data, size, 100);
				size = sprintf(data, "UF 3 100 15 65535 %s\n\r", temp_name);
				HAL_UART_Transmit(&huart2, data, size, 100);
				free(str2);
			}
		}
		break;
	case 2:
		len = strlen(temp_pcs);
		break;
	case 3:
		len = strlen(temp_length);
		break;
	case 4:
		len = strlen(temp_left_cov);
		break;
	case 5:
		len = strlen(temp_left_eye);
		break;
	case 6:
		len = strlen(temp_right_eye);
		break;
	case 7:
		len = strlen(temp_right_cov);
		break;
	default:
		len = strlen(temp_str);
		break;
	}

	if (len < max_size1 && param_number > 1) {
		add = 0;

		char *str2 = malloc(len + 1 + 1); //allokacja pamięci tla tymczasowego stringa - zwiększenie o jeden plus jeden dla znaku końca

		switch (param_number) {
		case 2:
			strcpy(str2, temp_pcs);
			break;
		case 3:
			strcpy(str2, temp_length);
			break;
		case 4:
			strcpy(str2, temp_left_cov);
			break;
		case 5:
			strcpy(str2, temp_left_eye);
			break;
		case 6:
			strcpy(str2, temp_right_eye);
			break;
		case 7:
			strcpy(str2, temp_right_cov);
			break;
		default:
			strcpy(str2, temp_str);
			break;
		}

		str2[len] = c;
		str2[len + 1] = '\0';

		size = sprintf(data, "%s\n\r", str2);
		HAL_UART_Transmit(&huart1, data, size, 1000);

		//temp_str = (char *) realloc(temp_str, len + 1);

		switch (param_number) {

		case 2:
			strcpy(temp_pcs, str2);
			if (atoi(temp_pcs) > 9999) {
				size = sprintf(data, "BUZ 500 500\n\r");
				HAL_UART_Transmit(&huart2, data, size, 100);
				sprintf(temp_pcs, "");
				size = sprintf(data, "CLR 160 74 270 90 65535\n\r");
				HAL_UART_Transmit(&huart2, data, size, 100);

			} else {
				size = sprintf(data, "CLR 160 74 270 90 65535\n\r");
				HAL_UART_Transmit(&huart2, data, size, 100);

				size = sprintf(data, "UF 2 %d 72 0 %s PCS\n\r", 214-(strlen(temp_pcs)+5)*5, temp_pcs);
				HAL_UART_Transmit(&huart2, data, size, 100);
			}
			break;

		case 3:
			strcpy(temp_length, str2);
			if (atoi(temp_length) > 999) {
				size = sprintf(data, "BUZ 500 500\n\r");
				HAL_UART_Transmit(&huart2, data, size, 100);
				sprintf(temp_length, "");
				size = sprintf(data, "CLR 140 114 220 131 65535\n\r");
				HAL_UART_Transmit(&huart2, data, size, 100);

			} else {
				size = sprintf(data, "CLR 140 114 220 131 65535\n\r");
				HAL_UART_Transmit(&huart2, data, size, 100);

				size = sprintf(data, "UF 2 %d 114 0 %s\n\r", 178-(strlen(temp_length)*5), temp_length);
				HAL_UART_Transmit(&huart2, data, size, 100);
			}
			break;

		case 4:
			strcpy(temp_left_cov, str2);
			if (atoi(temp_left_cov) > 30) {
				size = sprintf(data, "BUZ 500 500\n\r");
				HAL_UART_Transmit(&huart2, data, size, 100);
				sprintf(temp_left_cov, "");
				size = sprintf(data, "CLR 77 243 100 260 65535\n\r");
				HAL_UART_Transmit(&huart2, data, size, 100);

			} else {
				size = sprintf(data, "CLR 77 243 100 260 65535\n\r");
				HAL_UART_Transmit(&huart2, data, size, 100);

				size = sprintf(data, "UF 2 %d 243 0 %s\n\r", 89-strlen(temp_left_cov)*5, temp_left_cov);
				HAL_UART_Transmit(&huart2, data, size, 100);
			}
			break;

		case 5:
			strcpy(temp_left_eye, str2);
			if (atoi(temp_left_eye) > 30) {
				size = sprintf(data, "BUZ 500 500\n\r");
				HAL_UART_Transmit(&huart2, data, size, 100);
				sprintf(temp_left_eye, "");
				size = sprintf(data, "CLR 114 152 135 168 65535\n\r");
				HAL_UART_Transmit(&huart2, data, size, 100);

			} else {
				size = sprintf(data, "CLR 114 152 135 168 65535\n\r");
				HAL_UART_Transmit(&huart2, data, size, 100);

				size = sprintf(data, "UF 2 %d 152 0 %s\n\r", 126-strlen(temp_left_eye)*5, temp_left_eye);
				HAL_UART_Transmit(&huart2, data, size, 100);
			}
			break;

		case 6:
			strcpy(temp_right_eye, str2);
			if (atoi(temp_right_eye) > 30) {
				size = sprintf(data, "BUZ 500 500\n\r");
				HAL_UART_Transmit(&huart2, data, size, 100);
				sprintf(temp_right_eye, "");
				size = sprintf(data, "CLR 217 154 240 168 65535\n\r");
				HAL_UART_Transmit(&huart2, data, size, 100);

			} else {
				size = sprintf(data, "CLR 217 154 240 168 65535\n\r");
				HAL_UART_Transmit(&huart2, data, size, 100);

				size = sprintf(data, "UF 2 %d 152 0 %s\n\r", 230-strlen(temp_right_eye)*5, temp_right_eye);
				HAL_UART_Transmit(&huart2, data, size, 100);
			}
			break;

		case 7:
			strcpy(temp_right_cov, str2);
			if (atoi(temp_right_cov) > 30) {
				size = sprintf(data, "BUZ 500 500\n\r");
				HAL_UART_Transmit(&huart2, data, size, 100);
				sprintf(temp_right_cov, "");
				size = sprintf(data, "CLR 257 243 280 260 65535\n\r");
				HAL_UART_Transmit(&huart2, data, size, 100);

			} else {
				size = sprintf(data, "CLR 257 243 280 260 65535\n\r");
				HAL_UART_Transmit(&huart2, data, size, 100);

				size = sprintf(data, "UF 2 %d 243 0 %s\n\r", 268-strlen(temp_right_cov)*5,temp_right_cov);
				HAL_UART_Transmit(&huart2, data, size, 100);

			}
			break;

		default:
			strcpy(temp_str, str2);
			break;
		}

		free(str2);
	}
}

void screen4_plus_minus(uint8_t option) {
	uint16_t temp = 0;

	switch (param_number) {

	case 2:
		temp = atoi(temp_pcs);
		if (temp > 9999) {
			size = sprintf(data, "BUZ 500 500\n\r");
			HAL_UART_Transmit(&huart2, data, size, 100);
			sprintf(temp_pcs, "");
			size = sprintf(data, "CLR 160 74 270 90 65535\n\r");
			HAL_UART_Transmit(&huart2, data, size, 100);

		} else {
			if (option == 1)
				temp -= 1;
			if (option == 2)
				temp += 1;
			if (option == 3)
				temp -= 10;
			if (option == 4)
				temp += 10;

			if (temp < 0)
				temp = 0;

			if (temp > 9999)
				temp = 9999;

			sprintf(temp_pcs, "%d", temp);
			size = sprintf(data, "CLR 160 74 270 90 65535\n\r");
			HAL_UART_Transmit(&huart2, data, size, 100);

			size = sprintf(data, "UF 2 %d 72 0 %s PCS\n\r", 214-(strlen(temp_pcs)+5)*5, temp_pcs);
			HAL_UART_Transmit(&huart2, data, size, 100);
		}
		break;

	case 3:
		temp = atoi(temp_length);
		if (temp > 999) {
			size = sprintf(data, "BUZ 500 500\n\r");
			HAL_UART_Transmit(&huart2, data, size, 100);
			sprintf(temp_length, "");
			size = sprintf(data, "CLR 140 114 220 131 65535\n\r");
			HAL_UART_Transmit(&huart2, data, size, 100);

		} else {
			if (option == 1)
				temp -= 1;
			if (option == 2)
				temp += 1;
			if (option == 3)
				temp -= 10;
			if (option == 4)
				temp += 10;

			if (temp < 0)
				temp = 0;

			if (temp > 999)
				temp = 999;

			sprintf(temp_length, "%d", temp);
			size = sprintf(data, "CLR 140 114 220 131 65535\n\r");
			HAL_UART_Transmit(&huart2, data, size, 100);

			size = sprintf(data, "UF 2 %d 114 0 %s\n\r", 178-(strlen(temp_length)*5), temp_length);
			HAL_UART_Transmit(&huart2, data, size, 100);
		}
		break;

	case 4:
		temp = atoi(temp_left_cov);
		if (temp > 30) {
			size = sprintf(data, "BUZ 500 500\n\r");
			HAL_UART_Transmit(&huart2, data, size, 100);
			sprintf(temp_left_cov, "");
			size = sprintf(data, "CLR 77 243 100 260 65535\n\r");
			HAL_UART_Transmit(&huart2, data, size, 100);

		} else {
			if (option == 1)
				temp -= 1;
			if (option == 2)
				temp += 1;
			if (option == 3)
				temp -= 10;
			if (option == 4)
				temp += 10;

			if (temp < 0)
				temp = 0;

			if (temp > 30)
				temp = 30;

			sprintf(temp_left_cov, "%d", temp);
			size = sprintf(data, "CLR 77 243 100 260 65535\n\r");
			HAL_UART_Transmit(&huart2, data, size, 100);

			size = sprintf(data, "UF 2 %d 243 0 %s\n\r", 89-strlen(temp_left_cov)*5, temp_left_cov);
			HAL_UART_Transmit(&huart2, data, size, 100);
		}
		break;

	case 5:
		temp = atoi(temp_left_eye);
		if (temp > 30) {
			size = sprintf(data, "BUZ 500 500\n\r");
			HAL_UART_Transmit(&huart2, data, size, 100);
			sprintf(temp_left_eye, "");
			size = sprintf(data, "CLR 114 152 135 168 65535\n\r");
			HAL_UART_Transmit(&huart2, data, size, 100);

		} else {
			if (option == 1)
				temp -= 1;
			if (option == 2)
				temp += 1;
			if (option == 3)
				temp -= 10;
			if (option == 4)
				temp += 10;

			if (temp < 0)
				temp = 0;

			if (temp > 30)
				temp = 30;

			sprintf(temp_left_eye, "%d", temp);
			size = sprintf(data, "CLR 114 152 135 168 65535\n\r");
			HAL_UART_Transmit(&huart2, data, size, 100);

			size = sprintf(data, "UF 2 %d 152 0 %s\n\r", 126-strlen(temp_left_eye)*5, temp_left_eye);
			HAL_UART_Transmit(&huart2, data, size, 100);
		}
		break;

	case 6:
		temp = atoi(temp_right_eye);
		if (temp > 30) {
			size = sprintf(data, "BUZ 500 500\n\r");
			HAL_UART_Transmit(&huart2, data, size, 100);
			sprintf(temp_right_eye, "");
			size = sprintf(data, "CLR 217 154 240 168 65535\n\r");
			HAL_UART_Transmit(&huart2, data, size, 100);

		} else {
			if (option == 1)
				temp -= 1;
			if (option == 2)
				temp += 1;
			if (option == 3)
				temp -= 10;
			if (option == 4)
				temp += 10;

			if (temp < 0)
				temp = 0;

			if (temp > 30)
				temp = 30;

			sprintf(temp_right_eye, "%d", temp);
			size = sprintf(data, "CLR 217 154 240 168 65535\n\r");
			HAL_UART_Transmit(&huart2, data, size, 100);

			size = sprintf(data, "UF 2 %d 152 0 %s\n\r", 230-strlen(temp_right_eye)*5, temp_right_eye);
			HAL_UART_Transmit(&huart2, data, size, 100);
		}
		break;

	case 7:
		temp = atoi(temp_right_cov);
		if (temp > 30) {
			size = sprintf(data, "BUZ 500 500\n\r");
			HAL_UART_Transmit(&huart2, data, size, 100);
			sprintf(temp_right_cov, "");
			size = sprintf(data, "CLR 257 243 280 260 65535\n\r");
			HAL_UART_Transmit(&huart2, data, size, 100);

		} else {
			if (option == 1)
				temp -= 1;
			if (option == 2)
				temp += 1;
			if (option == 3)
				temp -= 10;
			if (option == 4)
				temp += 10;

			if (temp < 0)
				temp = 0;

			if (temp > 30)
				temp = 30;

			sprintf(temp_right_cov, "%d", temp);
			size = sprintf(data, "CLR 257 243 280 260 65535\n\r");
			HAL_UART_Transmit(&huart2, data, size, 100);

			size = sprintf(data, "UF 2 %d 243 0 %s\n\r", 268-strlen(temp_right_cov)*5,temp_right_cov);
			HAL_UART_Transmit(&huart2, data, size, 100);

		}
		break;

	default:
		break;
	}
}

void screen4_save(){
	uint8_t name_repeat = 0;
	uint8_t temp_p = p;

	error = 0;

	for(int i = 0; i<ilosc_wezlow(L);i++){
		zwroc(L, i, &_name, &_pcs, &_pcs_done, &_length, &_left_cov, &_left_eye, &_right_eye,
						&_right_cov, &_knife, &_knife_move_back);

		if (i!=temp_p && strcmp(&temp_name, &_name)==0){
			size = sprintf(data, "nazwa1 %s nazwa2 %s \n\r", _name, temp_name);
				HAL_UART_Transmit(&huart1, data, size, 100);
			name_repeat=1;
			break;
		}

	}

	p = temp_p;
	zwroc(L, p, &_name, &_pcs, &_pcs_done, &_length, &_left_cov, &_left_eye, &_right_eye,
					&_right_cov, &_knife, &_knife_move_back);

	_pcs = atoi(temp_pcs);
	_pcs_done = atoi(temp_pcs_done);
	_length = atoi(temp_length);
	_left_cov = atoi(temp_left_cov);
	_left_eye = atoi(temp_left_eye);
	_right_eye = atoi(temp_right_eye);
	_right_cov = atoi(temp_right_cov);
	_knife = atoi(temp_knife);
	_knife_move_back = atoi(temp_knife_move_back);

	if (_pcs < 1) {
		size = sprintf(data, "BUZ 500 500\n\r");
		HAL_UART_Transmit(&huart2, data, size, 100);
		sprintf(temp_pcs, "");
		size = sprintf(data, "CLR 160 74 270 90 65535\n\r");
		HAL_UART_Transmit(&huart2, data, size, 100);

		size = sprintf(data, "LOAD 0 0 error.bmp\n\r");
		HAL_UART_Transmit(&huart2, data, size, 100);

		size = sprintf(data,
				"UF 3 60 15 65535 PCS value can't be less than 1\n\r");
		HAL_UART_Transmit(&huart2, data, size, 100);
		param_number = 2;
		max_size1 = 4;
		error = 1;

	} else if (_length < 50) {
		size = sprintf(data, "BUZ 500 500\n\r");
		HAL_UART_Transmit(&huart2, data, size, 100);
		sprintf(temp_length, "");
		size = sprintf(data, "CLR 140 114 220 131 65535\n\r");
		HAL_UART_Transmit(&huart2, data, size, 100);

		size = sprintf(data, "LOAD 0 0 error.bmp\n\r");
		HAL_UART_Transmit(&huart2, data, size, 100);

		size = sprintf(data,
				"UF 3 60 15 65535 length can't be less than 50mm\n\r");
		HAL_UART_Transmit(&huart2, data, size, 100);
		param_number = 3;
		max_size1 = 3;
		error = 1;

	} else if (((_left_cov + _right_cov) >= _length)) {
		size = sprintf(data, "BUZ 500 500\n\r");
		HAL_UART_Transmit(&huart2, data, size, 100);
		sprintf(temp_length, "");
		size = sprintf(data, "CLR 140 114 220 131 65535\n\r");
		HAL_UART_Transmit(&huart2, data, size, 100);

		size = sprintf(data, "LOAD 0 0 error.bmp\n\r");
		HAL_UART_Transmit(&huart2, data, size, 100);

		size = sprintf(data, "UF 3 60 15 65535 Wrong values\n\r");
		HAL_UART_Transmit(&huart2, data, size, 100);
		param_number = 3;
		max_size1 = 3;
		error = 1;

	}else if (strlen(temp_name)==0 || name_repeat==1 || temp_name[0] == ' '){
		size = sprintf(data, "BUZ 500 500\n\r");
		HAL_UART_Transmit(&huart2, data, size, 100);
		sprintf(temp_name, "");
		size = sprintf(data, "CLR 90 14 300 40 62882\n\r");
		HAL_UART_Transmit(&huart2, data, size, 100);
		size = sprintf(data, "LOAD 0 0 error.bmp\n\r");
		HAL_UART_Transmit(&huart2, data, size, 100);
		size = sprintf(data, "UF 3 60 15 65535 Wrong program name\n\r");
		HAL_UART_Transmit(&huart2, data, size, 100);
		param_number = 0; //jest 0, zamiast 1, aby użytkownik musiał kliknąć X by zamknąć komunikat o błędzie
		max_size1 = 13;
		error = 1;

	}	else {
		usun(&L, p);
		wstaw(&L, p, temp_name, _pcs, _pcs_done, _length, _left_cov, _left_eye,
				_right_eye, _right_cov, _knife, _knife_move_back);
		interrupt = 1;
		param_number = 0;
	}
}

