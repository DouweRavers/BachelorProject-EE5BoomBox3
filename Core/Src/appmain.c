/*
 * appmain.c
 *
 *  Created on: Feb 25, 2021
 *      Author: douwe
 */

#include "appmain.h"
#include "main.h"
#include "bluetooth_uart.h"

const uint8_t DAC_ADDR = 0b10011000;
I2C_HandleTypeDef * hi2c_handler;

void setup_dac();

void init_app(I2C_HandleTypeDef * hi2c)
{
	bmPowerOn();
	bmEnterPairingModeFromOff();
	hi2c_handler = hi2c;
	setup_dac();
}

void setup_dac()
{
	uint8_t reg_adr = 0b00010011, reg_data = 0b00000100; // 0 = reg_adr, 1 = reg_data
	uint16_t buf = reg_data;
	buf = buf << 8;
	buf += reg_adr;
	while(HAL_I2C_Master_Transmit(hi2c_handler, DAC_ADDR, &buf, 2, 5) != HAL_OK);
}
