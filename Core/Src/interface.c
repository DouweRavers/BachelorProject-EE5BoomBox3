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
bool screen_updated = true; // indicator of when to rewrite screen output
enum screentype previous_screen = StartScreen, current_screen = StartScreen; // keep track of the current and previous screen
uint32_t screen_start_time = 0;
uint32_t value_adc;
unsigned char button;

// private functions declaration
void interface_update(uint32_t frame);
void screen_update(uint32_t frame);
void volume_update(uint32_t frame);
void buttons_update(unsigned  char frame);
void buttons_controller( uint32_t value);


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
	screen_update(frame);
}


void interrupt_interface(uint16_t GPIO_Pin , ADC_HandleTypeDef *hadc1)
{
	if(GPIO_Pin == GPIO_PIN_9)
	{
		if(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_10) == 1) volume_delta++;
		else volume_delta--;
	}

	if(GPIO_Pin == GPIO_PIN_8){
		HAL_ADC_Start_IT(hadc1);
	}
}

void interrupt_adc_interface(ADC_HandleTypeDef *hadc){
	    value_adc = HAL_ADC_GetValue(hadc);
	    buttons_controller ( value_adc);
	    HAL_ADC_Stop_IT(hadc);

}


// private functions
 void buttons_controller( uint32_t value){

	unsigned char button = NO_SELECTION;
	// I use a marge of +- 100
	if(value <= 100 ){
		button = NO_SELECTION;

	}
	else if ( value >= 1781 && value <= 1981){
		button = SELECT;
	}
	else if ( value >= 2109 && value <= 2309){
		button = UP;
	}

	else if (value >=  2432 && value <= 2632){
		button = RIGHT;
	}

	else if (value >= 2953 && value <= 3153){
		button= DOWN;
	}
	else if ( value >= 3700){
		button =  LEFT;
	}

	if(button != NO_SELECTION) buttons_update(button);

}

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
		char first[16], second[16];
		screens[current_screen].GetScreenOutput(frame, first, second);
		lcd_send_string(first);
		lcd_put_cur(1,0);
		lcd_send_string(second);
		screen_updated = false;
	}
}

void buttons_update(unsigned char button)
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
		uint8_t debug = volume_level;
		if(volume_delta != 0) {
			volume_delta = 0;
			screen_updated = true;
			if(current_screen != VolumeScreen) previous_screen = current_screen;
			current_screen = VolumeScreen;
			screen_start_time = frame;
		}
	}
}
