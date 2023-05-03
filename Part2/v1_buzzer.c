#include <stdbool.h>
#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>
#include <stdio.h> 
#include "zumo_drivers.h"

#define BUZZER_PWM_PIN 7 // PD7

double a_note[] = {27.5, 55, 110, 220, 440, 880, 1760, 3520, 7040, 14080};
double a_sharp_note[] = {29.14, 58.27, 116.54, 233.08, 466.16, 932.33, 1864.66, 3729.31, 7458.62, 14917.24};
double b_note[] = {31, 62, 123.47, 246.94, 493.88, 987.77, 1975.53, 3951.07, 7902.13, 15804.26};
double c_note[] = {16.35, 32.7, 65.41, 130.81, 261.63, 523.25, 1046.5, 2093, 4186.01, 8372.02};
double c_sharp_notes[] = {17.32, 34.65, 69.3, 138.59, 277.18, 554.37, 1108.73, 2217.46, 4434.92, 8869.84};
double d_notes[] = {18.35, 36.71, 73.42, 146.83, 293.66, 587.33, 1174.66, 2349.32, 4698.64, 9397.28};
double d_sharp_notes[] = {19.45, 38.89, 77.78, 155.56, 311.13, 622.25, 1244.51, 2489.02, 4978.03, 9956.06};
double e_notes[] = {20.6, 41.2, 82.41, 164.81, 329.63, 659.26, 1318.51, 2637.02, 5274.04, 10548.08};
double f_notes[] = {21.83, 43.65, 87.31, 174.61, 349.23, 698.46, 1396.91, 2793.83, 5587.65, 11175.3};
double f_sharp_notes[] = {23.12, 46.25, 92.5, 185, 369.99, 739.99, 1479.98, 2959.96, 5919.91, 11839.82};
double g_notes[] = {24.5, 49, 98, 196, 392, 783.99, 1567.98, 3135.96, 6271.93, 12543.85};
double g_sharp_notes[] = {25.96, 51.91, 103.83, 207.65, 415.3, 830.61, 1661.22, 3322.44, 6644.88, 13289.75};

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

void play_note(double period, double duration)
{   
	int numCycles = (int) (duration * 250000 / (double) period);
    for(unsigned int cycles = 0; cycles < numCycles; cycles++)
    {
        turn_on_buzzer();
		delay_function(period);
        turn_off_buzzer();
		delay_function(period);
    }
}

double convert_to_half_period_us(double frequency){
	return (1/frequency) * 1000000/2;
}

int main()
{
    configure_buzzer();

    play_note(convert_to_half_period_us(c_sharp_note[4]), 1);
    play_note(convert_to_half_period_us(b_note[4]), 1);
    play_note(convert_to_half_period_us(c_sharp_note[4]), 1);
    play_note(convert_to_half_period_us(f_sharp_note[4]), 1);

    _delay_ms(500);

    play_note(d_note[4], .25);
    play_note(c_sharp_note[4], .25);
    play_note(d_note[4], .25);
    play_note(c_sharp_note[4], .25);
    play_note(b_note[4], .25);

    _delay_ms(500);

    play_note(d_note[4], 250);
    play_note(c_sharp_note[4], 250);
    play_note(d_note[4], 250);
    play_note(f_sharp_note[4], 500);

    _delay_ms(500);

    play_note(b_note[4], 250);
    play_note(a_note[4], 250);
    play_note(b_note[4], 250);
    play_note(a_note[4], 250);
    play_note(g_sharp_note[3], 250);
    play_note(b_note[4], 250);
    play_note(a_note[4], 500);

    _delay_ms(500);

}