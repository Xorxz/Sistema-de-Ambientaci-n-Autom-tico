

#include "display.h"


void pulseLCD()
{

	GPIOPinWrite(CMD_PORT_BASE, EN, 0);
	GPIOPinWrite(CMD_PORT_BASE, EN, EN);
	GPIOPinWrite(CMD_PORT_BASE, EN, 0);
}



void setCmd() {
	GPIOPinWrite(CMD_PORT_BASE, RS,0);
}



void setData() {
	GPIOPinWrite(CMD_PORT_BASE, RS,RS);
}



void sendByte(char byteToSend, int isData)
{
	if (isData)
		setData();
	else
		setCmd();
	SysCtlDelay(400);
	GPIOPinWrite(DATA_PORT_BASE, ALLDATAPINS, byteToSend >>4);
	pulseLCD();
	GPIOPinWrite(DATA_PORT_BASE, ALLDATAPINS, byteToSend);
	pulseLCD();
}



void setCursorPositionLCD(char row, char col)
{
	char address;

	if (row == 0)
		address = 0;
	else if (row==1)
		address = 0x40;
	else if (row==2)
		address = 0x14;
	else if (row==3)
		address = 0x54;
	else
		address = 0;

	address |= col;

	sendByte(0x80 | address, FALSE);
}



void clearLCD(void)
{
	sendByte(0x01, FALSE);
	sendByte(0x02, FALSE);
	SysCtlDelay(30000);
}



void homeLCD(void) {
	sendByte(0x02, FALSE);
	SysCtlDelay(30000);
}




void setBlockCursorLCD(void) {
	sendByte(0x0F, FALSE);
}



void setLineCursorLCD(void) {
	sendByte(0x0E, FALSE);
}



void cursorOnLCD(void) {
	sendByte(0x0E, FALSE);
}



void cursorOffLCD(void) {
	sendByte(0x0C, FALSE);
}



void displayOffLCD(void) {
	sendByte(0x08, FALSE);
}



void displayOnLCD(void) {
	sendByte(0x0C, FALSE);
}



void initLCD(void)
{

	SysCtlPeripheralEnable(DATA_PERIPH);
	SysCtlPeripheralEnable(CMD_PERIPH);
	GPIOPinTypeGPIOOutput(DATA_PORT_BASE,  ALLDATAPINS);
	GPIOPinTypeGPIOOutput(CMD_PORT_BASE, ALLCONTROLPINS);
	GPIOPinWrite(DATA_PORT_BASE, ALLDATAPINS ,0);
	GPIOPinWrite(CMD_PORT_BASE, ALLCONTROLPINS ,0);



	SysCtlDelay(10000);



	setCmd();
	SysCtlDelay(15000);
	GPIOPinWrite(DATA_PORT_BASE, ALLDATAPINS, 0b0010);
	pulseLCD();
	GPIOPinWrite(DATA_PORT_BASE, ALLDATAPINS, 0b0010);
	pulseLCD();
	sendByte(0x28,FALSE);
	cursorOffLCD();
	sendByte(0x06, FALSE);
	clearLCD();
}



void printLCD(char *text)
{
	char *c;
	c = text;

	while ((c != 0) && (*c != 0))
	{
		sendByte(*c, TRUE);
		c++;
	}
}
