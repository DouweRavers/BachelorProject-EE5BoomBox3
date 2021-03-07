/*
 * interface_windows.c
 *
 *  Created on: Mar 7, 2021
 *      Author: douwe
 */
#include "interface_screens.h"
#include "interface.h"

struct screen screens[2] = {
		{ VolumeScreen, &firstVolume, &secondVolume, &windowAtVolume },
		{ StartScreen, &firstStart, &secondStart, &windowAtStart }
};

// Volume screen
char* firstVolume(uint32_t frame){ return "     Volume"; }

char* secondVolume(uint32_t frame){
	char* vol = "                "; // 16 spaces
	for(int i=0;i<16;i++)
	{
		if(i * 20 < volume_level * 16 ) vol[i] = '\xff';
	}
	return vol;
}

enum screentype windowAtVolume(enum direction dir){
	// a button is pressed the volume screen should dissapear and the last menu one should appear
	return StartScreen;
}

// Start screen
char* firstStart(uint32_t frame){ return "    MainMenu"; }
char* secondStart(uint32_t frame){ return " In development"; }
enum screentype windowAtStart(enum direction dir){
	return VolumeScreen;
}


