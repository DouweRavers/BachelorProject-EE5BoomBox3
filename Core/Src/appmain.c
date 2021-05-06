/*
 * appmain.c
 *
 *  Created on: Feb 25, 2021
 *      Author: douwe
 */

#include "appmain.h"
#include "main.h"
#include "cmsis_os.h"

const uint8_t DAC_ADDR = 0b10011000;
I2C_HandleTypeDef hi2c1;
TIM_HandleTypeDef htim2;

void init_app(I2C_HandleTypeDef hi2c, TIM_HandleTypeDef htim)
{
	hi2c1 = hi2c;
	htim2 = htim;
	setup_dac();
}

void tick_app()
{

}


void setup_dac()
{
	uint8_t reg_adr = 0b00010011, reg_data = 0b00000100; // 0 = reg_adr, 1 = reg_data
	uint16_t buf = reg_data;
	buf = buf << 8;
	buf += reg_adr;
	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1); // set up program clock for DAC
	HAL_Delay(2); // wait some time for the dac to power up. should be 1k clock cycles I only use
//	while(HAL_I2C_Master_Transmit(&hi2c1, DAC_ADDR, &buf, 2, HAL_MAX_DELAY) != HAL_OK){ }
}
