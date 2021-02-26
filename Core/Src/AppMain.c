/*
 * AppMain.c
 *
 *  Created on: Feb 25, 2021
 *      Author: douwe
 */

#include "AppMain.h"
#include "main.h"
#include "cmsis_os.h"

int row=0;
int col=0;

/* Execute all configuration protocols */
void init_app()
{
	lcd_init ();
	lcd_put_cur(0, 0);
	lcd_send_string("LCD DEMO");

	lcd_put_cur(1, 0);
	lcd_send_string("Hello world");
}

void tick_app()
{
	osDelay(500);
}
