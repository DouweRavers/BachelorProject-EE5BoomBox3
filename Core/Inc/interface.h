/*
 * interface.h
 *
 *  Created on: Mar 4, 2021
 *      Author: douwe
 */

#ifndef INC_INTERFACE_H_
#define INC_INTERFACE_H_

#include <stdbool.h>
#include "main.h"

extern uint8_t volume_level;
extern bool LED_enabled;

// public functions
void init_interface(TIM_HandleTypeDef *htim); // set-up user interface
void tick_interface(uint32_t frame); // inf-loop of user interface
void interrupt_IO_interface(uint16_t GPIO_Pin , ADC_HandleTypeDef *hadc1); // handles IO interrupts
void interrupt_adc_interface(ADC_HandleTypeDef *hadc); // handles adc interrupts
void interrupt_timer_interface(TIM_HandleTypeDef *htim);

#endif /* INC_INTERFACE_H_ */
