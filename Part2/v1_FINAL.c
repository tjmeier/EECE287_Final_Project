//EECE 287, Spring 2023 Project 2
//Instructor: Meg Jain
// Group # - 
//Team Members - 

#include <stdbool.h>
#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>
#include <stdio.h> 
#include "zumo_drivers.h"


#define RIGHT_SENSOR_PIN 4 //PD4
#define LEFT_SENSOR_PIN 5 //PF5
#define FRONT_SENSOR_PIN 1 //PF1

//Output Port B
#define LEFT_MOTOR_PWM_LOCATION 6
#define RIGHT_MOTOR_PWM_LOCATION 5
#define LEFT_MOTOR_DIRECTION_LOCATION 2
#define RIGHT_MOTOR_DIRECTION_LOCATION 1

//Output Port C
#define YELLOW_LED_LOCATION 7

//Remote command macros
#define REMOTE_0 0xA61
#define REMOTE_1 0xA11
#define REMOTE_2 0xB10
#define REMOTE_3 0xA91
#define REMOTE_4 0xA51
#define REMOTE_5 0xB50
#define REMOTE_6 0xAD1
#define REMOTE_7 0xA31
#define REMOTE_8 0xB30
#define REMOTE_9 0xAB1

#define REMOTE_UP_ARROW 0xB40
#define REMOTE_DOWN_ARROW 0xB60
#define REMOTE_LEFT_ARROW 0xA21
#define REMOTE_RIGHT_ARROW 0xAA1
#define REMOTE_ENTER 0xB20
#define REMOTE_SETUP 0xA41
#define REMOTE_STOP 0xAC1
#define REMOTE_VOL_DOWN 0xA01
#define REMOTE_VOL_UP 0xA81
#define REMOTE_PAUSE 0xB00
#define REMOTE_BACK 0xAE1

#define REMOTE_VERIFICATION_CONSTANT 0xFE01F

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

#define COLLISION 0xA

//Taken from part 1
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

void drive_forward_inches (float/*uint8_t*/ distance_inches, int8_t speed_percentage){
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

/*
The following operates the proximity sensors by transmitting pulses on both 
the left and right LEDs at six different brightness levels. 
For each sensor, it generates two numbers: the number of brightness levels for the left LEDs that activated the sensor, 
and the number of brightness levels for the right LEDs that activated the sensor. 
A higher reading corresponds to more IR light getting reflected to the sensor, 
which is influenced by the size, reflectivity, proximity, and location of nearby objects. 
However, the presence of other sources of 38 kHz IR pulses (e.g. from another robot) can also affect the readings.
*/


uint16_t BrightnessLevels[] = {4, 15, 32, 55, 85, 120}; //think - duty cyles

// Right sensor
void configure_right_proximity_sensor()
{
	DDRD &= ~(1<<RIGHT_SENSOR_PIN);
	PORTD |= (1<<RIGHT_SENSOR_PIN);
}

// Left sensor
void configure_left_proximity_sensor()
{
	DDRF &= ~(1<<LEFT_SENSOR_PIN);
	PORTF |= (1<<LEFT_SENSOR_PIN);
	//configure left proximity sensor
}

// Front sensor
void configure_front_proximity_sensor()
{
	//configure right proximity sensor
	DDRF &= ~(1<<FRONT_SENSOR_PIN);
	PORTF |= (1<<FRONT_SENSOR_PIN);
}

// All proximity sensors
void configure_proximity_sensors()
{
	void configure_right_proximity_sensor();
	void configure_left_proximity_sensor();
	void configure_front_proximity_sensor();
}

uint8_t read_front_proximity_sensor()
{
	//turn off line sensor IR LEDS to remove interference
	DDRB &= ~(1<<7);
	PORTB &= ~(1<<7);
	uint16_t brightness_duty_cycle = 0;
	uint8_t front_sensor_detection_factor = 0;
	
	for (int i = 0; i < 6; i++){ //6 is the number of brightness levels in the array

		brightness_duty_cycle = BrightnessLevels[i];
		
		start_IR_emitters(0, brightness_duty_cycle); //0 indicates to use right IR emitters
		_delay_us(420);
		
		if(!(PINF & (1<<FRONT_SENSOR_PIN)))
			front_sensor_detection_factor++;
		
		stop_IR_emitters();
		_delay_us(550);
		
		start_IR_emitters(1, brightness_duty_cycle); //1 indicates to use left IR emitters
		_delay_us(420);
		
		if(!(PINF & (1<<FRONT_SENSOR_PIN)))
			front_sensor_detection_factor++;
		stop_IR_emitters();
		_delay_us(550);
		
	}
	return front_sensor_detection_factor;
}

uint16_t check_for_remote_button() {
	uint32_t remote_data = 0;
	
	if (get_ir_pin() != 1)
		remote_data = wait_for_remote_command();
	
	if (remote_data>>12 == REMOTE_VERIFICATION_CONSTANT); //looks at most siginifcant 20 bits (5 hex values)
		return remote_data - (REMOTE_VERIFICATION_CONSTANT << 12); //subtracts FE01F000
	
	return 0;
}

uint16_t wait_until_remote_button() {
	uint32_t remote_data;
	
	do{
		remote_data = wait_for_remote_command();

	}while (remote_data>>12 != REMOTE_VERIFICATION_CONSTANT); //looks at most siginifcant 20 bits (5 hex values)

	return remote_data - (REMOTE_VERIFICATION_CONSTANT << 12); //subtracts FE01F000
}

int decode_remote_numpad (uint32_t remote_data)
{
    switch(remote_data)
    {
        case REMOTE_0:
            return 0;
            break;
        case REMOTE_1:
            return 1;
            break;
        case REMOTE_2:
            return 2;
            break;
        case REMOTE_3:
            return 3;
            break;
        case REMOTE_4:
            return 4;
            break;
        case REMOTE_5:
            return 5;
            break;
        case REMOTE_6:
            return 6;
            break;
        case REMOTE_7:
            return 7;
            break;
        case REMOTE_8:
            return 8;
            break;
        case REMOTE_9:
            return 9;
            break;
        default:
            return -1; //debug value?
            break;
    }
}

void drive_until_collison(){

	uint8_t front_proximity_value;

	do  { 
	
		drive_forward_inches(0.25, DRIVE_SPEED);
		//read proximity sensor
		oled_set_cursor(3,5);
		front_proximity_value = read_front_proximity_sensor();
		oled_put_hex(front_proximity_value >> 4);
		oled_put_hex(front_proximity_value);

		//the condition is if the front sensor is less than collision (keeps driving if no collison) 
 	}
	while (front_proximity_value < COLLISION);
	
		//once an obstacle is detected, turn off the motors
	turn_off_right_motor_pwm();
	turn_off_left_motor_pwm();
}

int main(){
	
	configure_zumo();
	configure_proximity_sensors();
	configure_yellow_led();
	configure_motors();

	oled_clear();


    uint32_t remote_data;
    int display_value;
	uint8_t current_obstacle_count = 0;

	////////////////////////////
	//Step 1: Get quantity of obstacles

	do{
		remote_data = wait_until_remote_button();
	    display_value = decode_remote_numpad(remote_data);
		//if a valid numpad button was pressed, it gets displayed on the oled
		oled_set_cursor(1,5);
		if (display_value != -1)
        	oled_put_hex(display_value);
		else	
			oled_put_space();
	}
	//the quantity must be between 5 and 8; keeps waiting if it's not
	while (display_value < 5 || display_value > 8);

	//the quantity of obstacles to find is the integer pressed on the remote
	const uint8_t total_quantity_of_obstacles = display_value;


	/////////////////////////////////////////////////////////////////////////////////
	//Step 2: Main loop to hit all obstacles (except last one, which results in celebration)

	oled_set_cursor(2,5);
	oled_put_hex(0);

	while (current_obstacle_count < total_quantity_of_obstacles - 1){


		////////////////////////////////////////////
		//drive forward until it hits an obstable///
		////////////////////////////////////////////

		drive_until_collison();		

		/////////////////////////////////////
		//Use remote to turn left or right //
		/////////////////////////////////////
		
		current_obstacle_count++;
		oled_set_cursor(2,5);
		oled_put_hex(current_obstacle_count);

		get_turn_direction:

		remote_data = wait_until_remote_button();
		if (remote_data == REMOTE_LEFT_ARROW)
			turn_left_degrees(90, TURN_SPEED);
		else if (remote_data == REMOTE_RIGHT_ARROW)
			turn_right_degrees(90, TURN_SPEED);
		else	
			goto get_turn_direction;


		
	}


	/////////////////////////////
	//Step 3: Celebration //////
	////////////////////////////

	//keep driving to reach the last and final object;

	drive_until_collison();
	current_obstacle_count++;

	oled_set_cursor(2,5);
	oled_put_hex(current_obstacle_count);

	turn_left_degrees(720, TURN_SPEED);


	return 0;
}
