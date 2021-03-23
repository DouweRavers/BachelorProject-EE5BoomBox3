/*
 * interface_windows.c
 *
 *  Created on: Mar 7, 2021
 *      Author: douwe
 */

#include "interface_screens.h"
#include "interface.h"
#include <string.h>


/******************************
 * 		declarations
 *****************************/

/***********************
 * 		Pop up
 */

void getScreenOutputVolume(uint32_t frame, char * first, char * second); // volume popup
void getScreenOutputBluetoothConnect(uint32_t frame, char * first, char * second); // bluetooth connect popup
void getScreenOutputBluetoothDisconnect(uint32_t frame, char * first, char * second); // bluetooth disconnect popup
struct buttonaction onButtonPressedPopUp(enum direction dir); //

/***********************
 * 		Menu tabs
 */

// start screen
void getScreenOutputStart(uint32_t frame, char * first, char * second);
struct buttonaction onButtonPressedStart(enum direction dir);

// bluetooth search screen
void getScreenOutputBluetoothSearch(uint32_t frame, char * first, char * second);
struct buttonaction onButtonPressedBluetoothSearch(enum direction dir);

// Enable LED screen
void getScreenOutputLed(uint32_t frame, char * first, char * second);
struct buttonaction onButtonPressedLed(enum direction dir);

// Shows song and artist names if known
void getScreenOutputPlay(uint32_t frame, char * first, char * second);
struct buttonaction onButtonPressedPlay(enum direction dir);

/********************
 * 		array
 ********************/

// screen array
struct screen screens[] = {
		{ VolumeScreen, &getScreenOutputVolume, &onButtonPressedPopUp, 60 },
		{ BluetoothConnectScreen, &getScreenOutputBluetoothConnect, &onButtonPressedPopUp, 60 },
		{ BluetoothDisconnectScreen, &getScreenOutputBluetoothDisconnect, &onButtonPressedPopUp, 60 },
		{ StartScreen, &getScreenOutputStart, &onButtonPressedStart, 0 },
		{ BluetoothScreen, &getScreenOutputBluetoothSearch, &onButtonPressedBluetoothSearch, 0 },
		{ LedScreen, &getScreenOutputLed, &onButtonPressedLed, 0 },
		{ PlayScreen, &getScreenOutputPlay, &onButtonPressedPlay, 0 }

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

// bluetooth connected popup
void getScreenOutputBluetoothConnect(uint32_t frame, char * first, char * second)
{
	char r_first[] = "Bluetooth device";
	char r_second[] = "connected!";
	strncpy(first, r_first, 16);
	strncpy(second, r_second, 16);
}

// bluetooth disconnect popup
void getScreenOutputBluetoothDisconnect(uint32_t frame, char * first, char * second)
{
	char r_first[] = "Bluetooth device";
	char r_second[] = "Disconnected!";
	strncpy(first, r_first, 16);
	strncpy(second, r_second, 16);
}

/***********************
 * 		Menu tabs
 */

// Start screen
void getScreenOutputStart(uint32_t frame, char * first, char * second)
{
	char r_first[] = "    MainMenu";
	char r_second[] = "\x7f L    ^P^   B \x7e";
	strncpy(first, r_first, 16);
	strncpy(second, r_second, 16);
}

struct buttonaction onButtonPressedStart(enum direction dir)
{
	struct buttonaction response = { GoToScreen, 0 };
	switch(dir)
	{
	case Left:
		response.screen = LedScreen;
			break;
	case Right:
		response.screen = BluetoothScreen;
			break;
	case Up:
		response.screen = PlayScreen;
		break;
	default:
		response.action = NoBehavior;
		break;
	}
	return response;
}

// When then device searches for a bluetooth device it goes to this screen.
void getScreenOutputBluetoothSearch(uint32_t frame, char * first, char * second)
{
	char r_first[] = "   Bluetooth";
	char r_second[] = "\x7f M Searching";
	for(int i=1;i<=3;i++){
		if(frame%60 > 15*i) strcat(".", r_second);
	}
	strncpy(first, r_first, 16);
	strncpy(second, r_second, 16);
}

struct buttonaction onButtonPressedBluetoothSearch(enum direction dir)
{
	struct buttonaction response = { GoToScreen, 0 };
	switch(dir)
	{
	case Left:
		response.screen = StartScreen;
		break;
	default:
		response.action = NoBehavior;
		break;
	}
	return response;
}

// a configuration screen which (dis)enables the led effects
void getScreenOutputLed(uint32_t frame, char * first, char * second)
{
	char r_first[] = "   LED effects";
	char r_second[] = "\x7f E Disabled M \x7e";
	if(LED_enabled) strcpy(r_second, "\x7f D  Enabled M \x7e");
	strncpy(first, r_first, 16);
	strncpy(second, r_second, 16);
}

struct buttonaction onButtonPressedLed(enum direction dir)
{
	struct buttonaction response = { GoToScreen, 0 };
	switch(dir)
	{
	case Enter:
	case Left:
		LED_enabled = !LED_enabled;
		response.screen = LedScreen; // update screen
		break;
	case Right:
		response.screen = StartScreen;
		break;
	default:
		response.action = NoBehavior;
		break;
	}
	return response;
}

void getScreenOutputPlay(uint32_t frame, char * first, char * second)
{
	char s_dummy[] = "Songname";
	char a_dummy[] = "Artistname that is to long";

	char r_first[17] = "S: ";
	char r_second[17] = "A: ";
	int s_lenght = strlen(s_dummy); // song lenght
	int a_lenght = strlen(a_dummy); // artist lenght
	if(s_lenght <= 14) strcat(r_first, s_dummy);
	else{
		uint8_t s_offset = (frame%(s_lenght*30))/30-1;
		for(int i=0;i<13;i++){
			if(s_offset+i < s_lenght) r_first[3+i] = s_dummy[s_offset+i];
		}
	}
	if(a_lenght <= 14) strcat(r_second, a_dummy);
	else{
		uint8_t a_offset = (frame%(a_lenght*30))/30-1;
		for(int i=0;i<13;i++){
			if(a_offset+i < a_lenght) r_second[3+i] = a_dummy[a_offset+i];
		}
	}
	strncpy(first, r_first, 16);
	strncpy(second, r_second, 16);
}

struct buttonaction onButtonPressedPlay(enum direction dir)
{
	struct buttonaction response = { GoToScreen, 0 };
	switch(dir)
	{
	case Down:
			response.screen = StartScreen;
			break;
	default:
		response.screen = PlayScreen;
		break;
	}
	return response;
}
