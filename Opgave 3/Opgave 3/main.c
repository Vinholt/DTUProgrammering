
#define F_CPU 16000000UL

#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>
#include "I2C.h"
#include "ssd1306.h"
#include "SPI.h"


void init() {
	I2C_Init();
	InitializeDisplay();
	clear_display();

	SPI_init_master();
}

int getRawData() {
	char MSB;
	char LSB;
	int all_data;
	int temp_data;
	
	PORTB &=~ (1<<PB0); // set SS LOW

	SPI_MasterTransmit(0x00);

	MSB = SPDR;
	
	SPI_MasterTransmit(0x00);

	LSB = SPDR;

	PORTB |= (1<<PB0); // set SS HIGH

	temp_data = (MSB << 8 | LSB)>>3 ; // combine the two bytes in to one variable

	/* Extraxt the 12bits of temperature information */
	//temp_data = 0b111111111111 & (all_data >> 3); // 111111111111 & all_data (MINUS de tre første bits)

	return temp_data;
}


void print_bin(int c, int bits, int row) {
	char c_buffer;
	int XY = 15 - ((16 - bits) / 2); // prints in the center of the screen

	for (int i=0; i<bits; i++) {
		c_buffer = '0' + !!(c & (1 << i));
		sendCharXY(c_buffer, row, XY-i);
	}
}


void print_dec(int dec, int x, int y) {
	char str[20];
	sprintf(str, "%d", dec);
	sendStrXY(str, x, y);
}


void show_temp(int data) {
	print_bin(data, 12, 4);
	
	data = data * 0.25;
	print_dec(data, 5,7);
}


int main(void) {
	init();
	sendStrXY("MAX6675 test",0,2);
	sendStrXY("Temperatur:",2,2);
	
	int temp;
	int i = 0;
	int reel_temp = 0;
	int deci = 0;
	char str[20];
	
	while (1) {
		temp = getRawData();
		reel_temp = trunc(temp*0.25);
		deci = (temp*0.25 - reel_temp) * 100;
		
		sprintf(str, "%d.%0.2d", reel_temp, deci);
		sendStrXY(str, 3,3);
		
		
		//show_temp(temp);
// 		sprintf(str, "%d", i);
// 		sendStrXY(str, 1, 1);
// 		i++;
		
		
		_delay_ms(1000);
	}
}