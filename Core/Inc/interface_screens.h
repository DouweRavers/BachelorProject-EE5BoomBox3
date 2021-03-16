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
	void (*GetScreenOutput)(uint32_t frame, char * first, char * second); // string to display on LCD (first is first line, second is second line) (frame used for animation)
	enum screentype (*GetWindowAt)(enum direction dir); // control button behavior
};
struct screen screens[2]; // an array with all the screen configurations

// volume screen
void getScreenOutputVolume(uint32_t frame, char * first, char * second);
enum screentype windowAtVolume(enum direction dir);
// start screen
void getScreenOutputStart(uint32_t frame, char * first, char * second);
enum screentype windowAtStart(enum direction dir);


#endif /* INC_INTERFACE_SCREENS_H_ */
