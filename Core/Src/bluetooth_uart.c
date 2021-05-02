/*
 * bluetooth_uart.c
 *
 *  Created on: Mar 28, 2021
 *      Author: arne
 */

#include <stdio.h>
#include <string.h>
#include "bluetooth_uart.h"

void enterPairingModeFromOff(){
	uint8_t parameter[2] = { 0x00 , 0x50};
	sendCommand(0x02 , parameter , 0x03);
}
void changeDeviceName(char *name  ){
    uint8_t length = (uint8_t)strlen(name) + 1 ;
	sendCommand(0x05 ,  name , length);               // length is 33 bytes = 0x21
}
void powerOn(){
	uint8_t parameter [2] = {0x00 , 0x51};
	sendCommand(0x02 , parameter , 0x03);

	parameter[1] = 0x52 ;
	sendCommand(0x02 , parameter , 0x03);
}

void playPauseMusic(){
	uint8_t parameter[2] = { 0x00 , 0x32};
	sendCommand(0x02 , parameter , 0x03);

	HAL_GPIO_WritePin(GPIOB , GPIO_PIN_8 , GPIO_PIN_SET );

}

void readBtmVersion(){
	uint8_t parameter[1] = {0x00};
	sendCommand(0x08 , parameter , 0x02);
	//readEvent() ;
	HAL_GPIO_WritePin(GPIOB , GPIO_PIN_8 , GPIO_PIN_SET );

}

void sendCommand(uint8_t opCode, uint8_t *parameter, uint8_t length){
	uint8_t command[ length + 4];
	HAL_GPIO_WritePin (GPIOA, GPIO_PIN_1,  GPIO_PIN_SET);
	makeCommand( command , opCode , parameter , length);
	HAL_Delay(3);
	HAL_StatusTypeDef status =  HAL_UART_Transmit ( getUartPointerBluetooth(), command, length+4, 100);
	HAL_Delay(2);
	HAL_GPIO_WritePin (GPIOA, GPIO_PIN_1,  GPIO_PIN_RESET);

}

int readEvent(){

	HAL_StatusTypeDef status;
	bm_event_t event ;
	int length = 0;
	uint8_t buffer[2]= {0};





    if(HAL_GPIO_ReadPin( GPIOA , GPIO_PIN_0)    == GPIO_PIN_RESET){

    	status =HAL_UART_Receive_DMA(getUartPointerBluetooth() , buffer , 2);
    	if(status == HAL_OK){

    	}


    }

	clearBussy();

	/*
	HAL_UART_Receive(getUartPointerBluetooth() , buffer , 10, 1000);
	switch(buffer[1] ){
	case 0x00:
		break;
	case 0xAA :
		return 1;
		break;
	case 0x18:
		return 1;

		break;

	default:
		return 1;
	}
}
	return -1;
*/

return -1;

}
uint8_t *makeCommand(uint8_t* command, uint8_t opCode, uint8_t* parameter, uint8_t length){
	uint8_t init[4] = { 0xAA , 0x00 , length , opCode};
	memcpy(command, init, 4);
	memcpy(&command[4], parameter, length);
	command[length + 3] = makeCheckSum( &command[2] , length + 1 );

	return command;


}

uint8_t makeCheckSum(uint8_t *target , uint8_t length){
	uint8_t check = 255 ;  // 0xFF
	for(int i=0; i<length; i++){
	        check -= target[i];
	    }
	check = check  + 1 ;   // checksum + target should be equal to 0x00
	return check;
}


