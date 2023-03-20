// Binghamton University
// EECE 287 Sophomore Design
// Test PWM to control the speed of the motors

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

//define PWM parameters
#define PWM_TOP 100 //maximum value of 100, indicating 100%


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

void turn_on_left_motor_pwm(){
	PORTB |= (1<< LEFT_MOTOR_PWM_LOCATION); 
}

void turn_off_left_motor_pwm(){
	PORTB &= ~(1<< LEFT_MOTOR_PWM_LOCATION); 
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

int main() { 

    //from zumo_drivers.h
    configure_zumo();


	//initialize variables
 	unsigned int pwm_counter = 0;    	
	unsigned int duty_cycle = 0;
	unsigned int left_motor_speed = 0, right_motor_speed = 0;



	//set up state for when button C is pressed	
  
 	while(1) { 

 	 	//PWM Counter 
 	 	pwm_counter = pwm_counter + 1; 
 	 	if( pwm_counter >= PWM_TOP ){ 
 	 	 	pwm_counter = 0; //once the counter reaches the top, resets to zero
 	 	} 
  
 	 	//PWM for Left Motor
		if (pwm_counter < duty_cycle) { 
 	 	 	turn_on_yellow_led();
 	 	}
 	 	else { 
 	 	 	turn_off_yellow_led();
	 	} 

		  
 		//PWM for Right Motor
		if (pwm_counter < duty_cycle) { 
 	 	 	turn_on_yellow_led();
 	 	}
 	 	else { 
 	 	 	turn_off_yellow_led();
	 	} 
  
 	 	//Pulser for button A 
 	 	if((PINB & (1 << A_BUTTON_LOCATION)) != last_button_A_state){ //if a change in state of the button is detected
 	 	 	if((PINB & (1 << A_BUTTON_LOCATION)) == 0 ){//if button is pressed
 	 	 	 	button_A_pressed = 1; 
 	 	 	} 
			//update last button state
 	 	 	last_button_A_state = (PINB & (1 << A_BUTTON_LOCATION)); 
 	 	} 
 	 	else{ 
 	 	 	button_A_pressed = 0; 
 	 	} 
 	 	 

 	 	//Pulser for C button 
		if((PINB & (1 << C_BUTTON_LOCATION)) != last_button_C_state){ 
 	 	 	if((PINB & (1 << C_BUTTON_LOCATION)) == 0 ){//if button is pressed
 	 	 	 	button_C_pressed = 1; 
 	 	 	} 
			//update last button state
 	 	 	last_button_C_state = (PINB & (1 << C_BUTTON_LOCATION)); 
 	 	} 
 	 	else{ 
 	 	 	button_C_pressed = 0; 
 	 	} 
 	 	
 	 	///-------Enter code here-------
  
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