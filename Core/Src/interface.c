/*
 * interface.c
 *
 *  Created on: Mar 4, 2021
 *      Author: douwe
 */


#include <stdbool.h>
#include "cmsis_os.h"
#include "LCD1602.h"

#include "interface.h"
#include "interface_screens.h"

// public variables
uint8_t volume_level = 0; // between 0 - 20

// private variables
int volume_delta = 0; // volume wheel went up/down
bool screen_updated = false; // indicator of when to rewrite screen output
enum screentype previous_screen = StartScreen, current_screen = StartScreen; // keep track of the current and previous screen
uint32_t screen_start_time = 0;

// private functions declaration
void interface_update(uint32_t frame);
void screen_update(uint32_t frame);
void volume_update(uint32_t frame);
void buttons_update(uint32_t frame);

// public functions
void init_interface()
{
	lcd_init ();
}

void tick_interface(uint32_t frame)
{
	// at 30Hz the uint of frame will loop back after 100 years
	interface_update(frame);
	volume_update(frame);
	buttons_update(frame);
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
void interface_update(uint32_t frame)
{

}


void screen_update(uint32_t frame)
{
	if(current_screen == VolumeScreen && frame - screen_start_time > 60)
	{
		current_screen = previous_screen;
		screen_updated = true;
	}
	if(screen_updated)
	{
		lcd_clear();
		lcd_send_string(screens[current_screen].GetFirstLine(frame));
		lcd_put_cur(1,0);
		lcd_send_string(screens[current_screen].GetSecondLine(frame));
		screen_updated = false;
	}
}

void buttons_update(uint32_t frame)
{
	screen_updated = true;
}

// Volume wheel driver. Handles input of volume wheel.
void volume_update(uint32_t frame)
{
	if(frame % 2 == 0) // prescaler (checks every 2 frames (15Hz))
	{
		if(volume_delta > 0 && volume_level < 20) { volume_level++; }
		else if(volume_delta < 0 && volume_level > 0) { volume_level--; }
		if(volume_delta != 0) {
			volume_delta = 0;
			screen_updated = true;
			if(current_screen != VolumeScreen) previous_screen = current_screen;
			current_screen = VolumeScreen;
			screen_start_time = frame;
		}
	}
}
