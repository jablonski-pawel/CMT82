/*
 * init.h
 *
 *  Created on: 16.11.2016
 *      Author: gorian
 */
#include "main.h"
#include "stm32f1xx_hal.h"
#include <stdlib.h>

#ifndef INIT_H_
#define INIT_H_

void wlaczanie(huart1, huart2);
int * XY_position(char *str[]);
void bazowanie(void);

#endif /* INIT_H_ */
