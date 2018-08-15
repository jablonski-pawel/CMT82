/*
 * lista.c: implementacja listy jednokierunkowej
 *
 *  Created on: 28.11.2016
 *      Author: gorian
 */
#include <stdio.h>
#include <stdlib.h>
#include "lista.h"

extern UART_HandleTypeDef huart1;
extern I2C_HandleTypeDef hi2c2;
extern uint16_t size;
extern uint8_t data[50];

/* wstawPocz: tworzy i wstawia nowy element na poczatek listy */
uint8_t wstawPocz(wezel **L, char name[18], uint16_t pcs, uint16_t pcs_done, uint16_t length,
		uint8_t left_cov, uint8_t left_eye, uint8_t right_eye,
		uint8_t right_cov, uint16_t knife, uint16_t knife_move_back) {
	wezel *Q;

	if ((Q = (wezel *) malloc(sizeof(wezel))) == NULL)
		return BRAK_PAMIECI;

	//Q->name = name;
	strcpy(Q->name, name);
	Q->pcs = pcs;
	Q->pcs_done = pcs_done;
	Q->length = length;
	Q->left_cov = left_cov;
	Q->left_eye = left_eye;
	Q->right_eye = right_eye;
	Q->right_cov = right_cov;
	Q->knife = knife;
	Q->knife_move_back = knife_move_back;
	Q->nastepny = *L;

	*L = Q;

	return POPRAWNIE;
}

/* wstaw: tworzy i wstawia nowy element na p-ta pozycje na liscie;
 *        pozycja liczona jest od 0
 */
uint8_t wstaw(wezel **L, uint8_t p, char name[18], uint16_t pcs, uint16_t pcs_done,
		uint16_t length, uint8_t left_cov, uint8_t left_eye, uint8_t right_eye,
		uint8_t right_cov, uint16_t knife, uint16_t knife_move_back) {
	wezel *Q, *S = NULL, *T = *L;

	while (T && (p > 0)) {
		S = T;
		T = T->nastepny;
		p--;
	}

	if (p != 0)
		return BLAD_POZYCJI;

	if ((Q = (wezel *) malloc(sizeof(wezel))) == NULL)
		return BRAK_PAMIECI;

	//Q->name = name;
	strcpy(Q->name, name);
	Q->pcs = pcs;
	Q->pcs_done = pcs_done;
	Q->length = length;
	Q->left_cov = left_cov;
	Q->left_eye = left_eye;
	Q->right_eye = right_eye;
	Q->right_cov = right_cov;
	Q->knife = knife;
	Q->knife_move_back = knife_move_back;
	Q->nastepny = T;

	if (S == NULL)
		*L = Q;
	else
		S->nastepny = Q;

	return POPRAWNIE;
}

/* usun: usuwa element z p-tej pozycji na liscie;
 *       pozycja liczona jest od 0
 */
uint8_t usun(wezel **L, uint8_t p) {
	wezel *S = NULL, *T = *L;

	while (T && (p > 0)) {
		S = T;
		T = T->nastepny;
		p--;
	}

	if ((p != 0) || (T == NULL))
		return BLAD_POZYCJI;

	if (S == NULL)
		*L = T->nastepny;
	else
		S->nastepny = T->nastepny;

	free(T);

	return POPRAWNIE;
}

/* zwroc: przypisuje zmiennej "wartosc" wartosc p-tego elementu z listy */
uint8_t zwroc(wezel *L, uint8_t p, char *name[18], uint16_t *pcs, uint16_t *pcs_done,
		uint16_t *length, uint8_t *left_cov, uint8_t *left_eye,
		uint8_t *right_eye, uint8_t *right_cov, uint16_t *knife, uint16_t *knife_move_back) {
	while (L && (p > 0)) {
		L = L->nastepny;
		p--;
	}

	if ((p != 0) || (L == NULL))
		return BLAD_POZYCJI;

	//*name = L->name;
	strcpy(name, L->name);
	*pcs = L->pcs;
	*pcs_done = L->pcs_done;
	*length = L->length;
	*left_cov = L->left_cov;
	*left_eye = L->left_eye;
	*right_eye = L->right_eye;
	*right_cov = L->right_cov;
	*knife = L->knife;
	*knife_move_back = L->knife_move_back;

	return 0;
}

/* znajdz: przypisuje zmiennej "p" numer elementu listy, pod ktorym wystepuje
 *         "wartosc"
 */
/*
 uint8_t znajdz(wezel *L, uint8_t *p, uint8_t wartosc) {
 *p = 0;

 while (L && (L->wartosc != wartosc)) {
 (*p)++;
 L = L->nastepny;
 }

 if (L == NULL)
 return BLAD_POZYCJI;

 return POPRAWNIE;
 }*/

/* wyczysc: usuwa wszystkie elementy z listy */
void wyczysc(wezel **L) {
	wezel *Q;

	while (*L) {
		Q = *L;
		*L = (*L)->nastepny;

		free(Q);
	}
}

uint16_t ilosc_wezlow(wezel *L) {
	uint16_t p = 0;
	while (L) {
		L = (L)->nastepny;
		p++;
	}
	return p;
}

/* wypisz: wypisuje wszystkie elementy z listy */
void wypisz(wezel *L) {
	while (L) {
		//printf("%d\n", );
		size = sprintf(data, "Nazwa programu: %s\n\r", L->name);
		HAL_UART_Transmit(&huart1, data, size, 1000);

		size = sprintf(data,
				"Ilosc: %d\n\rDlugosc: %d\n\rDl. lewej gumy: %d\n\r", L->pcs,
				L->length, L->left_cov);
		HAL_UART_Transmit(&huart1, data, size, 1000);

		size = sprintf(data,
				"Dl. lewego oczka: %d\n\rDl. prawego oczka: %d\n\r",
				L->left_eye, L->right_eye);
		HAL_UART_Transmit(&huart1, data, size, 1000);

		size = sprintf(data, "Dl. prawej gumy: %d\n\r", L->right_cov);
		HAL_UART_Transmit(&huart1, data, size, 1000);

		size = sprintf(data, "Impulsy noza: %d\n\r", L->knife);
		HAL_UART_Transmit(&huart1, data, size, 1000);

		size = sprintf(data, "Impulsy odskoku noza: %d\n\r\n\r", L->knife_move_back);
		HAL_UART_Transmit(&huart1, data, size, 1000);
		L = L->nastepny;
	}
}

/* zapisz: zapisuje wszystkie elementy z listy do pliku tekstowego */
/*int zapisz(wezel *L, char *nazwaPliku)
 {
 FILE *f;

 if ((f = fopen(nazwaPliku, "w")) == NULL)
 return BLAD_IO;

 while (L) {
 if (fprintf(f, "%d%c",
 L -> wartosc, L -> nastepny ? ' ' : '\n') < 0)
 return BLAD_IO;

 L = L -> nastepny;
 }

 fclose(f);

 return POPRAWNIE;
 }

 */

/* wczytaj: wczytuje z pliku tekstowego elementy i wstawia je na liste
 *          w odwrotnej kolejnosci
 */
/*int wczytaj(wezel **L, char *nazwaPliku)
 {
 wezel *Q, *T = *L;
 FILE *f;
 int wartosc;

 if ((f = fopen(nazwaPliku, "r")) == NULL)
 return BLAD_IO;

 /* odnalezienie konca listy *//*
 while (T && (T -> nastepny))
 T = T -> nastepny;

 while (fscanf(f, "%d", &wartosc) != EOF) {
 if ((Q = (wezel *)malloc(sizeof (wezel))) == NULL)
 return BRAK_PAMIECI;

 Q -> wartosc = wartosc;
 Q -> nastepny = NULL;

 if (T == NULL)
 *L = T = Q;
 else {
 T -> nastepny = Q;   /* dopisywanie elementow do konca listy *//*
 T = T -> nastepny;
 }
 }

 fclose(f);

 return POPRAWNIE;
 }*/


void zapisz_EEPROM(wezel *L, uint8_t p){

	uint16_t adresRZ = 100;//adresRozpoczeciaZapisu
	uint16_t struct_size = 0;
	uint8_t *localZero;
	uint8_t *temp_array[2];
	uint8_t *ptr_name;
	uint8_t _temp_p;

	_temp_p = p;
	while (L && (_temp_p > 0)) {
			L = L->nastepny;
			_temp_p--;
	}

	HAL_GPIO_WritePin(EEPROM_WP_GPIO_Port, EEPROM_WP_Pin, GPIO_PIN_RESET);

	//zapisywanie nazwy
	ptr_name = L->name;
	localZero = 0;
	size = sprintf(data, "Adres pocz: %d, rozmiar str: %d\n\r", adresRZ+32*p, struct_size);
	HAL_UART_Transmit(&huart1, data, size, 100);

	for(uint8_t i=0; i<18; i++){
		if(i<strlen(L->name)){
			HAL_I2C_Mem_Write(&hi2c2, 0xa0, adresRZ+32*p+struct_size, 1, ptr_name+i, 1, HAL_MAX_DELAY);
			size = sprintf(data, "Zapisany znak: %s\n\r", ptr_name+i);
		}else{
			HAL_I2C_Mem_Write(&hi2c2, 0xa0, adresRZ+32*p+struct_size, 1, localZero, 1, HAL_MAX_DELAY);
			size = sprintf(data, "Zapisany znak: 0\n\r");
		}

		HAL_UART_Transmit(&huart1, data, size, 100);
		struct_size+= 1;
	}

	//zapisywanie ilości sztuk
	temp_array[0] = (uint8_t)((L->pcs & 0xff00) >> 8);
	temp_array[1] = (uint8_t)L->pcs & 0xff;
	HAL_I2C_Mem_Write(&hi2c2, 0xa0, adresRZ+32*p+struct_size, 1, temp_array, 1, HAL_MAX_DELAY);
	struct_size+=1;
	size = sprintf(data, "Zapisana wartosc PCS: %d; Bajt 1\n\r", L->pcs);
	HAL_UART_Transmit(&huart1, data, size, 100);

	HAL_I2C_Mem_Write(&hi2c2, 0xa0, adresRZ+32*p+struct_size, 1, temp_array+1, 1, HAL_MAX_DELAY);
	struct_size+=1;
	size = sprintf(data, "Zapisana wartosc PCS: %d; Bajt 2\n\r", L->pcs);
	HAL_UART_Transmit(&huart1, data, size, 100);

	//zapisywanie ilości sztuk wykonanych
	temp_array[0] = (uint8_t)((L->pcs_done & 0xff00) >> 8);
	temp_array[1] = (uint8_t)L->pcs_done & 0xff;
	HAL_I2C_Mem_Write(&hi2c2, 0xa0, adresRZ+32*p+struct_size, 1, temp_array, 1, HAL_MAX_DELAY);
	struct_size+=1;
	size = sprintf(data, "Zapisana wartosc PCS Done: %d; Bajt 1\n\r", L->pcs_done);
	HAL_UART_Transmit(&huart1, data, size, 100);

	HAL_I2C_Mem_Write(&hi2c2, 0xa0, adresRZ+32*p+struct_size, 1, temp_array+1, 1, HAL_MAX_DELAY);
	struct_size+=1;
	size = sprintf(data, "Zapisana wartosc PCS Done: %d; Bajt 2\n\r", L->pcs_done);
	HAL_UART_Transmit(&huart1, data, size, 100);

	//zapisywanie długości przewodu
	temp_array[0] = (uint8_t)((L->length & 0xff00) >> 8);
	temp_array[1] = (uint8_t)L->length & 0xff;
	HAL_I2C_Mem_Write(&hi2c2, 0xa0, adresRZ+32*p+struct_size, 1, temp_array, 1, HAL_MAX_DELAY);
	struct_size+= 1;
	size = sprintf(data, "Zapisana wartosc Length: %d; Bajt 1\n\r", L->length);
	HAL_UART_Transmit(&huart1, data, size, 100);

	HAL_I2C_Mem_Write(&hi2c2, 0xa0, adresRZ+32*p+struct_size, 1, temp_array+1, 1, HAL_MAX_DELAY);
	struct_size+= 1;
	size = sprintf(data, "Zapisana wartosc Length: %d; Bajt 2\n\r", L->length);
	HAL_UART_Transmit(&huart1, data, size, 100);

	//zapisywanie lewego skórowania
	HAL_I2C_Mem_Write(&hi2c2, 0xa0, adresRZ+32*p+struct_size, sizeof(L->left_cov), &L->left_cov, sizeof(L->left_cov), HAL_MAX_DELAY);
	struct_size+=sizeof(L->left_cov);
	size = sprintf(data, "Zapisana wartosc Left cov: %d\n\r", L->left_cov);
	HAL_UART_Transmit(&huart1, data, size, 100);

	//zapisywanie lewego oczka
	HAL_I2C_Mem_Write(&hi2c2, 0xa0, adresRZ+32*p+struct_size, sizeof(L->left_eye), &L->left_eye, sizeof(L->left_eye), HAL_MAX_DELAY);
	struct_size+=sizeof(L->left_eye);
	size = sprintf(data, "Zapisana wartosc Left eye: %d\n\r", L->left_eye);
	HAL_UART_Transmit(&huart1, data, size, 100);

	//zapisywanie prawego oczka
	HAL_I2C_Mem_Write(&hi2c2, 0xa0, adresRZ+32*p+struct_size, sizeof(L->right_eye), &L->right_eye, sizeof(L->right_eye), HAL_MAX_DELAY);
	struct_size+=sizeof(L->right_eye);
	size = sprintf(data, "Zapisana wartosc Right eye: %d\n\r", L->right_eye);
	HAL_UART_Transmit(&huart1, data, size, 100);

	//zapisywanie prawego skórowania
	HAL_I2C_Mem_Write(&hi2c2, 0xa0, adresRZ+32*p+struct_size, sizeof(L->right_cov), &L->right_cov, sizeof(L->right_cov), HAL_MAX_DELAY);
	struct_size+=sizeof(L->right_cov);
	size = sprintf(data, "Zapisana wartosc Right cov: %d\n\r", L->right_cov);
	HAL_UART_Transmit(&huart1, data, size, 100);

	//zapisywanie średnicy rdzenia przewodu
	temp_array[0] = (uint8_t)((L->knife & 0xff00) >> 8);
	temp_array[1] = (uint8_t)L->knife & 0xff;
	HAL_I2C_Mem_Write(&hi2c2, 0xa0, adresRZ+32*p+struct_size, 1, temp_array, 1, HAL_MAX_DELAY);
	struct_size+= 1;
	size = sprintf(data, "Zapisana wartosc Knife: %d; Bajt 1\n\r", L->knife);
	HAL_UART_Transmit(&huart1, data, size, 100);

	HAL_I2C_Mem_Write(&hi2c2, 0xa0, adresRZ+32*p+struct_size, 1, temp_array+1, 1, HAL_MAX_DELAY);
	struct_size+= 1;
	size = sprintf(data, "Zapisana wartosc Knife: %d; Bajt 2\n\r", L->knife);
	HAL_UART_Transmit(&huart1, data, size, 100);

	//zapisywanie odskoku noży
	temp_array[0] = (uint8_t)((L->knife_move_back & 0xff00) >> 8);
	temp_array[1] = (uint8_t)L->knife_move_back & 0xff;
	HAL_I2C_Mem_Write(&hi2c2, 0xa0, adresRZ+32*p+struct_size, 1, temp_array, 1, HAL_MAX_DELAY);
	struct_size+= 1;
	size = sprintf(data, "Zapisana wartosc Knife move back: %d; Bajt 1\n\r", L->knife_move_back);
	HAL_UART_Transmit(&huart1, data, size, 100);

	HAL_I2C_Mem_Write(&hi2c2, 0xa0, adresRZ+32*p+struct_size, 1, temp_array+1, 1, HAL_MAX_DELAY);
	struct_size+= 1;
	size = sprintf(data, "Zapisana wartosc Knife move back: %d; Bajt 1\n\r", L->knife_move_back);
	HAL_UART_Transmit(&huart1, data, size, 100);

	size = sprintf(data, "Rozmiar struktury: %d, adres: %d\n\r", struct_size, adresRZ+32*p+struct_size);
	HAL_UART_Transmit(&huart1, data, size, 100);

	HAL_GPIO_WritePin(EEPROM_WP_GPIO_Port, EEPROM_WP_Pin, GPIO_PIN_SET);

}

void odczyt_EEPROM(wezel *L, uint8_t p){

	uint16_t adresRZ = 100;//adresRozpoczeciaZapisu
	uint16_t struct_size = 0;
	uint8_t odczyt[32];
	char _temp_name[18];
//	HAL_StatusTypeDef status = HAL_OK;

	uint16_t _temp_pcs, _temp_pcs_done, _temp_length, _temp_knife, _temp_knive_move_back;


	HAL_GPIO_WritePin(EEPROM_WP_GPIO_Port, EEPROM_WP_Pin, GPIO_PIN_SET);

	for (uint8_t i=0; i<32; i++){
//		if (status == HAL_OK)
			HAL_I2C_Mem_Read(&hi2c2, 0xa0, adresRZ+32*p+i, 1, (uint8_t*)&odczyt[i], 1, HAL_MAX_DELAY);
	}

//	for (uint8_t i=0; i<32; i++){
//		size = sprintf(data, ": %d  zapi: %d\n\r", adresRZ+32*p+i, odczyt[i]);
//		HAL_UART_Transmit(&huart1, data, size, 100);
//	}
	for (uint8_t i = 0; i < 18; ++i) {
		_temp_name[i] = odczyt[i];
	}
	size = sprintf(data, "nazwa: %s\n\r", _temp_name);
	HAL_UART_Transmit(&huart1, data, size, 100);

	_temp_pcs = ((uint16_t)odczyt[18] << 8) | odczyt[19];
	_temp_pcs_done = ((uint16_t)odczyt[20] << 8) | odczyt[21];
	_temp_length = ((uint16_t)odczyt[22] << 8) | odczyt[23];
	_temp_knife = ((uint16_t)odczyt[28] << 8) | odczyt[29];
	_temp_knive_move_back = ((uint16_t)odczyt[30] << 8) | odczyt[31];
	usun(&L, p);
	wstaw(&L, p, _temp_name, _temp_pcs+1, _temp_pcs_done+7, _temp_length, odczyt[24], odczyt[25], odczyt[26], odczyt[27], _temp_knife, _temp_knive_move_back);
}

