#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <avr/io.h> 
#include <avr/interrupt.h>
#include <util/delay.h>
#include <cpu_speed.h>
#include <graphics.h>
#include <macros.h>
#include "lcd_model.h"

//Global Variables
uint8_t paused = 0;

//Starfighter
#define HEIGHT 5
#define WIDTH 11
#define y 41
uint8_t x = 36;

uint8_t starfighter[55] = {
    0,0,0,0,0,1,0,0,0,0,0,
    0,1,1,0,1,1,1,0,1,1,0,
    1,1,1,1,1,1,1,1,1,1,1,
    0,1,1,0,0,1,0,0,1,1,0,
    1,1,0,0,1,1,1,0,0,1,1};

void draw_fighter() {
    for(uint8_t j = 0; j < HEIGHT; j++) {
        for (uint8_t i = 0; i < WIDTH; i++) {
            if (starfighter[(j*WIDTH)+i]) {
                draw_pixel(x+i, y+j, FG_COLOUR);
            }
        }
    }
    draw_line(0, 39, 83, 39, FG_COLOUR);
}

void move(uint8_t direction) {
    if(direction == 1) {
        x++;
    } else {
        x--;
    }
}

void set_bits() {
    CLEAR_BIT(DDRF, 5);
    CLEAR_BIT(DDRF, 6);
    SET_BIT(DDRB, 2);
}

void setup() {
    set_clock_speed(CPU_8MHz);
    lcd_init(40);
    lcd_clear();
    set_bits();
    paused = 0;
    x = 36;
    draw_fighter();
}

void take_input() {
    if (BIT_IS_SET(PINF, 6)) {
        setup();
    } if (BIT_IS_SET(PINF, 5)) {
        //quit();
    } if (BIT_IS_SET(PINB, 0)) {
        if (paused) {
            paused = 0;
        } else {
            paused = 1;
        }
    } if (BIT_IS_SET(PIND, 1)) { 
        SET_BIT(PORTB, 2);
        //fire();
    } else if (BIT_IS_SET(PINB, 1)) { 
        move(0);
    } else if (BIT_IS_SET(PINB, 7)) { 
        CLEAR_BIT(PORTB, 2);
        //sendStatus();
    } else if (BIT_IS_SET(PIND, 0)) {    
        move(1);
    }
}

void loop() {
    clear_screen();
    draw_fighter();
    show_screen();
    take_input();
}

int main(void) {
	setup();
	for ( ;; ) {
        if (!paused) {
        loop();
        }
    }
    return 0;
}