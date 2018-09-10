/*
 * screen16.c
 *
 *  Created on: 14.08.2018
 *      Author: gorian
 *      Ekran: ustawienia maszyny
 */
#include "screen16.h"

extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;
extern uint8_t screen;
extern uint16_t size;
extern uint16_t position_x;
extern uint16_t position_y;
extern uint8_t action;
extern uint8_t interrupt;

extern uint8_t data[50];
uint8_t feed_speed;
uint8_t blade_speed;
uint16_t batch_pieces;
uint8_t batch_delay;
float scaling;

char *temp_batch_pieces[4];
char *temp_scaling[5];

uint16_t prescaler_array[10]={0, 999, 710, 740, 723 , 795, 740, 740, 805, 825 ,719};
uint16_t period_array[10] ={0, 17, 22, 19, 18, 15, 15, 14, 12, 11, 12};


void screen16_init() {

	sprintf(temp_batch_pieces, "%d", batch_pieces);
	sprintf(temp_scaling, "%.1f", scaling);


	size = sprintf(data, "LOAD 0 0 16.bmp\n\r");
	HAL_UART_Transmit(&huart2, data, size, 100);

	size = sprintf(data, "UF 3 %d 15 65535 Settings\n\r", 240-(8*5));
	HAL_UART_Transmit(&huart2, data, size, 1000);

	size = sprintf(data, "Zaladowano ekran 16\n\r");
	HAL_UART_Transmit(&huart1, data, size, 1000);

	size = sprintf(data, "UF 3 60 %d 0 Feed speed\n\r", 59+37*0);
	HAL_UART_Transmit(&huart2, data, size, 100);

	size = sprintf(data, "LOAD 240 %d minus.bmp\n\r", 55+37*0);
	HAL_UART_Transmit(&huart2, data, size, 100);

	size = sprintf(data, "UF 3 327 %d 0 %d\n\r", 59+37*0, feed_speed);
	HAL_UART_Transmit(&huart2, data, size, 100);

	size = sprintf(data, "LOAD 394 %d plus.bmp\n\r", 55+37*0);
	HAL_UART_Transmit(&huart2, data, size, 100);

	size = sprintf(data, "UF 3 60 %d 0 Blade speed\n\r", 59+37*1);
	HAL_UART_Transmit(&huart2, data, size, 100);

	size = sprintf(data, "LOAD 240 %d minus.bmp\n\r", 55+37*1);
	HAL_UART_Transmit(&huart2, data, size, 100);

	size = sprintf(data, "UF 3 327 %d 0 %d\n\r", 59+37*1, blade_speed);
	HAL_UART_Transmit(&huart2, data, size, 100);

	size = sprintf(data, "LOAD 394 %d plus.bmp\n\r", 55+37*1);
	HAL_UART_Transmit(&huart2, data, size, 100);

	size = sprintf(data, "UF 3 60 %d 0 Batch pieces\n\r", 59+37*2);
	HAL_UART_Transmit(&huart2, data, size, 100);

	size = sprintf(data, "LOAD 240 %d minus.bmp\n\r", 55+37*2);
	HAL_UART_Transmit(&huart2, data, size, 100);

	size = sprintf(data, "UF 3 %d %d 0 %d\n\r", 327-strlen(temp_batch_pieces)*5/2, 59+37*2, batch_pieces);
	HAL_UART_Transmit(&huart2, data, size, 100);

	size = sprintf(data, "LOAD 394 %d plus.bmp\n\r", 55+37*2);
	HAL_UART_Transmit(&huart2, data, size, 100);

	size = sprintf(data, "UF 3 60 %d 0 Batch delay\n\r", 59+37*3);
	HAL_UART_Transmit(&huart2, data, size, 100);

	size = sprintf(data, "LOAD 240 %d minus.bmp\n\r", 55+37*3);
	HAL_UART_Transmit(&huart2, data, size, 100);

	size = sprintf(data, "UF 3 327 %d 0 %d\n\r", 59+37*3, batch_delay);
	HAL_UART_Transmit(&huart2, data, size, 100);

	size = sprintf(data, "LOAD 394 %d plus.bmp\n\r", 55+37*3);
	HAL_UART_Transmit(&huart2, data, size, 100);

	size = sprintf(data, "UF 3 60 %d 0 Length correction\n\r", 59+37*4);
	HAL_UART_Transmit(&huart2, data, size, 100);

	size = sprintf(data, "LOAD 240 %d minus.bmp\n\r", 55+37*4);
	HAL_UART_Transmit(&huart2, data, size, 100);

	size = sprintf(data, "UF 3 %d %d 0 %.1f\n\r", 327-strlen(temp_scaling)*5/2, 59+37*4, scaling);
	HAL_UART_Transmit(&huart2, data, size, 100);

	size = sprintf(data, "LOAD 394 %d plus.bmp\n\r", 55+37*4);
	HAL_UART_Transmit(&huart2, data, size, 100);

//	screen16_table(0);
	screen = 16;
}

int screen16_button() {

	if (((0 < position_x) && (position_x < 45))
			&& ((0 < position_y) && (position_y < 45))) {
		size = sprintf(data, "BUZ 150 2000\n\r");
		HAL_UART_Transmit(&huart2, data, size, 100);
		return 1;
	}

//	if (((0 < position_x) && (position_x < 45))
//			&& ((45 < position_y) && (position_y < 90)) && list_position >1) {
//		size = sprintf(data, "BUZ 150 2000\n\r");
//		HAL_UART_Transmit(&huart2, data, size, 100);
//
//		size = sprintf(data, "LOAD 0 0 16.bmp\n\r");
//		HAL_UART_Transmit(&huart2, data, size, 100);
//
//		if(list_position-1==1){
//			size = sprintf(data, "CLR 453 57 479 94 65535\n\r");
//			HAL_UART_Transmit(&huart2, data, size, 100);
//		}
//
//		return 2;
//	}

//	if (((0 < position_x) && (position_x < 45))
//			&& ((227 < position_y) && (position_y < 272)) && list_position < (max_nodes/6)+footer ) {
//		size = sprintf(data, "BUZ 150 2000\n\r");
//		HAL_UART_Transmit(&huart2, data, size, 100);
//
//		size = sprintf(data, "LOAD 0 0 3.bmp\n\r");
//		HAL_UART_Transmit(&huart2, data, size, 100);
//
//
//		size = sprintf(data, "CLR 453 %d 479 270 65535\n\r", 57+37*(6-(list_position+1)*6+max_nodes));
//		HAL_UART_Transmit(&huart2, data, size, 100);
//		return 3;
//	}

	if (((240 < position_x) && (position_x < 285))
			&& ((55+37*0 < position_y) && (position_y < 55+37*1))) {
		size = sprintf(data, "BUZ 150 2000\n\r");
		HAL_UART_Transmit(&huart2, data, size, 100);
		return 4;
	}

	if (((394 < position_x) && (position_x < 425))
			&& ((55+37*0 < position_y) && (position_y < 55+37*1))) {
		size = sprintf(data, "BUZ 150 2000\n\r");
		HAL_UART_Transmit(&huart2, data, size, 100);
		return 5;
	}

	if (((240 < position_x) && (position_x < 285))
			&& ((55+37*1 < position_y) && (position_y < 55+37*2))) {
		size = sprintf(data, "BUZ 150 2000\n\r");
		HAL_UART_Transmit(&huart2, data, size, 100);
		return 6;
	}

	if (((394 < position_x) && (position_x < 425))
			&& ((55+37*1 < position_y) && (position_y < 55+37*2))) {
		size = sprintf(data, "BUZ 150 2000\n\r");
		HAL_UART_Transmit(&huart2, data, size, 100);
		return 7;
	}

	if (((240 < position_x) && (position_x < 285))
			&& ((55+37*2 < position_y) && (position_y < 55+37*3))) {
		size = sprintf(data, "BUZ 150 2000\n\r");
		HAL_UART_Transmit(&huart2, data, size, 100);
		return 8;
	}

	if (((394 < position_x) && (position_x < 425))
			&& ((55+37*2 < position_y) && (position_y < 55+37*3))) {
		size = sprintf(data, "BUZ 150 2000\n\r");
		HAL_UART_Transmit(&huart2, data, size, 100);
		return 9;
	}

	if (((240 < position_x) && (position_x < 285))
			&& ((55+37*3 < position_y) && (position_y < 55+37*4))) {
		size = sprintf(data, "BUZ 150 2000\n\r");
		HAL_UART_Transmit(&huart2, data, size, 100);
		return 10;
	}

	if (((394 < position_x) && (position_x < 425))
			&& ((55+37*3 < position_y) && (position_y < 55+37*4))) {
		size = sprintf(data, "BUZ 150 2000\n\r");
		HAL_UART_Transmit(&huart2, data, size, 100);
		return 11;
	}

	if (((240 < position_x) && (position_x < 285))
			&& ((55+37*4 < position_y) && (position_y < 55+37*5))) {
		size = sprintf(data, "BUZ 150 2000\n\r");
		HAL_UART_Transmit(&huart2, data, size, 100);
		return 12;
	}

	if (((394 < position_x) && (position_x < 425))
			&& ((55+37*4 < position_y) && (position_y < 55+37*5))) {
		size = sprintf(data, "BUZ 150 2000\n\r");
		HAL_UART_Transmit(&huart2, data, size, 100);
		return 13;
	}

	position_x = 0;
	position_y = 0;
	return 0;
	//0 - puste miejsce
	//1 - powrót do ekranu 2 (wyjście)
	//2 - w górę
	//3 - w dół

	//4 - feed speed -
	//5 - feed speed +
	//6 - blade speed -
	//7 - blade speed +
	//8 - batch pieces -
	//9 - batch pieces +
	//10 - batch delay -
	//11 - batch delay +
	//12 - scaling -
	//13 - scaling +
}

void screen16_action(int button) {

	uint8_t *temp_array[2];

	sprintf(temp_batch_pieces, "%d", batch_pieces);
	sprintf(temp_scaling, "%.1f", scaling);

	switch (button) {

	case 1:
		interrupt = 4;
		screen = 2;
		action = 1;
		break;

//	case 2:
//		screen3_table(1);
//		break;
//
//	case 3:
//		screen3_table(2);
//		break;


	case 4:
		if(feed_speed>1){
			feed_speed--;
			size = sprintf(data, "CLR 285 %d 380 %d 65535\n\r",55+37*0, 50+37*1);
			HAL_UART_Transmit(&huart2, data, size, 1000);
			size = sprintf(data, "UF 3 %d %d 0 %d\n\r", 327-5/2, 59+37*0, feed_speed);
			HAL_UART_Transmit(&huart2, data, size, 100);
		}
		break;

	case 5:
		if(feed_speed<9){
			feed_speed++;
			size = sprintf(data, "CLR 285 %d 380 %d 65535\n\r",55+37*0, 50+37*1);
			HAL_UART_Transmit(&huart2, data, size, 1000);
			size = sprintf(data, "UF 3 %d %d 0 %d\n\r", 327-5/2, 59+37*0, feed_speed);
			HAL_UART_Transmit(&huart2, data, size, 100);
		}
		break;

	case 6:
		if(blade_speed>1){
			blade_speed--;
			size = sprintf(data, "CLR 285 %d 380 %d 65535\n\r",55+37*1, 50+37*2);
			HAL_UART_Transmit(&huart2, data, size, 1000);
			size = sprintf(data, "UF 3 %d %d 0 %d\n\r", 327-5/2, 59+37*1, blade_speed);
			HAL_UART_Transmit(&huart2, data, size, 100);
		}
		break;

	case 7:
		if(blade_speed<9){
			blade_speed++;
			size = sprintf(data, "CLR 285 %d 380 %d 65535\n\r",55+37*1, 50+37*2);
			HAL_UART_Transmit(&huart2, data, size, 1000);
			size = sprintf(data, "UF 3 %d %d 0 %d\n\r", 327-5/2, 59+37*1, blade_speed);
			HAL_UART_Transmit(&huart2, data, size, 100);
		}
		break;

	case 8:
		if(batch_pieces>4){
			batch_pieces-=5;
			size = sprintf(data, "CLR 285 %d 380 %d 65535\n\r",55+37*2, 50+37*3);
			HAL_UART_Transmit(&huart2, data, size, 1000);
			size = sprintf(data, "UF 3 %d %d 0 %d\n\r", 327-strlen(temp_batch_pieces)*5/2, 59+37*2, batch_pieces);
			HAL_UART_Transmit(&huart2, data, size, 100);
		}
		break;

	case 9:
		if(batch_pieces<996){
			batch_pieces+=5;
			size = sprintf(data, "CLR 285 %d 380 %d 65535\n\r",55+37*2, 50+37*3);
			HAL_UART_Transmit(&huart2, data, size, 100);
			size = sprintf(data, "UF 3 %d %d 0 %d\n\r", 327-strlen(temp_batch_pieces)*5/2, 59+37*2, batch_pieces);
			HAL_UART_Transmit(&huart2, data, size, 100);
		}
		break;


	case 10:
		if(batch_delay>0){
			batch_delay--;
			size = sprintf(data, "CLR 285 %d 380 %d 65535\n\r",55+37*3, 50+37*4);
			HAL_UART_Transmit(&huart2, data, size, 1000);
			size = sprintf(data, "UF 3 %d %d 0 %d\n\r", 327-5/2, 59+37*3, batch_delay);
			HAL_UART_Transmit(&huart2, data, size, 100);
		}
		break;
// 9 9 620 165 56929.9
	case 11:
		if(batch_delay<9){
			batch_delay++;
			size = sprintf(data, "CLR 285 %d 380 %d 65535\n\r",55+37*3, 50+37*4);
			HAL_UART_Transmit(&huart2, data, size, 1000);
			size = sprintf(data, "UF 3 %d %d 0 %d\n\r", 327-5/2, 59+37*3, batch_delay);
			HAL_UART_Transmit(&huart2, data, size, 100);
		}
		break;

	case 12:
		if(scaling>90.1){
			scaling-=0.1;
			size = sprintf(data, "CLR 285 %d 380 %d 65535\n\r",55+37*4, 45+37*5);
			HAL_UART_Transmit(&huart2, data, size, 1000);
			size = sprintf(data, "UF 3 %d %d 0 %.1f\n\r", 327-strlen(temp_scaling)*5/2, 59+37*4, scaling);
			HAL_UART_Transmit(&huart2, data, size, 100);
		}
		break;

	case 13:
		if(scaling<110){
			scaling+=0.1;
			size = sprintf(data, "CLR 285 %d 380 %d 65535\n\r",55+37*4, 45+37*5);
			HAL_UART_Transmit(&huart2, data, size, 1000);
			size = sprintf(data, "UF 3 %d %d 0 %.1f\n\r", 327-strlen(temp_scaling)*5/2, 59+37*4, scaling);
			HAL_UART_Transmit(&huart2, data, size, 100);
		}
		break;

	default:
		break;
	}
}

void screen16_table(int dir) {
//	temp_position  = 0;
////	if (list_position == 1)
////		temp_position=1;
//
//	if (dir == 0) {
//		list_position = 1;
//
//
//	} else if (dir == 1) {
//		size = sprintf(data, "BUZ 150 2000\n\r");
//		HAL_UART_Transmit(&huart2, data, size, 100);
//		--list_position;
//
//	} else if (dir == 2) {
//		size = sprintf(data, "BUZ 150 2000\n\r");
//		HAL_UART_Transmit(&huart2, data, size, 100);
//		++list_position;
//	}
//
//	for(s=0; s<6; s++){
//		if(s<5){
//			size = sprintf(data, "CLR 58 %d 169 %d 65535\n\r", 57+37*s, 87+35*s);
//			HAL_UART_Transmit(&huart2, data, size, 100);
//
//
//		}else{
//			size = sprintf(data, "CLR 58 %d 169 %d 65535\n\r", 57+37*s, 270);
//			HAL_UART_Transmit(&huart2, data, size, 100);
//
//		}
//
////		size = sprintf(data, " l_position %d nasz s %d nasze t_pos %d \n\r", list_position, s, temp_position);
////					HAL_UART_Transmit(&huart1, data, size, 100);
//
//		if(6*(list_position-1)+s < max_nodes){
//			zwroc(L, 6*(list_position-1)+s, &_name, &_pcs, &_pcs_done, &_length, &_left_cov, &_left_eye, &_right_eye,
//				&_right_cov, &_knife, &_knife_move_back);
//			sprintf(temp_name, "%s", _name);
//			size = sprintf(data, "UF 3 60 %d 0 %s\n\r", 59+37*s, temp_name);
//			HAL_UART_Transmit(&huart2, data, size, 100);
//
////			size = sprintf(data, "nasze p %d nasza nazwa %s nasze max %d \n\r", p, temp_name, ilosc_wezlow(L));
////			HAL_UART_Transmit(&huart1, data, size, 100);
//		}
//	}

}
