/*
 * AppMain.c
 *
 *  Created on: Feb 25, 2021
 *      Author: douwe
 */

#include "AppMain.h"
#include "main.h"
#include "cmsis_os.h"

int volume = 0;

/* Execute all configuration protocols */
void init_app()
{
	lcd_init ();
}

void tick_app()
{
	osDelay(30);
	lcd_clear();
	char vol[] = "                "; // 16 spaces
	for(int i=0;i<16;i++)
	{
		if(i * 20 < volume * 16 ) vol[i] = '\xff';
	}
	lcd_send_string(vol);
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_10) == 1)
	{
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_12, 1);
		if(volume < 20) volume++;
	}
	else
	{
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_12, 0);
		if(volume > 0) volume--;
	}

}
