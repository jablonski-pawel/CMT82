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
uint8_t ptr_scaling;

extern uint8_t feed_speed;
extern uint8_t blade_speed;
extern uint16_t batch_pieces;
extern uint8_t batch_delay;
extern float scaling;

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

	//zapisywanie nazwy
	ptr_name = L->name;
	localZero = 0;

	for(uint8_t i=0; i<18; i++){
		if(i<strlen(L->name)){
			if(HAL_I2C_Mem_Write_IT(&hi2c2, 0xa0, adresRZ+32*p+struct_size, 1, ptr_name+i, 1)!= HAL_OK){
				Error_Handler();
			}
			while (HAL_I2C_GetState(&hi2c2) != HAL_I2C_STATE_READY){}
		}else{
			if(HAL_I2C_Mem_Write_IT(&hi2c2, 0xa0, adresRZ+32*p+struct_size, 1, localZero, 1)!= HAL_OK){
				Error_Handler();
			}
			while (HAL_I2C_GetState(&hi2c2) != HAL_I2C_STATE_READY){}
		}
		HAL_Delay(5);
		struct_size+= 1;
	}

	//zapisywanie ilości sztuk
	temp_array[0] = (uint8_t)((L->pcs & 0xff00) >> 8);
	temp_array[1] = (uint8_t)L->pcs & 0xff;
	if(HAL_I2C_Mem_Write_IT(&hi2c2, 0xa0, adresRZ+32*p+struct_size, 1, temp_array, 1)!= HAL_OK){
		Error_Handler();
	}
	while (HAL_I2C_GetState(&hi2c2) != HAL_I2C_STATE_READY){}
	struct_size+=1;
	HAL_Delay(5);
	if(HAL_I2C_Mem_Write_IT(&hi2c2, 0xa0, adresRZ+32*p+struct_size, 1, temp_array+1, 1)!= HAL_OK){
		Error_Handler();
	}
	while (HAL_I2C_GetState(&hi2c2) != HAL_I2C_STATE_READY){}
	struct_size+=1;
	HAL_Delay(5);

	//zapisywanie ilości sztuk wykonanych
	temp_array[0] = (uint8_t)((L->pcs_done & 0xff00) >> 8);
	temp_array[1] = (uint8_t)L->pcs_done & 0xff;
	if(HAL_I2C_Mem_Write_IT(&hi2c2, 0xa0, adresRZ+32*p+struct_size, 1, temp_array, 1)!= HAL_OK){
		Error_Handler();
	}
	while (HAL_I2C_GetState(&hi2c2) != HAL_I2C_STATE_READY){}
	struct_size+=1;
	HAL_Delay(5);
	if(HAL_I2C_Mem_Write_IT(&hi2c2, 0xa0, adresRZ+32*p+struct_size, 1, temp_array+1, 1)!= HAL_OK){
		Error_Handler();
	}
	while (HAL_I2C_GetState(&hi2c2) != HAL_I2C_STATE_READY){}
	struct_size+=1;
	HAL_Delay(5);

	//zapisywanie długości przewodu
	temp_array[0] = (uint8_t)((L->length & 0xff00) >> 8);
	temp_array[1] = (uint8_t)L->length & 0xff;
	if(HAL_I2C_Mem_Write_IT(&hi2c2, 0xa0, adresRZ+32*p+struct_size, 1, temp_array, 1)!= HAL_OK){
		Error_Handler();
	}
	while (HAL_I2C_GetState(&hi2c2) != HAL_I2C_STATE_READY){}
	struct_size+= 1;
	HAL_Delay(5);
	if(HAL_I2C_Mem_Write_IT(&hi2c2, 0xa0, adresRZ+32*p+struct_size, 1, temp_array+1, 1)!= HAL_OK){
		Error_Handler();
	}

	while (HAL_I2C_GetState(&hi2c2) != HAL_I2C_STATE_READY){}
	struct_size+= 1;
	HAL_Delay(5);

	//zapisywanie lewego skórowania
	if(HAL_I2C_Mem_Write_IT(&hi2c2, 0xa0, adresRZ+32*p+struct_size, sizeof(L->left_cov), &L->left_cov, sizeof(L->left_cov))!= HAL_OK){
		Error_Handler();
	}
	while (HAL_I2C_GetState(&hi2c2) != HAL_I2C_STATE_READY){}
	struct_size+=sizeof(L->left_cov);
	HAL_Delay(5);

	//zapisywanie lewego oczka
	if(HAL_I2C_Mem_Write_IT(&hi2c2, 0xa0, adresRZ+32*p+struct_size, sizeof(L->left_eye), &L->left_eye, sizeof(L->left_eye))!= HAL_OK){
		Error_Handler();
	}
	while (HAL_I2C_GetState(&hi2c2) != HAL_I2C_STATE_READY){}
	struct_size+=sizeof(L->left_eye);
	HAL_Delay(5);

	//zapisywanie prawego oczka
	if(HAL_I2C_Mem_Write_IT(&hi2c2, 0xa0, adresRZ+32*p+struct_size, sizeof(L->right_eye), &L->right_eye, sizeof(L->right_eye))!= HAL_OK){
		Error_Handler();
	}
	while (HAL_I2C_GetState(&hi2c2) != HAL_I2C_STATE_READY){}
	struct_size+=sizeof(L->right_eye);
	HAL_Delay(5);

	//zapisywanie prawego skórowania
	if(HAL_I2C_Mem_Write_IT(&hi2c2, 0xa0, adresRZ+32*p+struct_size, sizeof(L->right_cov), &L->right_cov, sizeof(L->right_cov))!= HAL_OK){
		Error_Handler();
	}
	while (HAL_I2C_GetState(&hi2c2) != HAL_I2C_STATE_READY){}
	struct_size+=sizeof(L->right_cov);
	HAL_Delay(5);

	//zapisywanie średnicy rdzenia przewodu
	temp_array[0] = (uint8_t)((L->knife & 0xff00) >> 8);
	temp_array[1] = (uint8_t)L->knife & 0xff;
	if(HAL_I2C_Mem_Write_IT(&hi2c2, 0xa0, adresRZ+32*p+struct_size, 1, temp_array, 1)!= HAL_OK){
		Error_Handler();
	}
	while (HAL_I2C_GetState(&hi2c2) != HAL_I2C_STATE_READY){}
	struct_size+= 1;
	HAL_Delay(5);
	if(HAL_I2C_Mem_Write_IT(&hi2c2, 0xa0, adresRZ+32*p+struct_size, 1, temp_array+1, 1)!= HAL_OK){
		Error_Handler();
	}
	while (HAL_I2C_GetState(&hi2c2) != HAL_I2C_STATE_READY){}
	struct_size+= 1;
	HAL_Delay(5);

	//zapisywanie odskoku noży
	temp_array[0] = (uint8_t)((L->knife_move_back & 0xff00) >> 8);
	temp_array[1] = (uint8_t)L->knife_move_back & 0xff;
	if(HAL_I2C_Mem_Write_IT(&hi2c2, 0xa0, adresRZ+32*p+struct_size, 1, temp_array, 1)!= HAL_OK){
		Error_Handler();
	}
	while (HAL_I2C_GetState(&hi2c2) != HAL_I2C_STATE_READY){}
	struct_size+= 1;
	HAL_Delay(5);
	if(HAL_I2C_Mem_Write_IT(&hi2c2, 0xa0, adresRZ+32*p+struct_size, 1, temp_array+1, 1)!= HAL_OK){
		Error_Handler();
	}
	while (HAL_I2C_GetState(&hi2c2) != HAL_I2C_STATE_READY){}
	struct_size+= 1;
	HAL_Delay(5);

}

void odczyt_EEPROM(wezel *L, uint8_t p){

	uint16_t adresRZ = 100;//adresRozpoczeciaZapisu
	uint8_t odczyt[32];
	char _temp_name[18];

	uint16_t _temp_pcs, _temp_pcs_done, _temp_length, _temp_knife, _temp_knive_move_back;

	for (uint8_t i=0; i<32; i++){
		if(HAL_I2C_Mem_Read_IT(&hi2c2, 0xa0, adresRZ+32*p+i, 1, (uint8_t*)&odczyt[i], 1)!= HAL_OK){
			Error_Handler();
		}
		while (HAL_I2C_GetState(&hi2c2) != HAL_I2C_STATE_READY){}
	}

	for (uint8_t i = 0; i < 18; i++) {
		_temp_name[i] = odczyt[i];
	}

	_temp_pcs = ((uint16_t)odczyt[18] << 8) | odczyt[19];
	_temp_pcs_done = ((uint16_t)odczyt[20] << 8) | odczyt[21];
	_temp_length = ((uint16_t)odczyt[22] << 8) | odczyt[23];
	_temp_knife = ((uint16_t)odczyt[28] << 8) | odczyt[29];
	_temp_knive_move_back = ((uint16_t)odczyt[30] << 8) | odczyt[31];
	usun(&L, p);
	wstaw(&L, p, _temp_name, _temp_pcs, _temp_pcs_done, _temp_length, odczyt[24], odczyt[25], odczyt[26], odczyt[27], _temp_knife, _temp_knive_move_back);
}

//usuwanie z eeproma
void usun_z_EEPROM(wezel *L, uint8_t node_to_delete){

	uint8_t new_p_max = 0;
	uint8_t p_eeprom = 0;
	uint16_t adresRZ = 100;//adresRozpoczeciaZapisu
	uint8_t temp_odczyt;

	if(HAL_I2C_Mem_Read_IT(&hi2c2, 0xa0, 99, 1, &p_eeprom, 1)!= HAL_OK){
		Error_Handler();
	}
	while (HAL_I2C_GetState(&hi2c2) != HAL_I2C_STATE_READY){}
	for(uint8_t i=node_to_delete; i<p_eeprom-1; i++){
		for(uint8_t j=0; j<32; j++){

			if(HAL_I2C_Mem_Read_IT(&hi2c2, 0xa0, adresRZ+32*(i+1)+j, 1, &temp_odczyt, 1)!= HAL_OK){
				Error_Handler();
			}
			while (HAL_I2C_GetState(&hi2c2) != HAL_I2C_STATE_READY){}

			if(HAL_I2C_Mem_Write_IT(&hi2c2, 0xa0, adresRZ+32*i+j, 1, &temp_odczyt, 1)!= HAL_OK){
				Error_Handler();
			}
			while (HAL_I2C_GetState(&hi2c2) != HAL_I2C_STATE_READY){}
			HAL_Delay(5);

		}
	}

	//zmiana ilosci programow po usunieciu
	new_p_max = ilosc_wezlow(L);
	if(HAL_I2C_Mem_Write_IT(&hi2c2, 0xa0, 99, 1, &new_p_max, 1)!= HAL_OK){
		Error_Handler();
	}
	while (HAL_I2C_GetState(&hi2c2) != HAL_I2C_STATE_READY){}

}

void odczyt_ustawien_EEPROM(){

	uint8_t odczyt[10];
	uint8_t przecinek;
	uint16_t calkowita;
	uint16_t potega;
	uint8_t licznik;
	uint8_t temp_liczba;
	uint8_t odczytScaling[5];
	float tempFloat;


	for (uint8_t i=1; i<6; i++){	//tu od 1 bo zapisuje od 1
		if(HAL_I2C_Mem_Read_IT(&hi2c2, 0xa0, i, 1, (uint8_t*)&odczyt[i-1], 1)!= HAL_OK){ //i-1 zeby bylo od 0
			Error_Handler();
		}
		while (HAL_I2C_GetState(&hi2c2) != HAL_I2C_STATE_READY){}
	}

	feed_speed = odczyt[0];
	blade_speed = odczyt[1];
	batch_pieces = ((uint16_t)odczyt[2] << 8) | (uint16_t)odczyt[3];
	batch_delay = odczyt[4];

	for(uint8_t i=6; i<11; i++){	//odczytaj scaling z eeprom
		if(HAL_I2C_Mem_Read_IT(&hi2c2, 0xa0, i, 1, (uint8_t*)&odczytScaling[i-6], 1)!= HAL_OK){ //i-1 zeby bylo od 0
			Error_Handler();
		}
		while (HAL_I2C_GetState(&hi2c2) != HAL_I2C_STATE_READY){}
	}

	przecinek = 0;
	for(uint8_t c=0; c<5; c++){	//ustal lokalizacje przecinka
		if (odczytScaling[c] >= 48 && odczytScaling[c] <= 57){
		}else{
			przecinek=c;
			break;
		}
	}
	licznik=0;
	calkowita = 0;
	for(int8_t i=przecinek-1; i>=0; i--){	//sumuj kolejno setki, dziesiatki, jednosci
	    potega=1;
	    for(uint8_t j=0; j<i; j++){ //licz potegi 10
	        potega=potega*10;
	    }
	    temp_liczba=odczytScaling[licznik]-'0';
	    calkowita=calkowita+temp_liczba*potega;
	    licznik++;
	}
	temp_liczba=odczytScaling[przecinek+1]- '0';	//uwzglednij liczbe po przecinku
	tempFloat=1.0*((float)temp_liczba/100)*10;
	scaling = 1.0*calkowita+tempFloat;

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
