//When you have code to test implementing, past it here:

// Binghamton University
// EECE 287 Sophomore Design
// Test PWM to control the speed of the motors
#define F_CPU 1000000UL

#include <avr/io.h> 
#include <util/delay.h> 
#include <stdint.h>
#include <stdbool.h>
#include "zumo_drivers.h"

//Output Port B
#define LEFT_MOTOR_PWM_LOCATION 6
#define RIGHT_MOTOR_PWM_LOCATION 5
#define LEFT_MOTOR_DIRECTION_LOCATION 2
#define RIGHT_MOTOR_DIRECTION_LOCATION 1

//Input Pin C
#define A_BUTTON_LOCATION 3
#define C_BUTTON_LOCATION 0

//Output Port C
#define YELLOW_LED_LOCATION 7

//define PWM parameters
#define INCREMENT 10
#define PWM_TOP 100 //pwm ranges from 0 to 100



void configure_motors(){

	//Sets DDRB to be outputs at the 4 motor locations	
	DDRB |= (1<< LEFT_MOTOR_PWM_LOCATION); 		
	DDRB |= (1<< RIGHT_MOTOR_PWM_LOCATION); 
	DDRB |= (1<< LEFT_MOTOR_DIRECTION_LOCATION); 		
	DDRB |= (1<< RIGHT_MOTOR_DIRECTION_LOCATION);

	//Clears all PORTB at motor locations
	PORTB &= ~(1<< LEFT_MOTOR_PWM_LOCATION); 		
	PORTB &= ~(1<< RIGHT_MOTOR_PWM_LOCATION); 
	PORTB &= ~(1<< LEFT_MOTOR_DIRECTION_LOCATION); 		
	PORTB &= ~(1<< RIGHT_MOTOR_DIRECTION_LOCATION); 		
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


void configure_A_button(){
	
	DDRB &= ~(1<< A_BUTTON_LOCATION); 
	PORTB |= (1<< A_BUTTON_LOCATION); 
}

void configure_C_button(){
	
	DDRB &= ~(1<< C_BUTTON_LOCATION); 
	PORTB |= (1<< C_BUTTON_LOCATION);		//it should start at 1?
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


int main() { 

    //from zumo_drivers.h
    configure_zumo();

	//for ddr and resistor stuff
	configure_motors();
	configure_A_button();
	configure_C_button();
	configure_yellow_led();

	//initialize variables
 	unsigned int pwm_counter = 0;    	
	unsigned int duty_cycle = 0;
	unsigned int left_motor_speed = 0, right_motor_speed = 0;

	//Main Loop

 	while(1) { 

 	 	//PWM Counter 
 	 	pwm_counter = pwm_counter + 1; 
 	 	if( pwm_counter >= PWM_TOP ){ 
 	 	 	pwm_counter = 0; //once the counter reaches the top, resets to zero
 	 	} 
		
 	 	//PWM for Left Motor
		if (pwm_counter < left_motor_speed) { 
 	 	 	turn_on_left_motor_pwm();
 	 	}
 	 	else { 
 	 	 	turn_off_left_motor_pwm();
	 	} 

		  
 		//PWM for Right Motor
		if (pwm_counter < right_motor_speed) { 
 	 	 	turn_on_right_motor_pwm();
 	 	}
 	 	else { 
 	 	 	turn_off_right_motor_pwm();
	 	} 
  

 	 	//Decrement duty cycle when button A is pressed  	 	
		if( button_A_pressed == 1 ){  	 	 	
			if( duty_cycle >= INCREMENT ){ 
 	 	 	 	duty_cycle = duty_cycle - INCREMENT; 
 	 	 	} 
 	 	 	else{ 
 	 	 	 	duty_cycle = 0; 
 	 	 	} 
 	 	} 
 	 	
 	 	//Increment duty cycle when C button pressed 
		
		///-------Enter code here-------
  		if( button_C_pressed == 1 ){  	 	 	
			if( duty_cycle <= (PWM_TOP - INCREMENT) ){ 
 	 	 	 	duty_cycle = duty_cycle + INCREMENT; 
 	 	 	} 
 	 	 	else{ 
 	 	 	 	duty_cycle = PWM_TOP; 
 	 	 	} 
 	 	} 
 	 	//small delay to slow down main loop 
 	 	_delay_us(10); 
 	} 
}
3