/*
 * interface.c
 *
 *  Created on: Mar 4, 2021
 *      Author: douwe
 */

#include "interface.h"
#include "cmsis_os.h"

// public variables
int volume = 0; // between 0 - 20

// private variables
int volume_delta = 0; // volume wheel went up/down
uint8_t screen_updated = 0; // change in display config

// private functions declaration
void interface_update();
void volume_update();
void screen_update();

// public functions
void init_interface()
{
	lcd_init ();
}

void tick_interface(uint32_t frame)
{
	volume_update(frame);
	screen_update(frame);
}


void interrupt_interface(uint16_t GPIO_Pin)
{
	if(GPIO_Pin == GPIO_PIN_9)
	{
		if(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_10) == 1) volume_delta++;
		else volume_delta--;
	}
}

// private functions
void screen_update(uint32_t frame)
{
	if(frame % 10 == 0) // prescaler
	{
		lcd_clear();
		lcd_send_string("Volume");
		lcd_put_cur(1,0);
		char vol[] = "                "; // 16 spaces
		for(int i=0;i<16;i++)
		{
			if(i * 20 < volume * 16 ) vol[i] = '\xff';
		}
		lcd_send_string(vol);
	}
}

void volume_update(uint32_t frame)
{
	if(frame % 2 == 0) // prescaler
	{
		if(volume_delta > 0)
		{
			volume++;
			volume_delta = 0;
		} else if(volume_delta < 0)
		{
			volume--;
			volume_delta = 0;
		}
	}
}
