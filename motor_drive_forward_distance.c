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
#define INCREMENT 5
#define LEFTQUAD_TO_INCHES 1000

/////////////////////////////////
//Hardware Function Definitions//
/////////////////////////////////

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

//////////////////////////////////
// Software Function Defintions //
//////////////////////////////////
void drive_revolutions(uint32_t revolutions){
	set_forward_right_motor_direction();
	set_forward_left_motor_direction();
	
	uint32_t initial_left_revs = get_left_quadrature_counter();

	const unsigned int comparison_tolerance = 0;
	//less than 1 makes robot turn more left, greater than 1 makes robot turn more right
	const double right_turn_bias = 1.02;//seems to be between 1.02 and 1.03
	const double tweak_magnitude = 3;

 	unsigned int pwm_counter=0;    	
	double left_duty_cycle=25;   
	double right_duty_cycle=25;  
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
 	} while (left_revs < initial_left_revs + revolutions);
	
	
	turn_on_left_motor_pwm();
	turn_off_left_motor_pwm();
}

void drive_forward_distance (uint8_t distance_inches){
	drive_revolutions(distance_inches * LEFTQUAD_TO_INCHES);
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

	oled_clear();
	oled_set_cursor(0,0);
	oled_put_hex(0x03);

	drive_forward_distance(12);
}
