/*
 * screen15.c
 *
 *  Created on: 18.11.2016
 *      Author: gorian
 *      Ekran: usuwanie programu - potwierdzenie
 */
#include "screen15.h"
#include "lista.h"

extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;
extern I2C_HandleTypeDef hi2c2;

extern uint8_t screen;
extern uint16_t size;
extern uint16_t position_x;
extern uint16_t position_y;
extern uint8_t action;

extern uint8_t data[50];

//lista jednokierunkowa
extern wezel *L;
extern uint16_t p;

uint8_t node_to_delete = 0;

void screen15_init() {

	size = sprintf(data, "LOAD 0 0 15.bmp\n\r");
	HAL_UART_Transmit(&huart2, data, size, 100);

	size = sprintf(data, "Zaladowano ekran 15\n\r");
	HAL_UART_Transmit(&huart1, data, size, 1000);

	size = sprintf(data, "BUZ 500 2000\n\r");
	HAL_UART_Transmit(&huart2, data, size, 100);

	screen = 15;
}

int screen15_button() {
	if (((240 < position_x) && (position_x < 310))
			&& ((140 < position_y) && (position_y < 190))) {
		size = sprintf(data, "BUZ 150 2000\n\r");
		HAL_UART_Transmit(&huart2, data, size, 100);
		return 1;
	}

	if (((310 < position_x) && (position_x < 380))
			&& ((140 < position_y) && (position_y < 190))) {
		size = sprintf(data, "BUZ 150 2000\n\r");
		HAL_UART_Transmit(&huart2, data, size, 100);
		return 2;
	}

	position_x = 0;
	position_y = 0;
	return 0;
	//0 - puste miejsce
	//1 - nie - powrót do ekranu 3 (wyjście)
	//2 - tak - usuwanie
}

void screen15_action(int button) {

	uint8_t new_p_max;
	uint8_t p_eeprom;
	uint16_t adresRZ = 100;//adresRozpoczeciaZapisu
	uint8_t temp_odczyt;
	switch (button) {

	case 1:
		screen = 2;
		action = 1;
		break;

	case 2:
		size = sprintf(data, "NODE VAL: %d\n\r", node_to_delete);
		HAL_UART_Transmit(&huart1, data, size, 100);
		if(node_to_delete>0){
			usun(&L, node_to_delete);

			size = sprintf(data, "node_to_delete %d\n\r", node_to_delete);
			HAL_UART_Transmit(&huart1, data, size, 100);

			//usuwanie z eeproma
			HAL_GPIO_WritePin(EEPROM_WP_GPIO_Port, EEPROM_WP_Pin, GPIO_PIN_SET);
			HAL_I2C_Mem_Read_IT(&hi2c2, 0xa0, 99, 1, &p_eeprom, 1);

			for(uint8_t i=node_to_delete; i<p_eeprom-1; i++){
				for(uint8_t j=0; j<32; j++){
					HAL_GPIO_WritePin(EEPROM_WP_GPIO_Port, EEPROM_WP_Pin, GPIO_PIN_SET);
					HAL_I2C_Mem_Read(&hi2c2, 0xa0, adresRZ+32*(i+1)+j, 1, &temp_odczyt, 1, HAL_MAX_DELAY);
//					size = sprintf(data, "odczytuje %d do komorki: %d\n\r", temp_odczyt, adresRZ+32*(i+1)+j);
//					HAL_UART_Transmit(&huart1, data, size, 100);
					HAL_GPIO_WritePin(EEPROM_WP_GPIO_Port, EEPROM_WP_Pin, GPIO_PIN_RESET);
					HAL_I2C_Mem_Write(&hi2c2, 0xa0, adresRZ+32*i+j, 1, &temp_odczyt, 1, HAL_MAX_DELAY);
					size = sprintf(data, "zapisuje %d do komorki: %d\n\r", temp_odczyt, adresRZ+32*i+j );
					HAL_UART_Transmit(&huart1, data, size, 100);
				}
			}

			//zmiana ilosci programow po usunieciu
			new_p_max = ilosc_wezlow(L);
//			HAL_GPIO_WritePin(EEPROM_WP_GPIO_Port, EEPROM_WP_Pin, GPIO_PIN_RESET);
			HAL_I2C_Mem_Write(&hi2c2, 0xa0, 99, 1, &new_p_max, 1, HAL_MAX_DELAY);
			size = sprintf(data, "ilosc p do odczytu %d - 1\n\r", new_p_max);
			HAL_UART_Transmit(&huart1, data, size, 100);

			HAL_GPIO_WritePin(EEPROM_WP_GPIO_Port, EEPROM_WP_Pin, GPIO_PIN_SET);

			p = 0;
			screen = 2;
			action = 1;
		}
		break;

	default:
		break;
	}
}
