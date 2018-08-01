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
// zegarek
//			size = sprintf(data, "CLR 0 0 479 45 405441\n\r");
//			HAL_UART_Transmit(&huart2, data, size, 1);
//
//			size = sprintf(data, "UF 5 405 10 65535 %0.2d:%0.2d\n\r", hours, minutes);
//			HAL_UART_Transmit(&huart2, data, size, 1);

			size = sprintf(data, "UF 3 10 15 65535 Program \"%s\" in progress\n\r", temp_name);
			HAL_UART_Transmit(&huart2, data, size, 1);



			size = sprintf(data, "UF 2 %d 114 0 %s\n\r",240-(strlen(temp_length)*5), temp_length);
			HAL_UART_Transmit(&huart2, data, size, 1000);

			size = sprintf(data, "UF 2 %d 152 0 %s\n\r", 184-strlen(temp_left_eye)*5, temp_left_eye);
			HAL_UART_Transmit(&huart2, data, size, 1000);

			size = sprintf(data, "UF 2 %d 152 0 %s\n\r", 288-strlen(temp_right_eye)*5, temp_right_eye);
			HAL_UART_Transmit(&huart2, data, size, 1000);

			size = sprintf(data, "UF 2 %d 243 0 %s\n\r", 147-strlen(temp_left_cov)*5, temp_left_cov);
			HAL_UART_Transmit(&huart2, data, size, 1000);

			size = sprintf(data, "UF 2 %d 243 0 %s\n\r", 326-strlen(temp_right_cov)*5,temp_right_cov);
			HAL_UART_Transmit(&huart2, data, size, 1000);
		}

		size = sprintf(data, "CLR 150 73 300 90 65535\n\rUF 2 %d 72 0 %s\/%s PCS\n\r", 240-(strlen(temp_pcs)+strlen(temp_pcs_done)+5)*5, temp_pcs_done, temp_pcs);
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
	}

	screen = 6;
}

int screen6_button() {

	if (((0 < position_x) && (position_x < 45))
			&& ((0 < position_y) && (position_y < 45)) && start == 0) {
		size = sprintf(data, "BUZ 150 2000\n\r");
		HAL_UART_Transmit(&huart2, data, size, 100);
		return 1;
	}

	position_x = 0;
	position_y = 0;
	return 0;
	//0 - puste miejsce
	//1 - przycisk menu
}

void screen6_action(int button) {

	switch (button) {

	case 1:
		screen = 2;
		action = 1;
		break;

	default:
		action = 0;
		break;
	}

}
