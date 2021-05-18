/*
 * ledeffects.c
 *
 *  Created on: Apr 20, 2021
 *      Author: douwe
 */

#include <math.h>
#include "ledeffects.h"
#include "interface_screens.h"

/***************************
 *	public variables
 **************************/

/***************************
 *	private variables
 **************************/

// timer
TIM_HandleTypeDef *timer_handle;

// screen
uint8_t screen_io[256];
uint8_t screen_iterator = 0; // Don't change the screen iterator as the hardware will not reset with it..
bool clock_status = false;
bool mono_update = false;

/********************************
 *	private function declaration
 ********************************/

void colors_to_screen(enum Color colors[70]);

/***************************
 *	public functions
 **************************/

void init_led(TIM_HandleTypeDef *htim)
{
	timer_handle = htim;
	enum Color startscreen[70];
	for(int i=0;i<70;i++){
		startscreen[i] = i%4; // just assign all possible colors by iterating from 0 to 3
	}
	colors_to_screen(startscreen);
	HAL_TIM_Base_Start_IT(timer_handle);
}

void tick_led(uint32_t frame)
{
	if(LED_enabled){
		enum Color screen[70];
		switch(led_mode){
		case Smooth:
			for(int i=0;i<70;i++){
				switch(i%10){
				case 0: case 6: screen[i] = (frame/15)%4; break;
				case 1: case 5: case 7: screen[i] = (frame/15)%4 + 1; break;
				case 2: case 4: case 8:screen[i] = (frame/15)%4 + 2; break;
				case 3: case 9: screen[i] = (frame/15)%4 + 3; break;
				default: break;
				}
				if(screen[i] > 3) screen[i] -= 3;
			}
			break;
		case Flicker:
			for(int i=0;i<70;i++){
				screen[i] = (i%3) + ((frame/15)%2); // 4 colors color and location dependant.
			}
		break;
		case MonoRed:
			for(int i=0;i<70;i++) screen[i] = Red;
			break;
		case MonoGreen:
			for(int i=0;i<70;i++) screen[i] = Green;
			break;
		case MonoYellow:
			for(int i=0;i<70;i++) screen[i] = Yellow;
			break;
		default:
			for(int i=0;i<70;i++) screen[i] = Black;
			break;
		}
		colors_to_screen(screen);
	}
	else {
		enum Color darkscreen[70];
		for(int i=0;i<70;i++){
			darkscreen[i] = Black;
		}
		colors_to_screen(darkscreen);
	}
}

void interrupt_timer_led(TIM_HandleTypeDef *htim)
{
	if(htim == timer_handle){
		__HAL_TIM_SET_COUNTER(timer_handle, 0xffff-13440); // second argument is setting the timer 13440 ticks away from overflow
		if(clock_status){ // line counter iterates at this point
			HAL_GPIO_WritePin(LED_CLK_GPIO_Port, LED_CLK_Pin, 0);
			HAL_GPIO_WritePin(LED_DATA_GPIO_Port, LED_DATA_Pin, screen_io[screen_iterator]);
			screen_iterator++; // because 8bit it will automaticly reset to 0 after 256 counts
		} else { // shift register reads at this point
			HAL_GPIO_WritePin(LED_CLK_GPIO_Port, LED_CLK_Pin, 1);
		}
		clock_status = !clock_status;
		HAL_TIM_Base_Start_IT(timer_handle);
	}
}

/***************************
 *	private functions
 **************************/

void colors_to_screen(enum Color colors[70]){
	for(unsigned int i=0;i<256;i+=2){
		uint8_t line = floor(i/32);
		uint8_t character = i%32;

		if(8 <= character && character < 28){
			uint8_t place = line * 10; // y coordinate
			place += floor((character-8)/2); // x coordinate
			int8_t value[2] = {0,0}; // binary value for our colors
			switch(colors[place]){ // color to value
				case Black:
					value[0] = 0;
					value[1] = 0;
					break;
				case Green:
					value[0] = 1;
					value[1] = 0;
					break;
				case Red:
					value[0] = 0;
					value[1] = 1;
					break;
				case Yellow:
					value[0] = 1;
					value[1] = 1;
					break;
				default:
					break;
			}
			// if desired post processing can be done here...
			screen_io[i] = value[0]; // assign color to array
			screen_io[i+1] = value[1];
		} else {
			screen_io[i] = 0; // outside info just black
			screen_io[i+1] = 0;
		}
	}
}



