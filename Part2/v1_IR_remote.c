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

int main(){
	
	configure_zumo();
	configure_proximity_sensors();
	
	oled_clear();
	oled_set_cursor(0,0);
	
    uint32_t remote_data;
		
	loop:

    remote_data = wait_for_remote_command();


    oled_put_hex(remote_data>>12);
    oled_put_hex(remote_data>>8);
    oled_put_hex(remote_data>>4);
    oled_put_hex(remote_data);

    _delay_us(10);

    button:
        while(button_b_is_down())
        {
            goto loop;
        }
	goto button;

	return 0;
}