/*
 * interface.h
 *
 *  Created on: Mar 4, 2021
 *      Author: douwe
 */

#ifndef INC_INTERFACE_H_
#define INC_INTERFACE_H_

#include "main.h"

extern uint8_t volume_level;

// public functions
void init_interface(); // set-up user interface
void tick_interface(uint32_t frame); // inf-loop of user interface
void interrupt_interface(uint16_t GPIO_Pin , ADC_HandleTypeDef *hadc1);
void interrupt_adc_interface(ADC_HandleTypeDef *hadc);

// defines
#define NO_SELECTION 5
#define SELECT 0
#define UP 1
#define RIGHT 2
#define LEFT 3
#define DOWN 4

#endif /* INC_INTERFACE_H_ */
