/*
 * screen11.h
 *
 *  Created on: 25.01.2017
 *      Author: gorian
 */
#include "stm32f1xx_hal.h"
#include <stdlib.h>

#ifndef SCREEN11_H_
#define SCREEN11_H_

void screen11_init();
int screen11_button();
void screen11_action(int button);
void screen11_value_update(char c);

#endif /* SCREEN11_H_ */
