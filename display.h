

#ifndef LIB_DISPLAY_H_
#define LIB_DISPLAY_H_

#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"

void pulseLCD(void);
void sendByte(char, int);
void setCursorPositionLCD(char, char);
void clearLCD(void);
void initLCD(void);
void printLCD(char*);
void setBlockCursorLCD(void);
void setLineCursorLCD(void);
void cursorOnLCD(void);
void cursorOffLCD(void);
void displayOffLCD(void);
void displayOnLCD(void);
void homeLCD(void);

#define FALSE 0
#define TRUE 1


#define RS GPIO_PIN_4 // Pin 5
#define EN GPIO_PIN_5 // Pin 6
#define D4 GPIO_PIN_0 // Pin 23
#define D5 GPIO_PIN_1 // Pin 24
#define D6 GPIO_PIN_2 // Pin 25
#define D7 GPIO_PIN_3 // Pin 26
#define ALLDATAPINS  D7 | D6 | D5 | D4
#define ALLCONTROLPINS RS | EN

#define DATA_PORT_BASE GPIO_PORTD_BASE
#define CMD_PORT_BASE GPIO_PORTE_BASE
#define DATA_PERIPH SYSCTL_PERIPH_GPIOD
#define CMD_PERIPH SYSCTL_PERIPH_GPIOE

#endif
