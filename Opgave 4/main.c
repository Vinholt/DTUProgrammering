/*
 *
 *	Oscar Emil Vinholt
 *
 *	Opgave 4
 *
 *	Datateknik og programmering
 *
 */

#define F_CPU 16000000UL
#define BAUD 19200								//Baud rate
#define MYUBRRF F_CPU/8/BAUD-1					//full duplex
#include <avr/io.h>
#include <math.h>
#include <string.h>
#include "I2C.h"
#include <util/delay.h>
#include <avr/pgmspace.h>
#include "ssd1306.h"
#include <stdio.h>


void init()
{
	I2C_Init();
	InitializeDisplay();
	clear_display();
}

void uart1_Init(unsigned int ubrr)
{
	UCSR1A =(1<<U2X1);							//full duplex
	
	UCSR1B |=(1<<RXEN1)|(1<<TXEN1);				//enable receive and transmit
	
	UCSR1C |=(1<<UCSZ10)|(1<<UCSZ11);			// 8 bit, no parity, 1 start bit, 1 stop bit
	
	UBRR1H = (unsigned char)(ubrr>>8);			//Baud rate values up to 16 bit
	UBRR1L = (unsigned char)ubrr;
}

char getchUSART1(void)							//Receives 1 byte and returns it
{
	while(!(UCSR1A & (1<<RXC1)));				//Waits until the character is received
	return UDR1;
}

void putchUSART1(char tx)						//Transmit 1 byte
{
	while(!(UCSR1A & (1<<UDRE1)));				//Wait for empty transmit buffer
	UDR1 = tx;
}

void OLED_display()
{
	sendCharXY(UDR1,2,2);
}

int main(void)
{
	init();
	uart1_Init(MYUBRRF);
	while (1)
	{
		putchUSART1(getchUSART1()); //retransmit what is received
		_delay_ms(1000);
		OLED_display(UDR1);
	}
}