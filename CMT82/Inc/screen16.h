/*
 * screen16.h
 *
 *  Created on: 14.08.2018
 *      Author: gorian
 */
#include "stm32f1xx_hal.h"

#ifndef SCREEN16_H_
#define SCREEN16_H_

void screen16_init();
int screen16_button();
void screen16_action(int button);
void screen16_table(int dir);

#endif /* SCREEN16_H_ */
