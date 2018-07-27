/*
 * screen13.h
 *
 *  Created on: 20.11.2016
 *      Author: gorian
 */
#include "stm32f1xx_hal.h"

#ifndef SCREEN13_H_
#define SCREEN13_H_

void screen13_init();
int screen13_button();
void screen13_action(int button);

#endif /* SCREEN13_H_ */
