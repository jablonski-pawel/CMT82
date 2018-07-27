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
