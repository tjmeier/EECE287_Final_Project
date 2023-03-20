// Binghamton University
// EECE 287 Sophomore Design
// Test PWM to control the speed of the motors

#include <avr/io.h> 
#include <util/delay.h> 
#include <stdint.h>
#include <stdbool.h>

#define A_BUTTON_LOCATION 3
#define YELLOW_LED_LOCATION 7

//replace X with the pin mapping for button C
#define C_BUTTON_LOCATION 0

//define PWM parameters
#define PWM_TOP 100 //maximum value of 100, indicating 100%
#define INCREMENT 5 //how much the duty cycle changes with each button press 

void configure_A_button(){
	
	DDRB &= ~(1<< A_BUTTON_LOCATION); 
	PORTB |= (1<< A_BUTTON_LOCATION); 
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


//write code below to configure and set up button C 

void configure_C_button(){
	
	DDRB &= ~(1<< C_BUTTON_LOCATION); 
	PORTB |= (1<< C_BUTTON_LOCATION);		//it should start at 1?
}


int main() { 

	//configure ports
	configure_yellow_led();
	configure_A_button();
	configure_C_button();

	//add code to configure button C 
	unsigned int last_button_C_state = (PINB & (1 << C_BUTTON_LOCATION));
	unsigned int button_C_pressed;

	//initialize variables
 	unsigned int pwm_counter=0;    	
	unsigned int duty_cycle=30;   
	unsigned int last_button_A_state = (PINB & (1 << A_BUTTON_LOCATION));
	unsigned int button_A_pressed;



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