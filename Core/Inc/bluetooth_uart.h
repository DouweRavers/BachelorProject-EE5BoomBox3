/*
 * bluetoothUart.h
 *
 *  Created on: Mar 28, 2021
 *      Author: arne
 */

#ifndef INC_BLUETOOTH_UART_H_
#define INC_BLUETOOTH_UART_H_


#include "main.h"

typedef enum
{
	BM64_EVENT_ACK = 0x00

} BM_EVENT;

typedef struct {
	BM_EVENT event;
	int data_length;
	uint8_t *data;

} bm_event_t;


uint8_t *makeCommand(uint8_t* command, uint8_t opCode, uint8_t* parameter, uint8_t length);
/*
 * makeCommand()
 * creates the uart command and returns it.
 */

uint8_t makeCheckSum(uint8_t *target , uint8_t length);
/*
 * makeCheckSum()
 * Calculates the check sum
 */

void sendCommand(uint8_t opCode, uint8_t *parameter, uint8_t length);
/*
 * senCommand()
 * this command will acually send the uart comand
 */

int readEvent();
/*
 * readEvent()
 * This command will be called when expecting an event from the BM
 * It will return a bm_event_t structure
 */

void playPauseMusic();
void readBtmVersion();
void enterPairingModeFromOff();
void powerOn();
void changeDeviceName(char *name  );
/*
 * changeDeviceName
 * This command wil change the name of the boombox .
 * The parameter taht should be send over uart is of a 32bytes size.
 */
#endif /* INC_BLUETOOTH_UART_H_ */
