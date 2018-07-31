/*
 * screen5.h
 *
 *  Created on: 25.11.2016
 *      Author: gorian
 */
#include "stm32f1xx_hal.h"

#ifndef SCREEN5_H_
#define SCREEN5_H_

void screen5_init();
int screen5_button();
void screen5_value_update(char c);
void screen5_action(int button);
void screen5_plus_minus(uint8_t option);

#endif /* SCREEN5_H_ */
