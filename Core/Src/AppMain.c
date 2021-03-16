/*
 * AppMain.c
 *
 *  Created on: Feb 25, 2021
 *      Author: douwe
 */

#include "AppMain.h"
#include "main.h"
#include "cmsis_os.h"

int iterate = 0;

/* Execute all configuration protocols */
void init_app()
{
	lcd_init ();
}

void tick_app()
{
	osDelay(750);
	lcd_clear();
	iterate++;
	if(iterate%2 == 0){
		lcd_put_cur(0, 0);
		lcd_send_string("Hello");
	}
	else {
		lcd_put_cur(1, 0);
		lcd_send_string("kitty");
	}
}
