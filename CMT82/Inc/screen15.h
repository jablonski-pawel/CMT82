/*
 * screen15.h
 *
 *  Created on: 18.11.2016
 *      Author: gorian
 */
#include "stm32f1xx_hal.h"

#ifndef SCREEN15_H_
#define SCREEN15_H_

void screen15_init();
int screen15_button();
void screen15_action(int button);

#endif /* SCREEN15_H_ */
