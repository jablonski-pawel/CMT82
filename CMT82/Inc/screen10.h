/*
 * screen10.h
 *
 *  Created on: 23.11.2016
 *      Author: gorian
 */
#include "stm32f1xx_hal.h"

#ifndef SCREEN10_H_
#define SCREEN10_H_

void screen10_init();
int screen10_button();
void screen10_value_update(char c);
void screen10_action(int button);

#endif /* SCREEN10_H_ */
