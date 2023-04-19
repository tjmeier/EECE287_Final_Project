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
#define REMOTE_LEFT_ARROW 0x821
#define REMOTE_RIGHT_ARROW 0xAA1
#define REMOTE_ENTER 0xB20
#define REMOTE_SETUP 0xA41
#define REMOTE_STOP 0xAC1
#define REMOTE_VOL_DOWN 0xA01
#define REMOTE_VOL_UP 0xA81
#define REMOTE_PAUSE 0xB00
#define REMOTE_BACK 0xAE1

#define REMOTE_VERIFICATION_CONSTANT 0xFE01F

uint16_t get_remote_button() {
	uint32_t remote_data;
	
	do{
		remote_data = wait_for_remote_command();

	}while (remote_data>>12 != REMOTE_VERIFICATION_CONSTANT); //looks at most siginifcant 20 bits (5 hex values)

	return remote_data - (REMOTE_VERIFICATION_CONSTANT << 12); //subtracts FE01F000
}

int main(){
	
	configure_zumo();
	
	oled_clear();
	
    uint32_t remote_data;
		
	loop:
	oled_set_cursor(0,0);

	remote_data = get_remote_button();

	oled_put_hex(remote_data>>8);
	oled_put_hex(remote_data>>4);
	oled_put_hex(remote_data);

    _delay_us(10);

	//wait for button to be pressed
    button:
        while(button_b_is_up())
        {
            goto button;
        }

	oled_clear();
	goto loop;

	return 0;
}