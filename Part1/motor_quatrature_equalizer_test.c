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

#define PWM_TOP 100
#define INCREMENT 5

////////////////////////
//Function Definitions//
////////////////////////

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

////////////////////////
//    Main Function   //
////////////////////////

int main() { 

	configure_zumo();//deleting this line makes it work
	configure_yellow_led();

	configure_motors();

	set_forward_right_motor_direction();
	set_forward_left_motor_direction();

	//initialize variables
 	unsigned int pwm_counter=0;    	
	unsigned int left_duty_cycle=30;   
	unsigned int right_duty_cycle=30;  
	unsigned int tolerance = 0;
 	while(1) { 

		//pwm increment
		pwm_counter++;
		if (pwm_counter >= PWM_TOP)
			pwm_counter = 0;
		
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


		//correction for drifting left
		if (get_right_quadrature_counter() + tolerance < get_left_quadrature_counter()){
			if (right_duty_cycle < PWM_TOP)
				right_duty_cycle++;
			if (left_duty_cycle > 0)
				left_duty_cycle--;
		}

		//correction for drifting right
		if (get_left_quadrature_counter() + tolerance < get_right_quadrature_counter()){
			if (left_duty_cycle < PWM_TOP)
				left_duty_cycle++;
			if (right_duty_cycle > 0)
				right_duty_cycle--;
		}

 	 	_delay_us(10); 
 	} 
}
