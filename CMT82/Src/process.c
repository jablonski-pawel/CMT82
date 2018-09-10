/*
 * process.c
 *
 *  Created on: 16.12.2016
 *      Author: gorian
 */
#include "process.h"
#include "lista.h"

//obroty silników
uint16_t ipr = 2134; // ilość impulsów na jeden obrót
uint16_t mmpr = 157; // długość w mm na jeden obrót
uint16_t ipr_count = 0; //ilość zrobionych impulsów
uint16_t i2do = 0; //ilość impulsów do zrobienia w danym cyklu

uint8_t step = 0; // numer kroku procesu
uint8_t start_begin = 0; //flaga potwierdzająca ręczne włączenie procesu - potrzebna do wyświetlenia pierwszy raz ekranu 6

uint8_t step_cut = 0; // numer kroku bazowania
uint16_t close_open_knife_clk = 0;

extern UART_HandleTypeDef huart1;
extern TIM_HandleTypeDef htim3;
extern uint8_t data[50];
extern uint16_t size;

extern char temp_name[18];
extern uint16_t _pcs;
extern uint16_t _pcs_done;
extern uint16_t _length;
extern uint8_t _left_cov;
extern uint8_t _left_eye;
extern uint8_t _right_eye;
extern uint8_t _right_cov;
extern uint16_t _knife;
extern uint16_t _knife_move_back;
extern uint8_t start;
extern uint8_t stop_process;
extern uint8_t cut;
extern float scaling;
extern RTC_HandleTypeDef hrtc;
extern RTC_TimeTypeDef stimestructureget;

extern uint8_t feed_speed;
extern uint8_t blade_speed;
extern uint16_t batch_pieces;
extern uint8_t batch_delay;
uint8_t batch_delay_flag = 0;

extern uint16_t prescaler_array[10];
extern uint16_t period_array[10];

//lista jednokierunkowa
extern wezel *L;
extern uint16_t p;

void process_run() {
	//bazowanie();

	switch (step) {
	case 0:
		//Jeśli otwarto drzwiczki
		if(HAL_GPIO_ReadPin(door_GPIO_Port, door_Pin) < 1){
			start = 0;
			screen9_init();
		} else {
			if (batch_delay_flag>0)
				break;
			HAL_RTC_GetTime(&hrtc, &stimestructureget, RTC_FORMAT_BIN);
			if(_pcs_done < _pcs && stop_process < 1){
				screen6_init(stimestructureget.Hours, stimestructureget.Minutes);
			} else {
				//przygotowanie zmiennych do wysunięcia ostatniego przewodu obrobionego z maszyny
				ipr_count = 0;
				i2do = process_mm2i(scaling*80);
				step = 18;
				break;
			}
			//kierunek w lewo
			process_steppers_CW(1);
			//zmiana predkosci rolek
			htim3.Init.Period = period_array[feed_speed];
			htim3.Init.Prescaler = prescaler_array[feed_speed];
			if (HAL_TIM_OnePulse_Init(&htim3, TIM_OPMODE_REPETITIVE) != HAL_OK) {
				Error_Handler();
			}
			//włączenie silników
			process_steppers_EN(1);
			ipr_count = 0; // zerowanie counter'a

			if (_left_cov > 0) {
				i2do = process_mm2i(scaling*_left_cov); // obliczenie ilości impulsów do zrobienia
				++step;
			} else {
				// jeśli nie ma do zrobienia nacięcia i oczka, to idzie na drugą stronę
				i2do = process_mm2i(scaling*_length);
				step = 6;
			}
		}
		break;

	case 1:
		//odmierzenie lewego skórowania
		if (ipr_count < i2do) {
			HAL_GPIO_TogglePin(CP_L_GPIO_Port, CP_L_Pin);
			HAL_GPIO_TogglePin(CP_R_GPIO_Port, CP_R_Pin);
			++ipr_count;
		} else {
			++step;
			ipr_count = 0;
			//kierunek zamykania noży
			HAL_GPIO_WritePin(CW_C_GPIO_Port, CW_C_Pin, GPIO_PIN_RESET);
			//zmiana predkosci noży
			htim3.Init.Period = period_array[blade_speed];
			htim3.Init.Prescaler = prescaler_array[blade_speed];
			if (HAL_TIM_OnePulse_Init(&htim3, TIM_OPMODE_REPETITIVE) != HAL_OK) {
				Error_Handler();
			}
		}
		break;

	case 2:
		//nacięcie opony (zamknięcie noży)
		if (ipr_count < _knife) {
			//krok zamykający noże
			HAL_GPIO_TogglePin(CP_C_GPIO_Port, CP_C_Pin);
			++ipr_count;
		} else {
			++step;
			ipr_count = 0;
			//zmiana kierunku noży na otwieranie
			HAL_GPIO_WritePin(CW_C_GPIO_Port, CW_C_Pin, GPIO_PIN_SET);
		}
		break;

	case 3:
		//odskok noży po nacięciu opony
		if (ipr_count < _knife_move_back) {
			//krok otwierający noże
			HAL_GPIO_TogglePin(CP_C_GPIO_Port, CP_C_Pin);
			++ipr_count;
		} else {
			i2do = process_mm2i(scaling*_left_eye);

			//jeśli ma być zrobione oczko
			if (i2do > 0) {
				//kierunek w prawo
				process_steppers_CW(2);
				++step;

			//jeśli oczka ma nie być
			} else {
				i2do = process_mm2i(scaling*_length) - process_mm2i(scaling*_left_cov);
				step = 5;
			}
			//zmiana predkosci rolek
			htim3.Init.Period = period_array[feed_speed];
			htim3.Init.Prescaler = prescaler_array[feed_speed];
			if (HAL_TIM_OnePulse_Init(&htim3, TIM_OPMODE_REPETITIVE) != HAL_OK) {
				Error_Handler();
			}
			ipr_count = 0;
		}
		break;

	case 4:
		//odmierzenie lewego oczka
		if (ipr_count < i2do) {
			HAL_GPIO_TogglePin(CP_L_GPIO_Port, CP_L_Pin);
			HAL_GPIO_TogglePin(CP_R_GPIO_Port, CP_R_Pin);
			++ipr_count;
		} else {
			ipr_count = 0;
			++step;
			//zmiana predkosci noży
			htim3.Init.Period = period_array[blade_speed];
			htim3.Init.Prescaler = prescaler_array[blade_speed];
			if (HAL_TIM_OnePulse_Init(&htim3, TIM_OPMODE_REPETITIVE) != HAL_OK) {
				Error_Handler();
			}
		}
		break;

	case 5:
		//otwieranie noży w pełni przed przesuwaniem przewodu
		if (HAL_GPIO_ReadPin(knife_GPIO_Port, knife_Pin) < 1) {
			HAL_GPIO_TogglePin(CP_C_GPIO_Port, CP_C_Pin);
		} else {
			//kierunek w lewo
			process_steppers_CW(1);
			i2do = process_mm2i(scaling*_length) - (process_mm2i(scaling*_left_cov) - process_mm2i(scaling*_left_eye));
			//zmiana predkosci rolek
			htim3.Init.Period = period_array[feed_speed];
			htim3.Init.Prescaler = prescaler_array[feed_speed];
			if (HAL_TIM_OnePulse_Init(&htim3, TIM_OPMODE_REPETITIVE) != HAL_OK) {
				Error_Handler();
			}
			ipr_count = 0;
			++step;
		}
		break;

	case 6:
		//odmierzenie całego przewodu
		if (ipr_count < i2do) {
			HAL_GPIO_TogglePin(CP_L_GPIO_Port, CP_L_Pin);
			HAL_GPIO_TogglePin(CP_R_GPIO_Port, CP_R_Pin);
			++ipr_count;
		} else {
			//kierunek zamykania noży
			HAL_GPIO_WritePin(CW_C_GPIO_Port, CW_C_Pin, GPIO_PIN_RESET);
			ipr_count = 0;
			++step;
			//zmiana predkosci noży
			htim3.Init.Period = period_array[blade_speed];
			htim3.Init.Prescaler = prescaler_array[blade_speed];
			if (HAL_TIM_OnePulse_Init(&htim3, TIM_OPMODE_REPETITIVE) != HAL_OK) {
				Error_Handler();
			}
		}
		break;

	case 7:
		//ucięcie przewodu
		if(ipr_count < 950){
			HAL_GPIO_TogglePin(CP_C_GPIO_Port, CP_C_Pin);
			++ipr_count;
		} else {
			//zmiana kierunku noży na otwieranie
			HAL_GPIO_WritePin(CW_C_GPIO_Port, CW_C_Pin, GPIO_PIN_SET);

			ipr_count = 0;
			++step;
		}
		break;

	case 8:
		//otwarcie noży
		if (HAL_GPIO_ReadPin(knife_GPIO_Port, knife_Pin) < 1) {
					HAL_GPIO_TogglePin(CP_C_GPIO_Port, CP_C_Pin);
		} else {

			//Jeśli prawy koniec jest do odizolowania
			if (_right_cov > 0) {
				//kierunek w prawo
				process_steppers_CW(2);

				i2do = process_mm2i(5);	//obliczenie ilości impulsów do cofnięcia przewodu w rurce o 5mm
				++step;
			} else {
				step = 17;
			}

			ipr_count = 0;
			//zmiana predkosci rolek
			htim3.Init.Period = period_array[feed_speed];
			htim3.Init.Prescaler = prescaler_array[feed_speed];
			if (HAL_TIM_OnePulse_Init(&htim3, TIM_OPMODE_REPETITIVE) != HAL_OK) {
				Error_Handler();
			}
		}
		break;


	case 9:
		//cofanie przewodu w rurce o 5mm aby nie było kolizji po opuszczeniu rurki
		if (ipr_count < i2do) {
			HAL_GPIO_TogglePin(CP_R_GPIO_Port, CP_R_Pin);
			++ipr_count;
		} else {
			//opuszczenie rurki w dół
			HAL_GPIO_WritePin(C2_GPIO_Port, C2_Pin, GPIO_PIN_SET);

			i2do = process_mm2i(scaling*_right_cov); // obliczenie ilości impulsów do zrobienia
			++step;
			ipr_count = 0;
		}
		break;

	case 10:
		//odmierzenie prawego skórowania
		if (ipr_count < i2do) {
			HAL_GPIO_TogglePin(CP_L_GPIO_Port, CP_L_Pin);
			++ipr_count;
		} else {
			//kierunek zamykania noży
			HAL_GPIO_WritePin(CW_C_GPIO_Port, CW_C_Pin, GPIO_PIN_RESET);

			++step;
			ipr_count = 0;
			//zmiana predkosci noży
			htim3.Init.Period = period_array[blade_speed];
			htim3.Init.Prescaler = prescaler_array[blade_speed];
			if (HAL_TIM_OnePulse_Init(&htim3, TIM_OPMODE_REPETITIVE) != HAL_OK) {
				Error_Handler();
			}
		}
		break;

	case 11:
		//nacięcie opony (zamknięcie noży)
		if (ipr_count < _knife) {
			//krok zamykający noże
			HAL_GPIO_TogglePin(CP_C_GPIO_Port, CP_C_Pin);
			++ipr_count;
		} else {
			++step;
			ipr_count = 0;
			//zmiana kierunku noży na otwieranie
			HAL_GPIO_WritePin(CW_C_GPIO_Port, CW_C_Pin, GPIO_PIN_SET);
		}
		break;

	case 12:
		//odskok noży po nacięciu opony
		if (ipr_count < _knife_move_back) {
			//krok otwierający noże
			HAL_GPIO_TogglePin(CP_C_GPIO_Port, CP_C_Pin);
			++ipr_count;
		} else {
			i2do = process_mm2i(scaling*_right_eye);

			//jeśli ma być zrobione oczko
			if (i2do > 0) {
				//kierunek w lewo
				process_steppers_CW(1);
				++step;

			//jeśli oczka ma nie być
			} else {
				step = 14;
			}
			//zmiana predkosci rolek
			htim3.Init.Period = period_array[feed_speed];
			htim3.Init.Prescaler = prescaler_array[feed_speed];
			if (HAL_TIM_OnePulse_Init(&htim3, TIM_OPMODE_REPETITIVE) != HAL_OK) {
				Error_Handler();
			}
			ipr_count = 0;
		}
		break;

	case 13:
		//odmierzenie prawego oczka
		if (ipr_count < i2do) {
			HAL_GPIO_TogglePin(CP_L_GPIO_Port, CP_L_Pin);
			HAL_GPIO_TogglePin(CP_R_GPIO_Port, CP_R_Pin);
			++ipr_count;
		} else {
			ipr_count = 0;
			++step;
			//zmiana predkosci noży
			htim3.Init.Period = period_array[blade_speed];
			htim3.Init.Prescaler = prescaler_array[blade_speed];
			if (HAL_TIM_OnePulse_Init(&htim3, TIM_OPMODE_REPETITIVE) != HAL_OK) {
				Error_Handler();
			}
		}
		break;

	case 14:
		//otwieranie noży w pełni przed przesuwaniem przewodu
		if (HAL_GPIO_ReadPin(knife_GPIO_Port, knife_Pin) < 1) {
			HAL_GPIO_TogglePin(CP_C_GPIO_Port, CP_C_Pin);
		} else {

			//obliczenie długości przesunięcia przewodu w lewo o długość prawego skórowania + 15mm
			i2do = process_mm2i(15) + process_mm2i(scaling*_right_eye);
			//zmiana predkosci rolek
			htim3.Init.Period = period_array[feed_speed];
			htim3.Init.Prescaler = prescaler_array[feed_speed];
			if (HAL_TIM_OnePulse_Init(&htim3, TIM_OPMODE_REPETITIVE) != HAL_OK) {
				Error_Handler();
			}
			ipr_count = 0;
			++step;
		}
		break;

	case 15:
		//przesunięcie prawego skórowania - wyrzucenie przewodu poza obszar noży
		if (ipr_count < i2do) {
			HAL_GPIO_TogglePin(CP_L_GPIO_Port, CP_L_Pin);
			++ipr_count;
		} else {
			//podniesienie rurki do góry
			HAL_GPIO_WritePin(C2_GPIO_Port, C2_Pin, GPIO_PIN_RESET);

			//obliczenie długości przesunięcia przewodu w lewo o 5mm, aby dosunąć go z powrotem do noży
			i2do = process_mm2i(5);
			ipr_count = 0;
			++step;
		}
		break;

	case 16:
		//przesunięcie przewodu o 5mm z powrotem pod noże
		if (ipr_count < i2do) {
			HAL_GPIO_TogglePin(CP_R_GPIO_Port, CP_R_Pin);
			++ipr_count;
		} else {
			ipr_count = 0;
			++step;
		}
		break;

	case 17:
		//zwiększenie ilości wykonanych przewodów i zapisanie tej wartości do struktury
		++_pcs_done;
		//flaga postoju po zadanej ilości sztuk przewodów
		usun(&L, p);
		wstaw(&L, p, temp_name, _pcs, _pcs_done, _length, _left_cov, _left_eye,
			_right_eye, _right_cov, _knife, _knife_move_back);
		if(_pcs_done%batch_pieces==0 && batch_delay > 0){
			batch_delay_flag=1;
			step = 18;
			break;
		}
		step = 19;
		break;

	case 18:
		//wysunięcie ostatniego przewodu z maszyny i zakończenie procesu
		if (ipr_count < i2do) {
			HAL_GPIO_TogglePin(CP_L_GPIO_Port, CP_L_Pin);
			++ipr_count;
		} else {
			//jeśli flaga postoju po zrobieniu partii przewodów jest wystawiona i przewód ostatni został wysunięty, to przygotowuje do dalszej pracy po zerowaniu flagi
			if(batch_delay_flag>0){
				step = 19;
				break;
			}
			if(stop_process < 1){
				screen6_init(stimestructureget.Hours, stimestructureget.Minutes);
			} else {
				screen6_1_init(stimestructureget.Hours, stimestructureget.Minutes);
				stop_process = 0;
			}
			ipr_count = 0;
			process_steppers_EN(0);
			start = 0;
			step = 0;
		}
		break;

	default:
		step = 0;
		break;
	}
}

//zamiana długości w mm na ilość impulsów dla silników
uint16_t process_mm2i(float mm) {
	uint16_t i = 0;
	i = mm * ipr / mmpr;
	return i;
}

//włączanie/wyłączanie silników krokowych (sygnał EN)
// -1 - błąd
// 0 - wyłączony
// 1 - włączony
uint8_t process_steppers_EN(uint8_t state) {
	//wyłączenie silników
	if (state == 0) {
		HAL_GPIO_WritePin(ENA_GPIO_Port, ENA_Pin, GPIO_PIN_SET);
		return 0;
	}

	//włączenie silników
	if (state == 1) {
		HAL_GPIO_WritePin(ENA_GPIO_Port, ENA_Pin, GPIO_PIN_RESET);
		return 1;
	}

	return -1;
}

uint8_t process_steppers_CW(uint8_t dir) {
	//w lewo
	if (dir == 1) {
		HAL_GPIO_WritePin(CW_L_GPIO_Port, CW_L_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(CW_R_GPIO_Port, CW_R_Pin, GPIO_PIN_RESET);
		return 1;
	}

	//w prawo
	if (dir == 2) {
		HAL_GPIO_WritePin(CW_L_GPIO_Port, CW_L_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(CW_R_GPIO_Port, CW_R_Pin, GPIO_PIN_SET);
		return 2;
	}
	return 0;
}

void process_cut() {
	switch (step_cut) {
	case 0:
		//kierunek zamykania
		HAL_GPIO_WritePin(CW_C_GPIO_Port, CW_C_Pin, GPIO_PIN_RESET);
		close_open_knife_clk = 0;
		++step_cut;
		break;

	case 1:
		//zamykanie noży (ucięcie ew. przewodu)
		if (close_open_knife_clk < 900) {
			HAL_GPIO_TogglePin(CP_C_GPIO_Port, CP_C_Pin);
			++close_open_knife_clk;
		} else {
			//kierunek otwierania
			HAL_GPIO_WritePin(CW_C_GPIO_Port, CW_C_Pin, GPIO_PIN_SET);
			++step_cut;
		}
		break;

	case 2:
		//otwieranie noży
		if (close_open_knife_clk > 0) {
			HAL_GPIO_TogglePin(CP_C_GPIO_Port, CP_C_Pin);
			--close_open_knife_clk;
		} else {
			cut = 2;
			step_cut = 0;
		}
		break;

	default:
		cut = 0;
		break;
	}
}
