//EECE 287, Spring 2023 Project 2
//Instructor: Meg Jain
// Group #1
//Team Members - Kirill Kobyakov, Tyler Meier

#include <stdbool.h>
#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>
#include <stdio.h> 
#include "zumo_drivers.h"


#define RIGHT_SENSOR_PIN 4 //PD4
#define LEFT_SENSOR_PIN 5 //PF5
#define FRONT_SENSOR_PIN 1 //PF1
#define BUZZER_PWM_PIN 7 // PD7

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

//Function macros
#define PWM_TOP 100
#define LEFTQUAD_TO_INCHES 201.5f
#define QUAD_TO_DEGREES_ROTATED 7.6754f
#define F_RIGHT_TURN_BIAS 0.98660f //0.98970f
#define R_RIGHT_TURN_BIAS 1.01025f
#define X_ORIGIN 30
#define Y_ORIGIN 30
#define DRIVE_SPEED 25
#define TURN_SPEED 25

#define COLLISION 0xC

//Musical notes globals
double a_note[] = {27.5, 55, 110, 220, 440, 880, 1760, 3520, 7040, 14080};
double a_sharp_note[] = {29.14, 58.27, 116.54, 233.08, 466.16, 932.33, 1864.66, 3729.31, 7458.62, 14917.24};
double b_note[] = {31, 62, 123.47, 246.94, 493.88, 987.77, 1975.53, 3951.07, 7902.13, 15804.26};
double c_note[] = {16.35, 32.7, 65.41, 130.81, 261.63, 523.25, 1046.5, 2093, 4186.01, 8372.02};
double c_sharp_note[] = {17.32, 34.65, 69.3, 138.59, 277.18, 554.37, 1108.73, 2217.46, 4434.92, 8869.84};
double d_note[] = {18.35, 36.71, 73.42, 146.83, 293.66, 587.33, 1174.66, 2349.32, 4698.64, 9397.28};
double d_sharp_note[] = {19.45, 38.89, 77.78, 155.56, 311.13, 622.25, 1244.51, 2489.02, 4978.03, 9956.06};
double e_note[] = {20.6, 41.2, 82.41, 164.81, 329.63, 659.26, 1318.51, 2637.02, 5274.04, 10548.08};
double f_note[] = {21.83, 43.65, 87.31, 174.61, 349.23, 698.46, 1396.91, 2793.83, 5587.65, 11175.3};
double f_sharp_note[] = {23.12, 46.25, 92.5, 185, 369.99, 739.99, 1479.98, 2959.96, 5919.91, 11839.82};
double g_note[] = {24.5, 49, 98, 196, 392, 783.99, 1567.98, 3135.96, 6271.93, 12543.85};
double g_sharp_note[] = {25.96, 51.91, 103.83, 207.65, 415.3, 830.61, 1661.22, 3322.44, 6644.88, 13289.75};

uint16_t BrightnessLevels[] = {4, 15, 32, 55, 85, 120}; //the 6 brightnesses for proximity sensor to detect

////////////////////////////////////////////////////////
//Functions taken from our Part 1:
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

///////////////////////////////////////////////////
//Functions for Proximity Sensor operation:
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

///////////////////////////////////////////////////
//Functions for remote controller operation:

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

///////////////////////////////////////
//Functions for buzzer operation:

void configure_buzzer()
{
	DDRD &= ~(1<<BUZZER_PWM_PIN);
}

void turn_on_buzzer(){
	PORTD |= (1<< BUZZER_PWM_PIN); 
}

void turn_off_buzzer(){
	PORTD &= ~(1<< BUZZER_PWM_PIN); 
}

void delay_function(int microseconds)
{
	for(int count = 0; count < (int) microseconds; count++)
	{
		_delay_us(1);
	}
}

double convert_to_half_period_us(double frequency){
	return (1/frequency) * 1000000/2;
}
void play_note(double frequency, double duration)
{   
	double period = convert_to_half_period_us(frequency);

	int numCycles = (int) (duration * 250000 / (double) period);
    for(unsigned int cycles = 0; cycles < numCycles; cycles++)
    {
        turn_on_buzzer();
		delay_function(period);
        turn_off_buzzer();
		delay_function(period);
    }
}

void play_final_countdown(){

	configure_buzzer();

    play_note((c_sharp_note[5]), 0.175);
	_delay_ms(10);
    play_note((b_note[5]), 0.175);
	_delay_ms(10);
    play_note((c_sharp_note[6]), 0.40);
	_delay_ms(10);
    play_note((f_sharp_note[5]), 1.25);

    _delay_ms(300);

    play_note(d_note[6], 0.125);
	_delay_ms(10);
    play_note(c_sharp_note[6], 0.125);
	_delay_ms(10);
    play_note(d_note[6], 0.125);
	_delay_ms(10);
    play_note(c_sharp_note[6], 0.25);
	_delay_ms(10);
    play_note(b_note[5], 1.25);

    _delay_ms(300);

    play_note(d_note[6], 0.125);
	_delay_ms(10);
    play_note(c_sharp_note[5], 0.125);
	_delay_ms(10);
    play_note(d_note[6], 0.40);
	_delay_ms(10);
    play_note(f_sharp_note[5], 1.25);

    _delay_ms(300);

    play_note(b_note[5], 0.175);
	_delay_ms(10);
    play_note(a_note[5], 0.25);
	_delay_ms(10);
    play_note(b_note[5], 0.25);
	_delay_ms(10);
    play_note(a_note[5], 0.25);
	_delay_ms(10);

    play_note(g_sharp_note[5], 0.25);
	_delay_ms(10);
    play_note(b_note[5], 0.25);
	_delay_ms(10);
    play_note(a_note[5], 1.25);

    _delay_ms(100);

    play_note(g_sharp_note[4], 0.25);
    _delay_ms(10);
    play_note(a_note[5], 0.25);
    _delay_ms(10);
    play_note(b_note[5], 1);

    _delay_ms(100);

    play_note(a_note[5], 0.25);
    _delay_ms(10);
    play_note(b_note[5], 0.25);
    _delay_ms(10);
    play_note(c_sharp_note[6], 0.25);
    _delay_ms(10);
    play_note(b_note[5], 0.25);
    _delay_ms(10);
    play_note(a_note[5], 0.25);
    _delay_ms(10);
    play_note(g_sharp_note[5], 0.25);
    _delay_ms(10);
    play_note(f_sharp_note[5], 0.5);
    _delay_ms(10);
    play_note(d_note[6], 0.5);
    _delay_ms(10);
    play_note(c_sharp_note[6], 1);

    _delay_ms(200);

    play_note(c_sharp_note[6], .25);
    _delay_ms(10);
    play_note(d_note[5], .25);
    _delay_ms(10);
    play_note(c_sharp_note[5], .25);
    _delay_ms(10);
    play_note(b_note[5], .25);
    _delay_ms(10);
    play_note(c_sharp_note[6], 1);

}

//////////////////////////////////////////////
//Drive function using front proximity sensor

void drive_until_collison(){

	uint8_t front_proximity_value;

	do  { 
	
		drive_forward_inches(0.5, DRIVE_SPEED);
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


/////////////////////////////////////////////
//Main function

int main(){
	
	configure_zumo();
	configure_proximity_sensors();
	configure_yellow_led();
	configure_motors();

	oled_clear();


    uint32_t remote_data;
    int display_value;
	uint8_t current_obstacle_count = 0;

	/////////////////////////////////////
	//Step 1: Get quantity of obstacles//
	/////////////////////////////////////

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


	//////////////////////////////////////////////////////////////////////////////////////////
	//Step 2: Main loop to hit all obstacles (except last one, which results in celebration)//
	//////////////////////////////////////////////////////////////////////////////////////////

	oled_set_cursor(2,5);
	oled_put_hex(0);

	while (current_obstacle_count < total_quantity_of_obstacles - 1){


		////////////////////////////////////////////
		//drive forward until it hits an obstable///
		////////////////////////////////////////////

		drive_until_collison();		
		
		current_obstacle_count++;
		oled_set_cursor(2,5);
		oled_put_hex(current_obstacle_count);

		drive_reverse_inches(0.2, DRIVE_SPEED); //back away a little from the obstacle

		/////////////////////////////////////
		//Use remote to turn left or right //
		/////////////////////////////////////

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

	drive_reverse_inches(0.2, DRIVE_SPEED);

	oled_set_cursor(2,5);
	oled_put_hex(current_obstacle_count);

	//spin around twice
	turn_left_degrees(720, TURN_SPEED);

	//play music on the buzzer
	play_final_countdown();

	return 0;
}
