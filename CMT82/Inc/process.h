/*
 * process.h
 *
 *  Created on: 16.12.2016
 *      Author: gorian
 */
#include "stm32f1xx_hal.h"

#ifndef PROCESS_H_
#define PROCESS_H_

void process_run();
uint16_t process_mm2i(float mm);
uint8_t process_steppers_EN(uint8_t state);
uint8_t process_steppers_CW(uint8_t dir);
void process_cut(void);

#endif /* PROCESS_H_ */
