/*
 * ledeffects.c
 *
 *  Created on: Apr 20, 2021
 *      Author: douwe
 */

#include <math.h>
#include <stdbool.h>
#include "ledeffects.h"
#include "interface.h"
/***************************
 *	public variables
 **************************/


/***************************
 *	private variables
 **************************/

// timer
TIM_HandleTypeDef *timer_handle;

// screen
uint8_t screen[256];
uint8_t screen_iterator = 0;
bool timer_on = true;
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
	enum Color debugscreen[70];
	for(int i=0;i<70;i++){
		debugscreen[i] = Yellow;
	}
	colors_to_screen(debugscreen);
	do {
		if(timer6owner == NULL) timer6owner = osThreadGetId();
		else osDelay(10);
	} while(timer6owner != osThreadGetId());
}

void tick_led(uint32_t frame)
{

}

void interrupt_timer_led(TIM_HandleTypeDef *htim)
{
	if(timer6owner == osThreadGetId() && htim == timer_handle){
		__HAL_TIM_SET_COUNTER(timer_handle, 0xffff-13440); // second argument is setting the timer 13440 ticks away from overflow
		HAL_TIM_Base_Start_IT(timer_handle);
		if(timer_on){ // line counter iterates at this point
			HAL_GPIO_WritePin(LED_CLK_GPIO_Port, LED_CLK_Pin, 0);
			HAL_GPIO_WritePin(LED_DATA_GPIO_Port, LED_DATA_Pin, screen[screen_iterator]);
			screen_iterator++; // because 8bit it will automaticly reset to 0 after 256 counts
		} else { // shift register reads at this point
			HAL_GPIO_WritePin(LED_CLK_GPIO_Port, LED_CLK_Pin, 1);
		}
		timer_on = !timer_on;

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

			screen[i] = value[0]; // assign color to array
			screen[i+1] = value[1];
		} else {
			screen[i] = 0; // outside info just black
			screen[i+1] = 0;
		}
	}
}



