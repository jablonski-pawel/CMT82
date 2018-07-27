/*
 * screen7.h
 *
 *  Created on: 18.11.2016
 *      Author: gorian
 */
#include "stm32f1xx_hal.h"

#ifndef SCREEN7_H_
#define SCREEN7_H_

void screen7_init();
int screen7_button();
void screen7_action(int button);

#endif /* SCREEN7_H_ */
