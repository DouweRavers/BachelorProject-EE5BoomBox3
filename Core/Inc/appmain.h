/*
 * AppMain.h
 *
 *  Created on: Feb 25, 2021
 *      Author: douwe
 */

#ifndef INC_APPMAIN_H_
#define INC_APPMAIN_H_

#include "main.h"

void init_app(I2C_HandleTypeDef hi2c, TIM_HandleTypeDef htim); // set-up application
void tick_app(); // inf-loop of application


#endif /* INC_APPMAIN_H_ */
