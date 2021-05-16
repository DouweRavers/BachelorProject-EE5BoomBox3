/*
 * interface_windows.c
 *
 *  Created on: Mar 7, 2021
 *      Author: douwe
 */

#include "interface_screens.h"
#include "interface.h"
#include "bluetooth_uart.h"
#include <string.h>


/******************************
 * 		declarations
 *****************************/

/***********************
 * 		Pop up
 */

void getScreenOutputVolume(uint32_t frame, char * first, char * second); // volume popup
struct buttonaction onButtonPressedPopUp(enum direction dir); //

/***********************
 * 		Menu tabs
 */

// start screen
void getScreenOutputStart(uint32_t frame, char * first, char * second);
struct buttonaction onButtonPressedStart(enum direction dir);

// Main menu screen
enum menu_selection { SongPlayer, LedMenu };
enum menu_selection current_menu_selection = SongPlayer;
void getScreenOutputMenu(uint32_t frame, char * first, char * second);
struct buttonaction onButtonPressedMenu(enum direction dir);

// Enable LED screen
enum led_selection { Enable, Mode, Exit };
enum led_selection current_led_selection = SongPlayer;
enum led_modes led_mode = Smooth;
void getScreenOutputLed(uint32_t frame, char * first, char * second);
struct buttonaction onButtonPressedLed(enum direction dir);

// Shows song and artist names if known
void getScreenOutputSong(uint32_t frame, char * first, char * second);
struct buttonaction onButtonPressedSong(enum direction dir);

/********************
 * 		array
 ********************/

// screen array
struct screen screens[] = {
		{ VolumeScreen, &getScreenOutputVolume, &onButtonPressedPopUp, 60 },
		{ StartScreen, &getScreenOutputStart, &onButtonPressedStart, 0 },
		{ MenuScreen, &getScreenOutputMenu, &onButtonPressedMenu, 0 },
		{ SongScreen, &getScreenOutputSong, &onButtonPressedSong, 0 },
		{ LedScreen, &getScreenOutputLed, &onButtonPressedLed, 0 }
};

/*****************************
 * 		implementations
 *****************************/

/***********************
 * 		Pop up
 */

// Volume popup
void getScreenOutputVolume(uint32_t frame, char * first, char * second)
{
	char r_first[] = "     Volume";
	char r_second[17];
	for(int i=0;i<16;i++)
	{
		if(i * 20 < volume_level * 16 ) r_second[i] = '\xff';
		else r_second[i] = '_';
	}
	r_second[16] = '\0';
	strncpy(first, r_first, 16);
	strncpy(second, r_second, 16);
}

struct buttonaction onButtonPressedPopUp(enum direction dir){
	struct buttonaction response = { GoToPreviousScreen, VolumeScreen };
	return response;
}

/***********************
 * 		Menu tabs
 */

// Start screen
void getScreenOutputStart(uint32_t frame, char * first, char * second)
{
	char r_first[17] = "   Welcome!";
	char r_second[17], message[] = "  Press any button to continue...";
	// frame based animation
	int m_lenght = strlen(message); // artist lenght
	uint8_t offset = (frame%(m_lenght*15))/15-1;
	for(int i=0;i<16;i++){
		if(offset+i < m_lenght) r_second[i] = message[offset+i];
	}
	strncpy(first, r_first, 16);
	strncpy(second, r_second, 16);
}

struct buttonaction onButtonPressedStart(enum direction dir)
{
	struct buttonaction response = { NoBehavior, 0 };
	if(dir != NoSelection){
		response.action = GoToScreen;
		response.screen = MenuScreen;
	}
	return response;
}

// Main menu different settings accessed from here
void getScreenOutputMenu(uint32_t frame, char * first, char * second)
{
	char r_first[17], r_second[17];
	// first line animation
	switch(frame/15 % 6){
	case 0:
			strncpy(r_first, " oooO Menu Oooo ", 16);
			break;
	case 1: case 5:
			strncpy(r_first, " ooOo Menu oOoo ", 16);
			break;
	case 2: case 4:
			strncpy(r_first, " oOoo Menu ooOo ", 16);
			break;
	case 3:
			strncpy(r_first, " Oooo Menu oooO ", 16);
			break;
	}

	// second line animation
	switch(current_menu_selection)
	{
		case SongPlayer:
			strncpy(r_second, "\x7f  Song Menu   \x7e", 16);
			break;
		case LedMenu:
			strncpy(r_second, "\x7f  LED  Menu   \x7e", 16);
			break;
		default:
			strncpy(r_second, "  xx Error xx  ", 16);
			break;
	}
	strncpy(first, r_first, 16);
	strncpy(second, r_second, 16);
}

struct buttonaction onButtonPressedMenu(enum direction dir)
{
	struct buttonaction response = { NoBehavior, 0 };
	switch(dir)
	{
	case Enter:
		response.action = GoToScreen;
		if(current_menu_selection == SongPlayer) response.screen = SongScreen;
		else if(current_menu_selection == LedMenu) response.screen = LedScreen;
		break;
	case Left: case Right:
		if(current_menu_selection == SongPlayer) current_menu_selection = LedMenu;
		else if(current_menu_selection == LedMenu) current_menu_selection = SongPlayer;
		break;
	default:
		response.action = NoBehavior;
		break;
	}
	return response;
}

// A side menu for enabling leds and setting animation type
void getScreenOutputLed(uint32_t frame, char * first, char * second)
{
	char r_first[17], r_second[17];
	// first line animation
	switch(led_mode){
		case Smooth:
			if (frame%2==0) strncpy(r_first, " \xCAv LED Menu \xCAv ", 16);
			else strncpy(r_first, " v\xCA LED Menu v\xCA ", 16);
			break;
		case Flicker:
			if (frame%2==0) strncpy(r_first, " ** LED Menu ** ", 16);
			else strncpy(r_first, " xx LED Menu xx ", 16);
			break;
		case MonoRed: case MonoGreen: case MonoYellow:
			strncpy(r_first, " ## LED Menu ## ", 16);
			break;
	}

	// second line animation
	switch(current_led_selection)
	{
		case Enable:
			if(LED_enabled) strncpy(r_second, "\x7f Disable LEDs \x7e", 16);
			else strncpy(r_second, "\x7f Enable  LEDs \x7e", 16);
			break;
		case Mode:
			switch(led_mode){
				case Smooth:
					if (frame%2==0) strncpy(r_second, "\x7f \xCAv Smooth \xCAv \x7e", 16);
					else strncpy(r_second, "\x7f v\xCA Smooth v\xCA \x7e", 16);
					break;
				case Flicker:
					if (frame%2==0) strncpy(r_second, "\x7f ** Flicker **\x7e", 16);
					else strncpy(r_second, "\x7f xx Flicker xx\x7e", 16);
					break;
				case MonoRed:
					strncpy(r_second, "\x7f   Mono Red   \x7e", 16);
					break;
				case MonoGreen:
					strncpy(r_second, "\x7f  Mono Green  \x7e", 16);
					break;
				case MonoYellow:
					strncpy(r_second, "\x7f Mono Yellow  \x7e", 16);
					break;
			}
			break;
		case Exit:
			strncpy(r_second, "\x7f  Exit  Menu  \x7e", 16);
			break;
		default:
			strncpy(r_second, "  xx Error xx  ", 16);
			break;
	}
	strncpy(first, r_first, 16);
	strncpy(second, r_second, 16);
}

struct buttonaction onButtonPressedLed(enum direction dir)
{
	struct buttonaction response = { NoBehavior, 0 };
	switch(dir)
	{
	case Enter:
		switch(current_led_selection)
		{
			case Enable:
				LED_enabled = !LED_enabled;
				break;
			case Exit:
				response.action = GoToScreen;
				response.screen = MenuScreen;
				break;
			default:
				break;
		}
		break;
	case Left:
		if(current_led_selection == Enable) current_led_selection = Exit;
		else current_led_selection--;
		break;
	case Right:
		if(current_led_selection == Exit) current_led_selection = Enable;
		else current_led_selection++;
		break;
	case Up:
		if(current_led_selection == Mode) {
			if(led_mode == Smooth) led_mode = MonoYellow;
			else led_mode--;
		}
		break;
	case Down:
		if(current_led_selection == Mode){
			if(led_mode == MonoYellow) led_mode = Smooth;
			else led_mode++;
		}
		break;
	default:
		break;
	}
	return response;
}

void getScreenOutputSong(uint32_t frame, char * first, char * second)
{
	char r_first[17], r_second[17] = "\x7fPrev >/|| Next\x7e";
	// first line animation
	switch(frame/15 % 6){
	case 0:
			strncpy(r_first, " oooO Song Oooo ", 16);
			break;
	case 1: case 5:
			strncpy(r_first, " ooOo Song oOoo ", 16);
			break;
	case 2: case 4:
			strncpy(r_first, " oOoo Song ooOo ", 16);
			break;
	case 3:
			strncpy(r_first, " Oooo Song oooO ", 16);
			break;
	}
	// Second line animation
	strncpy(first, r_first, 16);
	strncpy(second, r_second, 16);
}

struct buttonaction onButtonPressedSong(enum direction dir)
{
	struct buttonaction response = { NoBehavior, 0 };
	switch(dir)
	{
	case Up:
			response.action = GoToScreen;
			response.screen = MenuScreen;
			break;
	case Left:
		bmSwitchToPreviousSong();
		break;
	case Right:
		bmSwitchToNextSong();
		break;
	case Enter:
		bmPlayPauseMusic();
		break;
	default:
		break;
	}
	return response;
}
