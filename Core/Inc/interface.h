/*
 * interface.h
 *
 *  Created on: Mar 4, 2021
 *      Author: douwe
 */

#ifndef INC_INTERFACE_H_
#define INC_INTERFACE_H_

#include "main.h"

uint8_t volume_level;
// public functions
void init_interface(); // set-up user interface
void tick_interface(uint32_t frame); // inf-loop of user interface
void interrupt_interface(uint16_t GPIO_Pin);


#endif /* INC_INTERFACE_H_ */
