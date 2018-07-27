/*
 * screen1.h
 *
 *  Created on: 17.11.2016
 *      Author: gorian
 */
#include "stm32f1xx_hal.h"

#ifndef SCREEN1_H_
#define SCREEN1_H_

void screen1_init(uint8_t hours, uint8_t minutes);
int screen1_button(void);
void screen1_action(int button);
void screen1_program_update(void);

#endif /* SCREEN1_H_ */
