#include <stdbool.h>
#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>
#include <stdio.h> 
#include "zumo_drivers.h"

#define BUZZER_PWM_PIN 7 // PD7

int a_note[] = {18182, 9091, 4546, 2273, 1136, 568, 284, 142, 71};
int a_sharp_note[] = {17193, 8597, 4298, 2149, 1075, 537, 269, 134, 67};
int b_note[] = {16195, 8097, 4049, 2024, 1012, 506, 253, 127, 63};
int c_note[] = {30675, 15337, 7669, 3834, 1917, 959, 479, 240, 120};
int c_sharp_note[] = {28865, 14432, 7216, 3608, 1804, 902, 451, 226, 113};
int d_note[] = {27391, 13695, 6848, 3424, 1712, 856, 428, 214, 107};
int d_sharp_note[] = {25775, 12887, 6444, 3222, 1611, 805, 403, 201, 101};
int e_note[] = {24142, 12071, 6035, 3018, 1509, 754, 377, 189, 94};
int f_note[] = {22727, 11363, 5682, 2841, 1420, 710, 355, 177, 89};
int f_sharp_note[] = {21431, 10716, 5358, 2679, 1339, 670, 335, 167, 84};
int g_note[] = {20248, 10124, 5062, 2531, 1266, 633, 316, 158, 79};
int g_sharp_note[] = {19159, 9580, 4789, 2395, 1198, 599, 299, 150, 75};

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

void play_note(int period, unsigned int duration)
{   
	int numCycles = (int) duration * 100000 / period;
    for(unsigned int cycles = 0; cycles < numCycles; cycles++)
    {
        turn_on_buzzer();
		delay_function(period);
        turn_off_buzzer();
		delay_function(period);
    }
}

int main()
{
    configure_buzzer();

	play_note(a_note[4], 10);

	play_note(c_note[3], 5);

	while(1)
	{
        turn_on_buzzer();
		delay_function(a_note[4]);
        turn_off_buzzer();
		delay_function(a_note[4]);
	}

}