// Binghamton University, EECE 287 Sophomore Design
// Final Project #1
// Instructor: Meghana Jain

//Group: #1
//Student Names: Kirill Kobyakov and Tyler Meier
#include <avr/io.h> 
#include <util/delay.h> 
#include <stdint.h>
#include <stdbool.h>
#include "zumo_drivers.h"

////////////////////////
//       Macros       //
////////////////////////

//Output Port B
#define LEFT_MOTOR_PWM_LOCATION 6
#define RIGHT_MOTOR_PWM_LOCATION 5
#define LEFT_MOTOR_DIRECTION_LOCATION 2
#define RIGHT_MOTOR_DIRECTION_LOCATION 1

//Output Port C
#define YELLOW_LED_LOCATION 7

//Software Constants
#define PWM_TOP 100
#define LEFTQUAD_TO_INCHES 201.5f
#define QUAD_TO_DEGREES_ROTATED 7.7695f
#define F_RIGHT_TURN_BIAS 0.98975f
#define R_RIGHT_TURN_BIAS 1.01025f
#define X_ORIGIN 30
#define Y_ORIGIN 30
#define DRIVE_SPEED 30
#define TURN_SPEED 30

/////////////////////////////////
//Hardware Function Definitions//
/////////////////////////////////

void configure_motors(){

	//Sets DDRB to be outputs at the 4 motor locations
	//when using zumodrivers.h, the ddr needs to be cleared to be set
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
void set_left_motor_direction_reverse(){
	//setting the port = reverse
	PORTB |= (1<< LEFT_MOTOR_DIRECTION_LOCATION); 
}

void set_left_motor_direction_forward(){
	//clearing the port = forward
	PORTB &= ~(1<< LEFT_MOTOR_DIRECTION_LOCATION); 
}

void set_right_motor_direction_reverse(){
	//setting the port = reverse
	PORTB |= (1<< RIGHT_MOTOR_DIRECTION_LOCATION); 
}

void set_right_motor_direction_forward(){
	//clearing the port = forward
	PORTB &= ~(1<< RIGHT_MOTOR_DIRECTION_LOCATION); 
}

void configure_yellow_led(){
	DDRC |= (1<<YELLOW_LED_LOCATION);
}

void turn_on_yellow_led(){
	PORTC |= (1<<YELLOW_LED_LOCATION);
}

void turn_off_yellow_led(){
	PORTC &= ~(1<<YELLOW_LED_LOCATION);
}

//////////////////////////////////
// Software Function Defintions //
//////////////////////////////////

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

void drive_revolutions(uint32_t revolutions, int8_t speed_percentage, float right_turn_bias){
	uint32_t initial_left_revs = get_left_quadrature_counter();
	uint32_t initial_right_revs = get_right_quadrature_counter();

	const uint8_t comparison_tolerance = 0;
	
	const uint8_t tweak_magnitude = 3;

 	unsigned int pwm_counter=0;    	
	int8_t left_duty_cycle=speed_percentage;   
	int8_t right_duty_cycle=speed_percentage;  
	uint32_t right_revs, left_revs;

 	do  { 
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


		right_revs = get_right_quadrature_counter() - initial_right_revs;
		left_revs = get_left_quadrature_counter() - initial_left_revs;
			//correction for drifting left
			if (right_revs*right_turn_bias + comparison_tolerance < left_revs){
				if (right_duty_cycle + tweak_magnitude <= PWM_TOP && left_duty_cycle - tweak_magnitude >= 0){
					right_duty_cycle += tweak_magnitude;
					left_duty_cycle -= tweak_magnitude;
				}
			}

			//correction for drifting right
			if (left_revs + comparison_tolerance < right_revs*right_turn_bias){
				if (left_duty_cycle + tweak_magnitude <= PWM_TOP && right_duty_cycle - tweak_magnitude >= 0){
					left_duty_cycle += tweak_magnitude;
					right_duty_cycle -= tweak_magnitude;
				}
			}

 	 	_delay_us(10); 
 	} while (left_revs < revolutions);
	
	
	turn_off_right_motor_pwm();
	turn_off_left_motor_pwm();
}

void drive_forward_inches (uint8_t distance_inches, int8_t speed_percentage){
	set_right_motor_direction_forward();
	set_left_motor_direction_forward();
	drive_revolutions(distance_inches * LEFTQUAD_TO_INCHES, speed_percentage, F_RIGHT_TURN_BIAS);
}

void drive_reverse_inches (uint8_t distance_inches, int8_t speed_percentage){
	set_right_motor_direction_reverse();
	set_left_motor_direction_reverse();
	drive_revolutions(distance_inches * LEFTQUAD_TO_INCHES, speed_percentage, R_RIGHT_TURN_BIAS);
}

void turn_left_degrees(uint16_t degrees_to_rotate, int8_t speed_percentage){
	set_right_motor_direction_forward();
	set_left_motor_direction_reverse();
	drive_revolutions(degrees_to_rotate * QUAD_TO_DEGREES_ROTATED, speed_percentage, F_RIGHT_TURN_BIAS);
}

void turn_right_degrees(uint16_t degrees_to_rotate, int8_t speed_percentage){
	set_right_motor_direction_reverse();
	set_left_motor_direction_forward();
	drive_revolutions(degrees_to_rotate * QUAD_TO_DEGREES_ROTATED, speed_percentage, F_RIGHT_TURN_BIAS);
}

//Place robot directly on the origin, facing positive y direction
void drive_to_coord(int x, int y)
{
	uint8_t x_distance = 0;
	uint8_t y_distance = 0;

	//go to y coordinate my either driving forward or in reverse
	if(y >= Y_ORIGIN)
	{
		y_distance = y - Y_ORIGIN;
		drive_forward_inches(y_distance, DRIVE_SPEED);
	}
	else
	{
		y_distance = Y_ORIGIN - y;
		drive_reverse_inches(y_distance, DRIVE_SPEED);
	}

	//go to x coordinate by either turning left or right and then driving forward
	if(x >= X_ORIGIN)
	{
		x_distance = x - X_ORIGIN;
		turn_right_degrees(90, TURN_SPEED);
	}
	else
	{
		x_distance = X_ORIGIN - x;
		turn_left_degrees(90, TURN_SPEED);
	}
	
	drive_forward_inches(x_distance, DRIVE_SPEED);
}

////////////////////////
//    Main Function   //
////////////////////////

int main() { 

	start:
	configure_zumo();//deleting this line makes it work
	oled_clear();
	configure_yellow_led();

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

	// Delay when resetting 
	_delay_us(1000000);

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
	displayCoord(x,'x');
	displayCoord(y,'y');

	// Countdown
	for(uint8_t i = 5; i > 0; i--)
	{
		oled_set_cursor(3,10-2*i);
		oled_put_hex(i);
		for(uint8_t j = 0; j < 100; j++)
			_delay_us(10000);		
	}

	turn_on_yellow_led();
	drive_to_coord(x,y);
	turn_off_yellow_led();

	// Button b can be used as a reset for main
	end:
		if(button_b_is_down())
			goto start;
	goto end;
}
