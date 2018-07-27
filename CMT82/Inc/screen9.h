/*
 * screen9.h
 *
 *  Created on: 18.11.2016
 *      Author: gorian
 */
#include "stm32f1xx_hal.h"

#ifndef SCREEN9_H_
#define SCREEN9_H_

void screen9_init();
int screen9_button();
void screen9_action(int button);

#endif /* SCREEN9_H_ */
