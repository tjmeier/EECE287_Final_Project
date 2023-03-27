// Binghamton University, EECE 287 Sophomore Design
// Final Project #
// Instructor: Meghana Jain

//Group: 
//Student Names: 

#include <stdbool.h>
#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>
#include <stdio.h> 
#include "zumo_drivers.h"

int x = 30;
int y = 30;

void changeCoord(int* coord)
{
	if(button_a_is_down() && *coord < 60)
		*coord += 1;
	else if(button_c_is_down() && *coord > 0)
		*coord += -1;
}

void displayCoord(int coord, char dimension)
{
	int digit0, digit1;
	digit0 = coord % 10;
	digit1 = coord / 10;

	if(dimension == 'x')
		oled_set_cursor(0,0);
	else if(dimension == 'y')
		oled_set_cursor(1,0);
	oled_put_hex(digit1);
	oled_put_hex(digit0);
}

int main(){

	configure_zumo();
	oled_clear();

	// Set up x-coord value on OLED
	oled_set_cursor(0,0);
	oled_put_hex(0x3);
	oled_put_hex(0x0);
	_delay_us(100);

	// Set up y-coord value on OLED
	oled_set_cursor(1,0);
	oled_put_hex(0x3);
	oled_put_hex(0x0);
	_delay_us(100);

	while(button_b_is_up())
	{
		displayCoord(x, 'x');
		changeCoord(&x);
		_delay_us(100000);
	}
	if(button_b_is_down())
		_delay_us(100000);
	while(button_b_is_up())
	{
		displayCoord(y, 'y');
		changeCoord(&y);
		_delay_us(100000);
	}

	while(1)
	{
		_delay_us(1000000);
		oled_set_cursor(2,0);
		oled_put_hex(100);
	}
	
	return 0;
}
