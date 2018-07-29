/*
 * screen12.c
 *
 *  Created on: 20.11.2016
 *      Author: gorian
 *      Ekran: Tabela AWG
 */
#include "screen12.h"

extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;
extern uint8_t screen;
extern uint16_t size;
extern uint16_t position_x;
extern uint16_t position_y;
extern uint8_t action;

extern uint8_t data[50];
uint8_t list_counter = 0;

void screen12_init() {

	size = sprintf(data, "LOAD 0 0 12.bmp\n\r");
	HAL_UART_Transmit(&huart2, data, size, 100);

	size = sprintf(data, "Zaladowano ekran 12\n\r");
	HAL_UART_Transmit(&huart1, data, size, 1000);

	screen12_AWG_struct_fill();
	screen12_AWG_table(0);
	screen = 12;
}

int screen12_button() {
	if (((0 < position_x) && (position_x < 45))
			&& ((0 < position_y) && (position_y < 45))) {
		size = sprintf(data, "BUZ 150 2000\n\r");
		HAL_UART_Transmit(&huart2, data, size, 100);
		return 1;
	}

	if (((0 < position_x) && (position_x < 45))
			&& ((45 < position_y) && (position_y < 90))) {
		size = sprintf(data, "BUZ 150 2000\n\r");
		HAL_UART_Transmit(&huart2, data, size, 100);
		return 2;
	}

	if (((0 < position_x) && (position_x < 45))
			&& ((227 < position_y) && (position_y < 272))) {
		size = sprintf(data, "BUZ 150 2000\n\r");
		HAL_UART_Transmit(&huart2, data, size, 100);
		return 3;
	}

	position_x = 0;
	position_y = 0;
	return 0;
	//0 - puste miejsce
	//1 - powrót do ekranu 2 (wyjście)
	//2 - w górę
	//3 - w dół

}

void screen12_action(int button) {

	switch (button) {

	case 1:
		screen = 2;
		action = 1;
		break;

	case 2:
		screen12_AWG_table(1);
		break;

	case 3:
		screen12_AWG_table(2);
		break;

	default:
		break;
	}
}

void screen12_AWG_struct_fill() {
	awg_struct[0].awg_no = 8;
	strcpy(awg_struct[0].dia, "3.264");
	strcpy(awg_struct[0].sect, "8.37");

	awg_struct[1].awg_no = 9;
	strcpy(awg_struct[1].dia, "2.906");
	strcpy(awg_struct[1].sect, "6.63");

	awg_struct[2].awg_no = 10;
	strcpy(awg_struct[2].dia, "2.588");
	strcpy(awg_struct[2].sect, "5.26");

	awg_struct[3].awg_no = 11;
	strcpy(awg_struct[3].dia, "2.305");
	strcpy(awg_struct[3].sect, "4.17");

	awg_struct[4].awg_no = 12;
	strcpy(awg_struct[4].dia, "2.053");
	strcpy(awg_struct[4].sect, "3.31");

	awg_struct[5].awg_no = 13;
	strcpy(awg_struct[5].dia, "1.828");
	strcpy(awg_struct[5].sect, "2.62");

	awg_struct[6].awg_no = 14;
	strcpy(awg_struct[6].dia, "1.628");
	strcpy(awg_struct[6].sect, "2.08");

	awg_struct[7].awg_no = 15;
	strcpy(awg_struct[7].dia, "1.450");
	strcpy(awg_struct[7].sect, "1.65");

	awg_struct[8].awg_no = 16;
	strcpy(awg_struct[8].dia, "1.291");
	strcpy(awg_struct[8].sect, "1.31");

	awg_struct[9].awg_no = 17;
	strcpy(awg_struct[9].dia, "1.150");
	strcpy(awg_struct[9].sect, "1.04");

	awg_struct[10].awg_no = 18;
	strcpy(awg_struct[10].dia, "1.024");
	strcpy(awg_struct[10].sect, "0.823");

	awg_struct[11].awg_no = 19;
	strcpy(awg_struct[11].dia, "0.912");
	strcpy(awg_struct[11].sect, "0.653");

	awg_struct[12].awg_no = 20;
	strcpy(awg_struct[12].dia, "0.812");
	strcpy(awg_struct[12].sect, "0.518");

	awg_struct[13].awg_no = 21;
	strcpy(awg_struct[13].dia, "0.723");
	strcpy(awg_struct[13].sect, "0.410");

	awg_struct[14].awg_no = 22;
	strcpy(awg_struct[14].dia, "0.643");
	strcpy(awg_struct[14].sect, "0.324");

	awg_struct[15].awg_no = 23;
	strcpy(awg_struct[15].dia, "0.573");
	strcpy(awg_struct[15].sect, "0.258");

	awg_struct[16].awg_no = 24;
	strcpy(awg_struct[16].dia, "0.511");
	strcpy(awg_struct[16].sect, "0.205");

	awg_struct[17].awg_no = 25;
	strcpy(awg_struct[17].dia, "0.455");
	strcpy(awg_struct[17].sect, "0.162");

	awg_struct[18].awg_no = 26;
	strcpy(awg_struct[18].dia, "0.405");
	strcpy(awg_struct[18].sect, "0.129");

	awg_struct[19].awg_no = 27;
	strcpy(awg_struct[19].dia, "0.361");
	strcpy(awg_struct[19].sect, "0.102");

	awg_struct[20].awg_no = 28;
	strcpy(awg_struct[20].dia, "0.321");
	strcpy(awg_struct[20].sect, "0.0810");

	awg_struct[21].awg_no = 29;
	strcpy(awg_struct[21].dia, "0.286");
	strcpy(awg_struct[21].sect, "0.0642");

	awg_struct[22].awg_no = 30;
	strcpy(awg_struct[22].dia, "0.255");
	strcpy(awg_struct[22].sect, "0.0509");

	awg_struct[23].awg_no = 31;
	strcpy(awg_struct[23].dia, "0.227");
	strcpy(awg_struct[23].sect, "0.0404");

	awg_struct[24].awg_no = 32;
	strcpy(awg_struct[24].dia, "0.202");
	strcpy(awg_struct[24].sect, "0.0320");

}

void screen12_AWG_table(int dir) {
	if (dir == 0) {
		list_counter = 0;
	} else if (dir == 1) {
		size = sprintf(data, "BUZ 150 2000\n\r");
		HAL_UART_Transmit(&huart2, data, size, 100);

		if (list_counter >= 5)
			list_counter -= 5;


			//list_counter = 0;
	} else if (dir == 2) {
		size = sprintf(data, "BUZ 150 2000\n\r");
		HAL_UART_Transmit(&huart2, data, size, 100);
		list_counter += 5;
		if (list_counter > 20)
			list_counter = 20;
	}

	size = sprintf(data, "CLR 58 89 169 124 65535\n\r");
	HAL_UART_Transmit(&huart2, data, size, 100);

	size = sprintf(data, "UF 3 60 96 0 %d\n\r",
			awg_struct[list_counter].awg_no);
	HAL_UART_Transmit(&huart2, data, size, 100);

	size = sprintf(data, "CLR 58 126 169 162 65535\n\r");
	HAL_UART_Transmit(&huart2, data, size, 100);

	size = sprintf(data, "UF 3 60 134 0 %d\n\r",
			awg_struct[list_counter + 1].awg_no);
	HAL_UART_Transmit(&huart2, data, size, 100);

	size = sprintf(data, "CLR 58 164 169 200 65535\n\r");
	HAL_UART_Transmit(&huart2, data, size, 100);

	size = sprintf(data, "UF 3 60 172 0 %d\n\r",
			awg_struct[list_counter + 2].awg_no);
	HAL_UART_Transmit(&huart2, data, size, 100);

	size = sprintf(data, "CLR 58 202 169 236 65535\n\r");
	HAL_UART_Transmit(&huart2, data, size, 100);

	size = sprintf(data, "UF 3 60 208 0 %d\n\r",
			awg_struct[list_counter + 3].awg_no);
	HAL_UART_Transmit(&huart2, data, size, 100);

	size = sprintf(data, "CLR 58 238 169 271 65535\n\r");
	HAL_UART_Transmit(&huart2, data, size, 100);

	size = sprintf(data, "UF 3 60 244 0 %d\n\r",
			awg_struct[list_counter + 4].awg_no);
	HAL_UART_Transmit(&huart2, data, size, 100);




	size = sprintf(data, "CLR 179 89 312 124 65535\n\r");
	HAL_UART_Transmit(&huart2, data, size, 100);

	size = sprintf(data, "UF 3 185 96 0 %s\n\r",
			awg_struct[list_counter].dia);
	HAL_UART_Transmit(&huart2, data, size, 100);

	size = sprintf(data, "CLR 179 126 312 162 65535\n\r");
	HAL_UART_Transmit(&huart2, data, size, 100);

	size = sprintf(data, "UF 3 185 134 0 %s\n\r",
			awg_struct[list_counter + 1].dia);
	HAL_UART_Transmit(&huart2, data, size, 100);

	size = sprintf(data, "CLR 179 164 312 200 65535\n\r");
	HAL_UART_Transmit(&huart2, data, size, 100);

	size = sprintf(data, "UF 3 185 172 0 %s\n\r",
			awg_struct[list_counter + 2].dia);
	HAL_UART_Transmit(&huart2, data, size, 100);

	size = sprintf(data, "CLR 179 202 312 236 65535\n\r");
	HAL_UART_Transmit(&huart2, data, size, 100);

	size = sprintf(data, "UF 3 185 208 0 %s\n\r",
			awg_struct[list_counter + 3].dia);
	HAL_UART_Transmit(&huart2, data, size, 100);

	size = sprintf(data, "CLR 179 238 312 271 65535\n\r");
	HAL_UART_Transmit(&huart2, data, size, 100);

	size = sprintf(data, "UF 3 185 244 0 %s\n\r",
			awg_struct[list_counter + 4].dia);
	HAL_UART_Transmit(&huart2, data, size, 100);




	size = sprintf(data, "CLR 314 89 451 124 65535\n\r");
	HAL_UART_Transmit(&huart2, data, size, 100);

	size = sprintf(data, "UF 3 326 96 0 %s\n\r",
			awg_struct[list_counter].sect);
	HAL_UART_Transmit(&huart2, data, size, 100);

	size = sprintf(data, "CLR 314 126 451 162 65535\n\r");
	HAL_UART_Transmit(&huart2, data, size, 100);

	size = sprintf(data, "UF 3 326 134 0 %s\n\r",
			awg_struct[list_counter + 1].sect);
	HAL_UART_Transmit(&huart2, data, size, 100);

	size = sprintf(data, "CLR 314 164 451 200 65535\n\r");
	HAL_UART_Transmit(&huart2, data, size, 100);

	size = sprintf(data, "UF 3 326 172 0 %s\n\r",
			awg_struct[list_counter + 2].sect);
	HAL_UART_Transmit(&huart2, data, size, 100);

	size = sprintf(data, "CLR 314 202 451 236 65535\n\r");
	HAL_UART_Transmit(&huart2, data, size, 100);

	size = sprintf(data, "UF 3 326 208 0 %s\n\r",
			awg_struct[list_counter + 3].sect);
	HAL_UART_Transmit(&huart2, data, size, 100);

	size = sprintf(data, "CLR 314 238 451 271 65535\n\r");
	HAL_UART_Transmit(&huart2, data, size, 100);

	size = sprintf(data, "UF 3 326 244 0 %s\n\r",
			awg_struct[list_counter + 4].sect);
	HAL_UART_Transmit(&huart2, data, size, 100);

}

