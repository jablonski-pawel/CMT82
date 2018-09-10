/*
 * lista.h
 *
 *  Created on: 28.11.2016
 *      Author: gorian
 */
#include "stm32f1xx_hal.h"

#ifndef LISTA_H_
#define LISTA_H_

typedef struct wezel {
	char name[18];
	uint16_t pcs;
	uint16_t pcs_done;
	uint16_t length;
	uint8_t left_cov;
	uint8_t left_eye;
	uint8_t right_eye;
	uint8_t right_cov;
	uint16_t knife;
	uint16_t knife_move_back;
    struct wezel *nastepny;
} wezel;

enum {
    POPRAWNIE = 0,
    BRAK_PAMIECI = -1,
    BLAD_IO = -2,
    BLAD_POZYCJI = -3
};

uint8_t wstawPocz(wezel **L, char name[18], uint16_t pcs, uint16_t pcs_done, uint16_t length, uint8_t left_cov, uint8_t left_eye, uint8_t right_eye, uint8_t right_cov, uint16_t knife, uint16_t knife_move_back);
uint8_t wstaw(wezel **L, uint8_t p, char name[18], uint16_t pcs, uint16_t pcs_done, uint16_t length, uint8_t left_cov, uint8_t left_eye, uint8_t right_eye, uint8_t right_cov, uint16_t knife, uint16_t knife_move_back);
uint8_t usun(wezel **L, uint8_t p);
uint8_t zwroc(wezel *L, uint8_t p, char *name[18], uint16_t *pcs, uint16_t *pcs_done, uint16_t *length, uint8_t *left_cov, uint8_t *left_eye, uint8_t *right_eye, uint8_t *right_cov, uint16_t *knife, uint16_t *knife_move_back);
//uint8_t znajdz(wezel *L, uint8_t *p, uint8_t wartosc);
void wyczysc(wezel **L);
uint16_t ilosc_wezlow(wezel *L);
void wypisz(wezel *L);
//uint8_t zapisz(wezel *L, char *nazwaPliku);
//uint8_t wczytaj(wezel **L, char *nazwaPliku);
void zapisz_EEPROM(wezel *L, uint8_t p);
void odczyt_EEPROM(wezel *L, uint8_t p);
void usun_z_EEPROM(wezel *L, uint8_t node_to_delete);
void odczyt_ustawien_EEPROM();
#endif /* LISTA_H_ */
