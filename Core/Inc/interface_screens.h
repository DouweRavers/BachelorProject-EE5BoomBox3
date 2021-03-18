/*
 * interface_screens.h
 *
 *  Created on: Mar 7, 2021
 *      Author: douwe
 */

#ifndef INC_INTERFACE_SCREENS_H_
#define INC_INTERFACE_SCREENS_H_

#include <stdbool.h>
#include "main.h"

enum direction{  // button pressed
	Up, Down, Left, Right, Enter, NoSelection
};
enum screentype{ // different screen types
	VolumeScreen = 0, StartScreen = 1
};

struct screen { // A struct that contains screen behavior
	enum screentype type; // which screen type is it (same as location in array)
	void (*GetScreenOutput)(uint32_t frame, char * first, char * second); // string to display on LCD (first is first line, second is second line) (frame used for animation)
	enum screentype (*GetWindowAt)(enum direction dir); // control button behavior
	bool popup;
};

struct screen screens[2]; // an array with all the screen configurations



#endif /* INC_INTERFACE_SCREENS_H_ */
