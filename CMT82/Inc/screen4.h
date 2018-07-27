/*
 * screen4.h
 *
 *  Created on: 25.11.2016
 *      Author: gorian
 */
#include "stm32f1xx_hal.h"

#ifndef SCREEN4_H_
#define SCREEN4_H_

void screen4_init();
int screen4_button();
void screen4_value_update(char c);
void screen4_action(int button);
void screen4_plus_minus(uint8_t option);

#endif /* SCREEN4_H_ */
