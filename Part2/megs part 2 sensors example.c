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

//right sensor
void configure_right_proximity_sensor()
{
	DDRD &= ~(1<<RIGHT_SENSOR_PIN);
	PORTD |= (1<<RIGHT_SENSOR_PIN);
}


//higher return value indicates you are closer to an object
uint8_t read_right_proximity_sensor() 
{
	//turn off the line sensor IR LEDS to remove interference
	DDRB &= ~(1<<7);
	PORTB &= ~(1<<7);
	
	uint8_t right_sensor_detection_factor = 0;
	
	for (int i = 0; i < 6; i++){ //6 is the number of brightness levels in the array

		uint16_t brightness_duty_cycle = BrightnessLevels[i];
		
		start_IR_emitters(0, brightness_duty_cycle); //0 indicates to use right IR emitters
		_delay_us(420);
		
		if(!(PIND & (1<<RIGHT_SENSOR_PIN)))
			right_sensor_detection_factor++;
		
		stop_IR_emitters();
		_delay_us(550);
		
		start_IR_emitters(1, brightness_duty_cycle); //1 indicates to use left IR emitters
		_delay_us(420);
		
		if(!(PIND & (1<<RIGHT_SENSOR_PIN)))
			right_sensor_detection_factor++;
		stop_IR_emitters();
		_delay_us(550);
		
	}
	return right_sensor_detection_factor;
}


void configure_proximity_sensors()
{
	void configure_right_proximity_sensor();
	//configure others
}


//left sensor
void configure_left_proximity_sensor()
{
	//configure left proximity sensor
}

uint8_t read_left_proximity_sensor() 
{
	//turn off line sensor IR LEDS to remove interference
	DDRB &= ~(1<<7);
	PORTB &= ~(1<<7);
	
	//------------write code here: refer to read_right_proximity_sensor ---------

}

//front sensor
void configure_front_proximity_sensor()
{
	//configure right proximity sensor
}

uint8_t read_front_proximity_sensor()
{
	//turn off line sensor IR LEDS to remove interference
	DDRB &= ~(1<<7);
	PORTB &= ~(1<<7);
	
	//------------write code here: refer to read_right_proximity_sensor ---------
}


int main(){
	
	configure_zumo();
	configure_proximity_sensors();
	
	oled_clear();
	oled_set_cursor(0,0);
	
	uint8_t right_proximity_value = 0;

	while(1){
		
		oled_set_cursor(0,0);


		right_proximity_value = read_right_proximity_sensor();
		oled_put_hex((right_proximity_value/10)%1000); //use the values you see to threshold detection of an object
		oled_put_hex((right_proximity_value/10)%100);
		oled_put_hex((right_proximity_value/10)%10);
		oled_put_hex(right_proximity_value%10);
		
		_delay_us(10);

	}
	
	return 0;
}
