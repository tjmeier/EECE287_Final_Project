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

//// Macros
#define X_ORIGIN 30
#define Y_ORIGIN 30

//Output Port B
#define LEFT_MOTOR_PWM_LOCATION 6
#define RIGHT_MOTOR_PWM_LOCATION 5
#define LEFT_MOTOR_DIRECTION_LOCATION 2
#define RIGHT_MOTOR_DIRECTION_LOCATION 1
#define YELLOW_LED_LOCATION 7

//define PWM parameters
#define PWM_TOP 100 //pwm ranges from 0 to 100



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

//tyler
void configure_motors(){

	//Sets DDRB to be outputs at the 4 motor locations	
	DDRB &= ~(1<< LEFT_MOTOR_PWM_LOCATION); 		
	DDRB &= ~(1<< RIGHT_MOTOR_PWM_LOCATION); 
	DDRB &= ~(1<< LEFT_MOTOR_DIRECTION_LOCATION); 		
	DDRB &= ~(1<< RIGHT_MOTOR_DIRECTION_LOCATION);

}

////Toggling motor pwms////
void turn_on_left_motor_pwm(){
	PORTB |= (1<< LEFT_MOTOR_PWM_LOCATION); 
}

void turn_off_left_motor_pwm(){
	PORTB &= ~(1<< LEFT_MOTOR_PWM_LOCATION); 
}

void turn_on_right_motor_pwm(){
	PORTB |= (1<< RIGHT_MOTOR_PWM_LOCATION); 
}

void turn_off_right_motor_pwm(){
	PORTB &= ~(1<< RIGHT_MOTOR_PWM_LOCATION); 
}

////Toggling motor directions////
void set_reverse_left_motor_direction(){
	//setting the port = reverse
	PORTB |= (1<< LEFT_MOTOR_DIRECTION_LOCATION); 
}

void set_forward_left_motor_direction(){
	//clearing the port = forward
	PORTB &= ~(1<< LEFT_MOTOR_DIRECTION_LOCATION); 
}

void set_reverse_right_motor_direction(){
	//setting the port = reverse
	PORTB |= (1<< RIGHT_MOTOR_DIRECTION_LOCATION); 
}

void set_forward_right_motor_direction(){
	//clearing the port = forward
	PORTB &= ~(1<< RIGHT_MOTOR_DIRECTION_LOCATION); 
}

void configure_yellow_led()
{
	DDRC |= (1<<YELLOW_LED_LOCATION);
}

void turn_on_yellow_led()
{
	PORTC |= (1<<YELLOW_LED_LOCATION);
}

void turn_off_yellow_led()
{
	PORTC &= ~(1<<YELLOW_LED_LOCATION);
}


int main()
{
    // Setting up the zumo bot
    configure_zumo();
	oled_clear();
	configure_motors();


    // Variable initializations
    int x = X_ORIGIN;
    int y = Y_ORIGIN;

	// Set up x-coord value on OLED
	oled_set_cursor(0,0); // First row
    displayCoord(x, 'x');

	// Set up y-coord value on OLED
	oled_set_cursor(1,0); // Second row
    displayCoord(y, 'y');

    // Loop to change x coordinate
    while(button_b_is_up())
	{
		displayCoord(x, 'x');
		changeCoord(&x);
		_delay_us(100000);
	}

    // Delay for user input
	if(button_b_is_down())
		_delay_us(1000000);

    // Loop to chnage y coordinate
	while(button_b_is_up())
	{
		displayCoord(y, 'y');
		changeCoord(&y);
		_delay_us(100000);
	}
	
    
    _delay_us(5000000);
    
	//initialize variables
	const unsigned int comparison_tolerance = 0;
	//less than 1 makes robot turn more left, greater than 1 makes robot turn more right
	const double right_turn_bias = 1.02;//seems to be between 1.02 and 1.03
	const double tweak_magnitude = 3;

 	unsigned int pwm_counter=0;    	
	double left_duty_cycle=25;   
	double right_duty_cycle=25;  
	uint32_t right_revs, left_revs;


 	while(1) { 

		//pwm increment
		pwm_counter++;
		if (pwm_counter >= PWM_TOP){
			pwm_counter = 0;
		}
		
		if (pwm_counter < left_duty_cycle) { 
			turn_on_left_motor_pwm();
 	 	}
 	 	else { 
			turn_off_left_motor_pwm();
	 	} 

		if (pwm_counter < right_duty_cycle) { 
			turn_on_right_motor_pwm();
 	 	}
 	 	else { 
			turn_off_right_motor_pwm();
	 	} 


		right_revs = get_right_quadrature_counter();
		left_revs = get_left_quadrature_counter();
			//correction for drifting left
			if (right_revs*right_turn_bias + comparison_tolerance < left_revs){
				if (right_duty_cycle < PWM_TOP)
					right_duty_cycle += tweak_magnitude;
				if (left_duty_cycle > 0)
					left_duty_cycle -= tweak_magnitude;
			}

			//correction for drifting right
			if (left_revs + comparison_tolerance < right_revs*right_turn_bias){
				if (left_duty_cycle < PWM_TOP)
					left_duty_cycle += tweak_magnitude;
				if (right_duty_cycle > 0)
					right_duty_cycle -= tweak_magnitude;
			}

 	 	_delay_us(10); 
}


