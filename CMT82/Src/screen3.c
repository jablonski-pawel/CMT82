/*
 * screen3.c
 *
 *  Created on: 18.11.2016
 *      Author: gorian
 *      Ekran: lista programów
 */
#include "screen3.h"
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

char temp_name[18];
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
uint16_t local_p = 0; //numer aktualnie wyświetlanego programu na liście
uint16_t max_nodes = 0;	//maksymalna liczba węzłów

extern uint8_t node_to_delete;

uint8_t temp_position =0;
uint8_t list_position = 0;
uint8_t s = 0;


void screen3_init() {
	max_nodes = ilosc_wezlow(L);

	size = sprintf(data, "LOAD 0 0 3.bmp\n\r");
	HAL_UART_Transmit(&huart2, data, size, 100);

	size = sprintf(data, "Zaladowano ekran 3\n\r");
	HAL_UART_Transmit(&huart1, data, size, 1000);

	size = sprintf(data, "CLR 453 57 479 94 65535\n\r");
	HAL_UART_Transmit(&huart2, data, size, 100);

	if(max_nodes < 6){
		size = sprintf(data, "CLR 453 %d 479 270 65535\n\r", 57+37*(6-(6-max_nodes)));
		HAL_UART_Transmit(&huart2, data, size, 100);
		HAL_UART_Transmit(&huart1, data, size, 100);
	}
	screen3_table(0);
	screen = 3;
}

int screen3_button() {
	uint8_t footer = 0;
	if(max_nodes%6 != 0)
		footer = 1;

	if (((0 < position_x) && (position_x < 45))
			&& ((0 < position_y) && (position_y < 45))) {
		size = sprintf(data, "BUZ 150 2000\n\r");
		HAL_UART_Transmit(&huart2, data, size, 100);
		return 1;
	}

	if (((0 < position_x) && (position_x < 45))
			&& ((45 < position_y) && (position_y < 90)) && list_position >1) {
		size = sprintf(data, "BUZ 150 2000\n\r");
		HAL_UART_Transmit(&huart2, data, size, 100);

		size = sprintf(data, "LOAD 0 0 3.bmp\n\r");
		HAL_UART_Transmit(&huart2, data, size, 100);

		if(list_position-1==1){
			size = sprintf(data, "CLR 453 57 479 94 65535\n\r");
			HAL_UART_Transmit(&huart2, data, size, 100);
		}

		return 2;
	}
	size = sprintf(data, "LP: %d, nodes: %d, footer: %d: max node: %d\n\r", list_position, (max_nodes/6)+footer, footer, max_nodes);
			HAL_UART_Transmit(&huart1, data, size, 100);
	if (((0 < position_x) && (position_x < 45))
			&& ((227 < position_y) && (position_y < 272)) && list_position < (max_nodes/6)+footer ) {
		size = sprintf(data, "BUZ 150 2000\n\r");
		HAL_UART_Transmit(&huart2, data, size, 100);

		size = sprintf(data, "LOAD 0 0 3.bmp\n\r");
		HAL_UART_Transmit(&huart2, data, size, 100);


		size = sprintf(data, "CLR 453 %d 479 270 65535\n\r", 57+37*(6-(list_position+1)*6+max_nodes));
		HAL_UART_Transmit(&huart2, data, size, 100);
		return 3;
	}

	for (int i=0; i<6;i++){
		//wybór elementu z listy
		if (((50 < position_x) && (position_x < 430))
				&& ((57+37*i < position_y) && (position_y < 87+35*i)) && list_position < max_nodes/6+footer) {
			size = sprintf(data, "BUZ 150 2000\n\r");
			HAL_UART_Transmit(&huart2, data, size, 100);
			p=6*(list_position-1)+i+temp_position;
			return 1;
		}else if (((50 < position_x) && (position_x < 430))
				&& ((57+37*i < position_y) && (position_y < 87+35*i))){
			if(i<6-list_position*6+max_nodes){
				size = sprintf(data, "BUZ 150 2000\n\r");
					HAL_UART_Transmit(&huart2, data, size, 100);
				p=6*(list_position-1)+i+temp_position;
				return 1;
			}else{
				return 0;
			}
		}

		//wybór śmietnika
		if (((430 < position_x) && (position_x < 480))
				&& ((57+37*i < position_y) && (position_y < 87+35*i)) && list_position < max_nodes/6+footer) {

			if(list_position==1 && i==0) return 0;

			size = sprintf(data, "BUZ 150 2000\n\r");
			HAL_UART_Transmit(&huart2, data, size, 100);
			node_to_delete = 6*(list_position-1)+i+temp_position; // numer elementu do usunięcia
			screen = 15;
			action = 1;

			return 0;

		}else if (((430 < position_x) && (position_x < 480))
				&& ((57+37*i < position_y) && (position_y < 87+35*i))){
			if(i<6-list_position*6+max_nodes){

				if(list_position==1 && i==0) return 0;

				size = sprintf(data, "BUZ 150 2000\n\r");
				HAL_UART_Transmit(&huart2, data, size, 100);
				node_to_delete = 6*(list_position-1)+i+temp_position;
				screen = 15;
				action = 1;

				return 0;
			}else{
				return 0;
			}
		}

	}

	position_x = 0;
	position_y = 0;
	return 0;
	//0 - puste miejsce
	//1 - powrót do ekranu 2 (wyjście)
	//2 - w górę
	//3 - w dół

}

void screen3_action(int button) {

	switch (button) {

	case 1:
//		size = sprintf(data, "P tuz po wyborze %d\n\r", p);
//		HAL_UART_Transmit(&huart1, data, size, 100);
		screen = 2;
		action = 1;
		break;

	case 2:
		screen3_table(1);
		break;

	case 3:
		screen3_table(2);
		break;

	default:
		break;
	}
}

void screen3_table(int dir) {
	temp_position  = 0;
//	if (list_position == 1)
//		temp_position=1;

	if (dir == 0) {
		list_position = 1;


	} else if (dir == 1) {
		size = sprintf(data, "BUZ 150 2000\n\r");
		HAL_UART_Transmit(&huart2, data, size, 100);
		--list_position;

	} else if (dir == 2) {
		size = sprintf(data, "BUZ 150 2000\n\r");
		HAL_UART_Transmit(&huart2, data, size, 100);
		++list_position;
	}

	for(s=0; s<6; s++){
		if(s<5){
			size = sprintf(data, "CLR 58 %d 169 %d 65535\n\r", 57+37*s, 87+35*s);
			HAL_UART_Transmit(&huart2, data, size, 100);


		}else{
			size = sprintf(data, "CLR 58 %d 169 %d 65535\n\r", 57+37*s, 270);
			HAL_UART_Transmit(&huart2, data, size, 100);

		}

//		size = sprintf(data, " l_position %d nasz s %d nasze t_pos %d \n\r", list_position, s, temp_position);
//					HAL_UART_Transmit(&huart1, data, size, 100);

		if(6*(list_position-1)+s < max_nodes){
			zwroc(L, 6*(list_position-1)+s, &_name, &_pcs, &_pcs_done, &_length, &_left_cov, &_left_eye, &_right_eye,
				&_right_cov, &_knife, &_knife_move_back);
			sprintf(temp_name, "%s", _name);
			size = sprintf(data, "UF 3 60 %d 0 %s\n\r", 59+37*s, temp_name);
			HAL_UART_Transmit(&huart2, data, size, 100);

//			size = sprintf(data, "nasze p %d nasza nazwa %s nasze max %d \n\r", p, temp_name, ilosc_wezlow(L));
//			HAL_UART_Transmit(&huart1, data, size, 100);
		}
	}

}
