/*
 * screen6.h
 *
 *  Created on: 17.11.2016
 *      Author: gorian
 */
#include "stm32f1xx_hal.h"

#ifndef SCREEN6_H_
#define SCREEN6_H_

void screen6_init(uint8_t hours, uint8_t minutes);
int screen6_button(void);
void screen6_action(int button);
//void screen6_program_update(void);

#endif /* SCREEN6_H_ */
