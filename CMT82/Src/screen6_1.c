/*
 * screen6_1.c
 *
 *  Created on: 18.11.2016
 *      Author: gorian
 *      Ekran: Zatrzymany proces przed ukończeniem wszystkich sztuk
 */
#include "screen6_1.h"
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

void screen6_1_init(uint8_t hours, uint8_t minutes) {

	zwroc(L, p, &_name, &_pcs, &_pcs_done, &_length, &_left_cov, &_left_eye, &_right_eye,
			&_right_cov, &_knife, &_knife_move_back);
	sprintf(temp_name, "%s", _name);
	sprintf(temp_pcs, "%d", _pcs);
	sprintf(temp_pcs_done, "%d", _pcs_done);

	size = sprintf(data, "LOAD 0 0 6a.bmp\n\r");
	HAL_UART_Transmit(&huart2, data, size, 10);

	size = sprintf(data, "Zaladowano ekran 6.1\n\r");
	HAL_UART_Transmit(&huart1, data, size, 10);

	size = sprintf(data, "BUZ 3000 1000\n\r");
	HAL_UART_Transmit(&huart2, data, size, 10);

	size = sprintf(data, "UF 5 %d 160 12710 Program \"%s\" stopped\n\r", 240-(strlen(temp_name)+18)*7, temp_name);
	HAL_UART_Transmit(&huart2, data, size, 1);

	size = sprintf(data, "UF 2 %d 195 40179 %s\/%s segments at %0.2d:%0.2d\n\r",240-(strlen(temp_pcs)+strlen(temp_pcs_done)+18)*5, temp_pcs_done, temp_pcs, hours, minutes);
	HAL_UART_Transmit(&huart2, data, size, 1);

	screen = 61;	//numer ekranu do wyświetlenia
}

int screen6_1_button() {
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

void screen6_1_action(int button) {

	switch (button) {

	case 1:
		screen = 2;
		action = 1;
		break;

	default:
		break;
	}
}
