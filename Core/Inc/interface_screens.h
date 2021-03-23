/*
 * interface_screens.h
 *
 *  Created on: Mar 7, 2021
 *      Author: douwe
 */

#ifndef INC_INTERFACE_SCREENS_H_
#define INC_INTERFACE_SCREENS_H_

#include "main.h"

enum direction{  // button pressed
	Up, Down, Left, Right, Enter, NoSelection
};
enum screentype{ // different screen types
	VolumeScreen=0, BluetoothConnectScreen=1, BluetoothDisconnectScreen=2,
	StartScreen=3, BluetoothScreen=4, LedScreen=5, PlayScreen=6
}; // manually set values so they can be used as index

enum actions{
	NoBehavior, GoToPreviousScreen, GoToScreen
};

struct buttonaction {
	enum actions action;
	enum screentype screen; // for action GoToScreen
};

struct screen { // A struct that contains screen behavior
	enum screentype type; // which screen type is it (same as location in array)
	void (*GetScreenOutput)(uint32_t frame, char * first, char * second); // string to display on LCD (first is first line, second is second line) (frame used for animation)
	struct buttonaction (*OnButtonPressed)(enum direction dir); // control button behavior
	uint16_t popup_time;
};

struct screen screens[7]; // an array with all the screen configurations

#endif /* INC_INTERFACE_SCREENS_H_ */
