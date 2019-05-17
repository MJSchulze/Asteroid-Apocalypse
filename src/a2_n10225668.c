#include <stdint.h>
#include <stdio.h>
#include <avr/io.h> 
#include <avr/interrupt.h>
#include <util/delay.h>
#include <cpu_speed.h>
#include <graphics.h>
#include <macros.h>
#include "lcd_model.h"

//Global Variables

//Starfighter

#define HEIGHT 5
#define WIDTH 11
#define y 41

char * starfighter = 
"     *     "
" ** *** ** "
"***********"
" **  *  ** "
"**  ***  **";
int x = 36;

void draw_fighter() {
  for (int j = 0; j < HEIGHT; j++) {
    for (int i = 0; i < WIDTH; i++) {
      if (starfighter[i+j*WIDTH] != ' ') {
        draw_char(x+i, y+j, starfighter[i+j*WIDTH], FG_COLOUR);
      }
    }
  }
  draw_line(0, 39, 83, 39, FG_COLOUR);
}

void move(int direction) {
    if(direction == 1) {
        x++;
    } else {
        x--;
    }
}

void take_input() {
    if (BIT_IS_SET(PIND, 1)) { 
        //fire();
    } else if (BIT_IS_SET(PINB, 1)) { 
        move(0);
    } else if (BIT_IS_SET(PINF, 7)) { 
        //sendStatus();
    } else if (BIT_IS_SET(PIND, 0)) {    
        move(1);
    }
}

void setup() {
    draw_fighter();
    lcd_init(LCD_DEFAULT_CONTRAST);
}

void loop() {
    clear_screen();
    draw_fighter();
    show_screen();
    take_input();
}

int main(void) {
	setup();
	for ( ;; ) { loop(); }
    return 0;
}