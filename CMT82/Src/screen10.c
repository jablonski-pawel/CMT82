/*
 * screen10.c
 *
 *  Created on: 23.11.2016
 *      Author: gorian
 */
#include "screen10.h"
#include <stdlib.h>

extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;
extern uint8_t screen;
extern uint16_t size;
extern uint16_t position_x;
extern uint16_t position_y;
extern uint8_t action;

extern uint8_t data[50];
extern RTC_DateTypeDef sdatestructureget;
extern RTC_TimeTypeDef stimestructureget;
extern RTC_HandleTypeDef hrtc;

uint8_t box_number = 0;
uint8_t max_size = 4;
uint16_t temp = 0;
char *temp_str[1];
char *temp_hours[1];
char *temp_minutes[1];
char *temp_day[1];
char *temp_month[1];
char *temp_year[1];

void screen10_init() {

	size = sprintf(data, "LOAD 0 0 10.bmp\n\r");
	HAL_UART_Transmit(&huart2, data, size, 100);

	size = sprintf(data, "Zaladowano ekran 10\n\r");
	HAL_UART_Transmit(&huart1, data, size, 1000);

	HAL_RTC_GetTime(&hrtc, &stimestructureget, RTC_FORMAT_BIN);
	size = sprintf(data, "UF 5 55 94 0 %0.2d\n\r", stimestructureget.Hours);
	HAL_UART_Transmit(&huart2, data, size, 100);

	size = sprintf(data, "UF 5 112 94 0 %0.2d\n\r", stimestructureget.Minutes);
	HAL_UART_Transmit(&huart2, data, size, 100);

	HAL_RTC_GetDate(&hrtc, &sdatestructureget, RTC_FORMAT_BIN);
	size = sprintf(data, "UF 5 55 151 0 %0.2d\n\r", sdatestructureget.Date);
	HAL_UART_Transmit(&huart2, data, size, 100);

	size = sprintf(data, "UF 5 113 151 0 %0.2d\n\r", sdatestructureget.Month);
	HAL_UART_Transmit(&huart2, data, size, 100);

	size = sprintf(data, "UF 5 171 151 0 20%0.2d\n\r", sdatestructureget.Year);
	HAL_UART_Transmit(&huart2, data, size, 100);

	sprintf(temp_hours, "%0.2d", stimestructureget.Hours);
	sprintf(temp_minutes, "%0.2d", stimestructureget.Minutes);

	sprintf(temp_day, "%0.2d", sdatestructureget.Date);
	sprintf(temp_month, "%0.2d", sdatestructureget.Month);
	sprintf(temp_year, "%0.2d", sdatestructureget.Year);

	screen = 10;
}

int screen10_button() {
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

	if (((45 < position_x) && (position_x < 90))
			&& ((84 < position_y) && (position_y < 124))) {
		size = sprintf(data, "BUZ 150 2000\n\r");
		HAL_UART_Transmit(&huart2, data, size, 100);
		return 4;
	}

	if (((100 < position_x) && (position_x < 150))
			&& ((84 < position_y) && (position_y < 124))) {
		size = sprintf(data, "BUZ 150 2000\n\r");
		HAL_UART_Transmit(&huart2, data, size, 100);
		return 5;
	}

	if (((45 < position_x) && (position_x < 90))
			&& ((150 < position_y) && (position_y < 180))) {
		size = sprintf(data, "BUZ 150 2000\n\r");
		HAL_UART_Transmit(&huart2, data, size, 100);
		return 6;
	}

	if (((100 < position_x) && (position_x < 150))
			&& ((150 < position_y) && (position_y < 180))) {
		size = sprintf(data, "BUZ 150 2000\n\r");
		HAL_UART_Transmit(&huart2, data, size, 100);
		return 7;
	}

	if (((160 < position_x) && (position_x < 240))
			&& ((150 < position_y) && (position_y < 180))) {
		size = sprintf(data, "BUZ 150 2000\n\r");
		HAL_UART_Transmit(&huart2, data, size, 100);
		return 8;
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

	position_x = 0;
	position_y = 0;
	return 0;
	//0 - puste miejsce
	//1 - powrót do ekranu 2 (wyjście)
	//2 - zapisanie ustawień i wyjście do ekranu 2
	//3 - usuwanie wartości
	//4 - godzina
	//5 - minuty
	//6 - dni
	//7 - miesiące
	//8 - lata
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
	// Y 70; 115; 160; 205; 250
	//X 320; 365; 410; 460

}

void screen10_action(int button) {

	switch (button) {

	case 1:
		sprintf(temp_str, "");
		sprintf(temp_hours, "");
		sprintf(temp_minutes, "");
		sprintf(temp_day, "");
		sprintf(temp_month, "");
		sprintf(temp_year, "");
		box_number = 0;
		screen = 2;
		action = 1;
		break;

	case 2:
		if (strlen(temp_hours) > 0)
			stimestructureget.Hours = atoi(temp_hours);

		if (strlen(temp_minutes) > 0)
			stimestructureget.Minutes = atoi(temp_minutes);

		if (strlen(temp_day) > 0)
			sdatestructureget.Date = atoi(temp_day);

		if (strlen(temp_month) > 0)
			sdatestructureget.Month = atoi(temp_month);

		if (strlen(temp_year) > 0)
			sdatestructureget.Year = atoi(temp_year);

		HAL_RTC_SetTime(&hrtc, &stimestructureget, RTC_FORMAT_BIN);
		HAL_RTC_SetDate(&hrtc, &sdatestructureget, RTC_FORMAT_BIN);
		sprintf(temp_str, "");
		sprintf(temp_hours, "");
		sprintf(temp_minutes, "");
		sprintf(temp_day, "");
		sprintf(temp_month, "");
		sprintf(temp_year, "");
		box_number = 0;
		screen = 2;
		action = 1;
		break;

	case 3:

		switch (box_number) {
		case 1:
			sprintf(temp_hours, "");
			size = sprintf(data, "CLR 45 84 90 124 65535\n\r");
			HAL_UART_Transmit(&huart2, data, size, 100);
			break;
		case 2:
			sprintf(temp_minutes, "");
			size = sprintf(data, "CLR 100 84 150 124 65535\n\r");
			HAL_UART_Transmit(&huart2, data, size, 100);
			break;
		case 3:
			sprintf(temp_day, "");
			size = sprintf(data, "CLR 45 150 90 180 65535\n\r");
			HAL_UART_Transmit(&huart2, data, size, 100);
			break;
		case 4:
			sprintf(temp_month, "");
			size = sprintf(data, "CLR 100 150 150 180 65535\n\r");
			HAL_UART_Transmit(&huart2, data, size, 100);
			break;
		case 5:
			sprintf(temp_year, "");
			size = sprintf(data, "CLR 160 150 240 180 65535\n\r");
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
		box_number = 1;
		max_size = 2;
		sprintf(temp_hours, "");
		size = sprintf(data, "Godzina\n\r");
		HAL_UART_Transmit(&huart1, data, size, 1000);
		break;

	case 5:
		box_number = 2;
		max_size = 2;
		sprintf(temp_minutes, "");
		size = sprintf(data, "Minuty\n\r");
		HAL_UART_Transmit(&huart1, data, size, 1000);
		break;

	case 6:
		box_number = 3;
		max_size = 2;
		sprintf(temp_day, "");
		size = sprintf(data, "Dzien\n\r");
		HAL_UART_Transmit(&huart1, data, size, 1000);
		break;

	case 7:
		box_number = 4;
		max_size = 2;
		sprintf(temp_month, "");
		size = sprintf(data, "Miesiac\n\r");
		HAL_UART_Transmit(&huart1, data, size, 1000);
		break;

	case 8:
		box_number = 5;
		max_size = 2;
		sprintf(temp_year, "");
		size = sprintf(data, "Rok\n\r");
		HAL_UART_Transmit(&huart1, data, size, 1000);
		break;

	case 10:
		screen10_value_update('0');
		temp = atoi(temp_str);
		size = sprintf(data, "Zero Val: %d\n\r", temp);
		HAL_UART_Transmit(&huart1, data, size, 1000);
		break;

	case 11:
		screen10_value_update('1');
		temp = atoi(temp_str);
		size = sprintf(data, "Jeden Val: %d\n\r", temp);
		HAL_UART_Transmit(&huart1, data, size, 1000);
		break;

	case 12:
		screen10_value_update('2');
		temp = atoi(temp_str);
		size = sprintf(data, "Dwa Val: %d\n\r", temp);
		HAL_UART_Transmit(&huart1, data, size, 1000);
		break;

	case 13:
		screen10_value_update('3');
		temp = atoi(temp_str);
		size = sprintf(data, "Trzy Val: %d\n\r", temp);
		HAL_UART_Transmit(&huart1, data, size, 1000);
		break;

	case 14:
		screen10_value_update('4');
		temp = atoi(temp_str);
		size = sprintf(data, "Cztery Val: %d\n\r", temp);
		HAL_UART_Transmit(&huart1, data, size, 1000);
		break;

	case 15:
		screen10_value_update('5');
		temp = atoi(temp_str);
		size = sprintf(data, "Piec Val: %d\n\r", temp);
		HAL_UART_Transmit(&huart1, data, size, 1000);
		break;

	case 16:
		screen10_value_update('6');
		temp = atoi(temp_str);
		size = sprintf(data, "Szesc Val: %d\n\r", temp);
		HAL_UART_Transmit(&huart1, data, size, 1000);
		break;

	case 17:
		screen10_value_update('7');
		temp = atoi(temp_str);
		size = sprintf(data, "Siedem Val: %d\n\r", temp);
		HAL_UART_Transmit(&huart1, data, size, 1000);
		break;

	case 18:
		screen10_value_update('8');
		temp = atoi(temp_str);
		size = sprintf(data, "Osiem Val: %d\n\r", temp);
		HAL_UART_Transmit(&huart1, data, size, 1000);
		break;

	case 19:
		screen10_value_update('9');
		temp = atoi(temp_str);
		size = sprintf(data, "Dziewiec Val: %d\n\r", temp);
		HAL_UART_Transmit(&huart1, data, size, 1000);
		break;

	default:
		break;
	}
}

void screen10_value_update(char c) {
	uint8_t len = 0;
	switch (box_number) {
	case 1:
		len = strlen(temp_hours);
		break;
	case 2:
		len = strlen(temp_minutes);
		break;
	case 3:
		len = strlen(temp_day);
		break;
	case 4:
		len = strlen(temp_month);
		break;
	case 5:
		len = strlen(temp_year);
		break;
	default:
		len = strlen(temp_str);
		break;
	}

	if (len < max_size) {

		char *str2 = malloc(len + 1 + 1); //allokacja pamięci tla tymczasowego stringa - zwiększenie o jeden plus jeden dla znaku końca

		switch (box_number) {
		case 1:
			strcpy(str2, temp_hours);
			break;
		case 2:
			strcpy(str2, temp_minutes);
			break;
		case 3:
			strcpy(str2, temp_day);
			break;
		case 4:
			strcpy(str2, temp_month);
			break;
		case 5:
			strcpy(str2, temp_year);
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
		switch (box_number) {

		case 1:
			strcpy(temp_hours, str2);
			if (atoi(temp_hours) > 23) {
				size = sprintf(data, "BUZ 500 500\n\r");
				HAL_UART_Transmit(&huart2, data, size, 100);
				sprintf(temp_hours, "");
				size = sprintf(data, "CLR 45 84 90 124 65535\n\r");
				HAL_UART_Transmit(&huart2, data, size, 100);

			} else {
				size = sprintf(data, "CLR 45 84 90 124 65535\n\r");
				HAL_UART_Transmit(&huart2, data, size, 100);

				size = sprintf(data, "UF 5 55 94 0 %s\n\r", temp_hours);
				HAL_UART_Transmit(&huart2, data, size, 100);
			}
			break;

		case 2:
			strcpy(temp_minutes, str2);
			if (atoi(temp_minutes) > 59) {
				size = sprintf(data, "BUZ 500 500\n\r");
				HAL_UART_Transmit(&huart2, data, size, 100);
				sprintf(temp_minutes, "");
				size = sprintf(data, "CLR 100 84 150 124 65535\n\r");
				HAL_UART_Transmit(&huart2, data, size, 100);

			} else {
				size = sprintf(data, "CLR 100 84 150 124 65535\n\r");
				HAL_UART_Transmit(&huart2, data, size, 100);

				size = sprintf(data, "UF 5 112 94 0 %s\n\r", temp_minutes);
				HAL_UART_Transmit(&huart2, data, size, 100);
			}
			break;

		case 3:
			strcpy(temp_day, str2);
			if (atoi(temp_day) > 31) {
				size = sprintf(data, "BUZ 500 500\n\r");
				HAL_UART_Transmit(&huart2, data, size, 100);
				sprintf(temp_day, "");
				size = sprintf(data, "CLR 45 150 90 180 65535\n\r");
				HAL_UART_Transmit(&huart2, data, size, 100);

			} else {
				size = sprintf(data, "CLR 45 150 90 180 65535\n\r");
				HAL_UART_Transmit(&huart2, data, size, 100);

				size = sprintf(data, "UF 5 55 151 0 %s\n\r", temp_day);
				HAL_UART_Transmit(&huart2, data, size, 100);
			}
			break;

		case 4:
			strcpy(temp_month, str2);
			if (atoi(temp_month) > 12) {
				size = sprintf(data, "BUZ 500 500\n\r");
				HAL_UART_Transmit(&huart2, data, size, 100);
				sprintf(temp_month, "");
				size = sprintf(data, "CLR 100 150 150 180 65535\n\r");
				HAL_UART_Transmit(&huart2, data, size, 100);

			} else {
				size = sprintf(data, "CLR 100 150 150 180 65535\n\r");
				HAL_UART_Transmit(&huart2, data, size, 100);

				size = sprintf(data, "UF 5 113 151 0 %s\n\r", temp_month);
				HAL_UART_Transmit(&huart2, data, size, 100);
			}
			break;

		case 5:
			strcpy(temp_year, str2);

			size = sprintf(data, "CLR 160 150 240 180 65535\n\r");
			HAL_UART_Transmit(&huart2, data, size, 100);

			size = sprintf(data, "UF 5 171 151 0 20%s\n\r", temp_year);
			HAL_UART_Transmit(&huart2, data, size, 100);
			break;

		default:
			strcpy(temp_str, str2);
			break;
		}

		free(str2);
	}
}
