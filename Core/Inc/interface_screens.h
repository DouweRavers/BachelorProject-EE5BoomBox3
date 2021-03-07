/*
 * interface_screens.h
 *
 *  Created on: Mar 7, 2021
 *      Author: douwe
 */

#ifndef INC_INTERFACE_SCREENS_H_
#define INC_INTERFACE_SCREENS_H_

#include "main.h"

enum direction{Up, Down, Left, Right, Enter}; // button pressed
enum screentype{ // different screen types
	VolumeScreen = 0, StartScreen = 1
};
struct screen { // A struct that contains screen behavior
	enum screentype type; // which screen type is it (same as location in array)
	char* (*GetFirstLine)(uint32_t frame); // string to first line LCD (frame used for animation)
	char* (*GetSecondLine)(uint32_t frame); // string to second line LCD (frame used for animation)
	enum screentype (*GetWindowAt)(enum direction dir); // control button behavior
};
struct screen screens[2]; // an array with all the screen configurations

// volume screen
char* firstVolume(uint32_t frame);
char* secondVolume(uint32_t frame);
enum screentype windowAtVolume(enum direction dir);
// start screen
char* firstStart(uint32_t frame);
char* secondStart(uint32_t frame);
enum screentype windowAtStart(enum direction dir);



#endif /* INC_INTERFACE_SCREENS_H_ */
