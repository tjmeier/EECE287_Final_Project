// Binghamton University, EECE 287 Sophomore Design
// Final Project #1
// Instructor: Meghana Jain

//Group: #1
//Student Names: Kirill Kobyakov and Tyler Meier

#include <stdbool.h>
#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>
#include <stdio.h> 
#include "zumo_drivers.h"

// Function to change coordinate using buttons
// Button A increases
// Button C decreases
void changeCoord(int* coord)
{
    // Increases the coordinate value if button a is pressed and less than max value
	if(button_a_is_down() && *coord < 60)
		*coord += 1;
        
    // Decreases the coordinate value if button c is pressed and greater than min value
	else if(button_c_is_down() && *coord > 0)
		*coord += -1;
}

// Function to display the cordiante value on the display
// If x coordinate, display on first row
// If y coordinate, display on second row
void displayCoord(int coord, char dimension)
{
    // Seperates each digit in the coordinate
	int digit0, digit1;
	digit0 = coord % 10;
	digit1 = coord / 10;

    // Sets the cursor based on what dimension is being edited
	if(dimension == 'x')
		oled_set_cursor(0,0);
	else if(dimension == 'y')
		oled_set_cursor(1,0);
    
    // Places each digit on the OLED
	oled_put_hex(digit1);
	oled_put_hex(digit0);
}