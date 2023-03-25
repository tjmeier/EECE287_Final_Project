// Binghamton University
// EECE 287 Sophomore Design
// Lab 7, Design 2 - Implement PWM on LEDs of the Zumo 32u4 OLED Robot 
// Basic PWM Implementation without using in-built PWM Timers/Counter Pins  

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

void configure_motors(){

	//Sets DDRB to be outputs at the 4 motor locations	
	DDRB |= (1<< LEFT_MOTOR_PWM_LOCATION); 		
	DDRB |= (1<< RIGHT_MOTOR_PWM_LOCATION); 
	DDRB |= (1<< LEFT_MOTOR_DIRECTION_LOCATION); 		
	DDRB |= (1<< RIGHT_MOTOR_DIRECTION_LOCATION);

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

#define A_BUTTON_LOCATION 3
#define YELLOW_LED_LOCATION 7
//replace X with the pin mapping for button C
#define C_BUTTON_LOCATION 0

//define PWM parameters
#define PWM_TOP 100 //maximum value of 100, indicating 100%
#define INCREMENT 5 //how much the duty cycle changes with each button press 

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

	//configure ports
	configure_zumo();
	configure_yellow_led();

	configure_motors();

	set_forward_right_motor_direction();
	set_reverse_left_motor_direction();

	//initialize variables
 	unsigned int pwm_counter=0;    	
	unsigned int duty_cycle=30;   




	//set up state for when button C is pressed	
  
 	while(1) { 
 	 	//PWM Counter 
 	 	pwm_counter = pwm_counter + 1; 
 	 	if( pwm_counter >= PWM_TOP ){ 
 	 	 	pwm_counter = 0; 
 	 	} 
  
 	 	//PWM on yellow LED  	 	
		if (pwm_counter < duty_cycle) { 
 	 	 	turn_on_yellow_led();
			turn_on_left_motor_pwm();
			turn_on_right_motor_pwm();
 	 	}
 	 	else { 
 	 	 	turn_off_yellow_led();
			turn_off_left_motor_pwm();
			turn_off_right_motor_pwm();
	 	} 
  
 	
 	 	_delay_us(10); 
 	} 
}
