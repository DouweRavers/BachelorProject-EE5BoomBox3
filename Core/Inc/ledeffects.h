/*
 * interface.h
 *
 *  Created on: Apr 20, 2021
 *      Author: douwe
 */

#ifndef INC_LEDEFFECTS_H_
#define INC_LEDEFFECTS_H_

#include <main.h>

enum Color{ Black, Green, Red, Yellow };

void init_led(TIM_HandleTypeDef *htim); // set-up led screens
void tick_led(uint32_t frame); // inf-loop for led screens
void interrupt_timer_led(TIM_HandleTypeDef *htim); // handles timer interrupts


#endif /* INC_INTERFACE_H_ */
