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

/********************
 * 		array
 ********************/

// screen array
struct screen screens[] = {
		{ VolumeScreen, &getScreenOutputVolume, &onButtonPressedPopUp, true },
		{ BluetoothConnectScreen, &getScreenOutputBluetoothConnect, &onButtonPressedPopUp, true },
		{ BluetoothDisconnectScreen, &getScreenOutputBluetoothDisconnect, &onButtonPressedPopUp, true },
		{ StartScreen, &getScreenOutputStart, &onButtonPressedStart, false },
		{ BluetoothScreen, &getScreenOutputBluetoothSearch, &onButtonPressedBluetoothSearch, false }
		//{ LedScreen, &getScreenOutputStart, &onButtonPressedStart, false },
		//{ PlayScreen, &getScreenOutputStart, &onButtonPressedStart, false }

};

/******************************
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
	char r_second[] = "\xf7 L    ^P^   B \xe7";
	strncpy(first, r_first, 16);
	strncpy(second, r_second, 16);
}

struct buttonaction onButtonPressedStart(enum direction dir){
	struct buttonaction response = { GoToScreen, 0 };
	switch(dir)
	{
	case Left:
		response.screen = LedScreen;
			break;
	case Right:
		response.screen = BluetoothScreen;
			break;
	case Down:
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
	char r_second[] = "\xf7 M Searching";
	for(int i=1;i<=3;i++){
		if(frame%60 > 15*i) strcat(".", r_second);
	}
	strncpy(first, r_first, 16);
	strncpy(second, r_second, 16);
}

struct buttonaction onButtonPressedBluetoothSearch(enum direction dir){
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

