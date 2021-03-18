/*
 * interface_windows.c
 *
 *  Created on: Mar 7, 2021
 *      Author: douwe
 */
#include "interface_screens.h"
#include "interface.h"
#include <string.h>


// declarations

// volume screen
void getScreenOutputVolume(uint32_t frame, char * first, char * second);
enum screentype windowAtVolume(enum direction dir);
// start screen
void getScreenOutputStart(uint32_t frame, char * first, char * second);
enum screentype windowAtStart(enum direction dir);

// screen array
struct screen screens[2] = {
		{ VolumeScreen, &getScreenOutputVolume, &windowAtVolume, true },
		{ StartScreen, &getScreenOutputStart, &windowAtStart, false }
};


// Volume screen
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

enum screentype windowAtVolume(enum direction dir){
	// a button is pressed the volume screen should dissapear and the last menu one should appear
	return StartScreen;
}

// Start screen
void getScreenOutputStart(uint32_t frame, char * first, char * second)
{
	char r_first[] = "    MainMenu";
	char r_second[] = " In development";
	strncpy(first, r_first, 16);
	strncpy(second, r_second, 16);
}

enum screentype windowAtStart(enum direction dir){
	return VolumeScreen;
}


