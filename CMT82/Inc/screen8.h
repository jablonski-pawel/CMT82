/*
 * screen8.h
 *
 *  Created on: 18.11.2016
 *      Author: gorian
 */
#include "stm32f1xx_hal.h"

#ifndef SCREEN8_H_
#define SCREEN8_H_

void screen8_init();
int screen8_button();
void screen8_action(int button);

#endif /* SCREEN8_H_ */
