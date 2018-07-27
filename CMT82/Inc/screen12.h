/*
 * screen12.h
 *
 *  Created on: 20.11.2016
 *      Author: gorian
 */
#include "stm32f1xx_hal.h"

#ifndef SCREEN12_H_
#define SCREEN12_H_

struct awg{
	int awg_no;
	char dia[8];
	char sect[8];
} awg_struct[25];

void screen12_init();
int screen12_button();
void screen12_action(int button);
void screen12_AWG_struct_fill(void);
void screen12_AWG_table(int dir);

#endif /* SCREEN12_H_ */
