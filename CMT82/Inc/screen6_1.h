/*
 * screen6_1.h
 *
 *  Created on: 18.11.2016
 *      Author: gorian
 */
#include "stm32f1xx_hal.h"

#ifndef SCREEN6_1_H_
#define SCREEN6_1_H_

void screen6_1_init(uint8_t hours, uint8_t minutes);
int screen6_1_button();
void screen6_1_action(int button);

#endif /* SCREEN6_1_H_ */
