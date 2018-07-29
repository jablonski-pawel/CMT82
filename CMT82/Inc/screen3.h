/*
 * screen3.h
 *
 *  Created on: 18.11.2016
 *      Author: gorian
 */
#include "stm32f1xx_hal.h"

#ifndef SCREEN3_H_
#define SCREEN3_H_

void screen3_init();
int screen3_button();
void screen3_action(int button);
void screen3_table(int dir);

#endif /* SCREEN3_H_ */
