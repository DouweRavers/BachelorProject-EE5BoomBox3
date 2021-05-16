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

void setup_dac();

void init_app(I2C_HandleTypeDef hi2c)
{
	hi2c1 = hi2c;
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
}
