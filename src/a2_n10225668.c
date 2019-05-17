#include <stdint.h>
#include <stdio.h>
#include <avr/io.h> 
#include <avr/interrupt.h>
#include <util/delay.h>
#include <cpu_speed.h>
#include <cab202_teensy.h>
#include <graphics.h>
#include <macros.h>
#include "lcd_model.h"

//Global Variables

//Starfighter
char * starfighter = 
"     *     "
" ** *** ** "
"***********"
" **  *  ** "
"**  ***  **";
#define HEIGHT = 5;
#define WIDTH = 11;
#define y = 41;
int x = 36;

void draw_fighter() {
  for (int j = 0; j < HEIGHT; j++) {
    for (int i = 0; i < WIDTH; i++) {
      if (starfighter[i+j*width] != ' ') {
        draw_char(x+i, y+j, starfighter[i+j*width]);
      }
    }
  }
}

void move(bool direction) {
    if(direction) {
        x++;
    } else {
        x--;
    }
}

void take_input(int ch) {
    if (ch == 'w') { 
        //fire();
    } else if (ch == 'a') { 
        move(false);
    } else if (ch == 's') { 
        //sendStatus();
    } else if (ch == 'd') { 
        move(true);
    }
}

void setup() {
    draw_fighter();
}

void loop() {
    int key = wait_char();
    take_input(key);
    draw_fighter();
}

int main(void) {
	setup();
	for ( ;; ) { loop(); }
    return 0;
}