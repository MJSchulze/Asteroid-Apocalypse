#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <cab202_graphics.h>
#include <cab202_timers.h>
#include "helpers.h"
// Insert other functions here, or include header files

// Global variables
bool game_over = false;
int delay = 10;
int timeout = INT_MAX;
double start_time;

// Device state
double x, y;
#define VACUUM_WIDTH 11
#define VACUUM_HEIGHT 9
bool paused = true;
int angle, battery, time, load;
int num_dust, num_slime, num_trash;
int add_dust, add_slime, add_trash;

// Status
char * timeString, directionString, weightString, batteryString, rubbishString;

// Specification 4
void draw_border() {
    int w, h;
    get_screen_size(&w, &h);
    const int chVert = '|';
    const int chHori = '-';
    const int chCorn = '+';
    draw_line(0, 0, 0, h - 3, chVert);
    draw_line(0, 0, w - 1, 0, chHori);
    draw_line(0, h - 3, w - 1, h - 3, chHori);
    draw_line(w - 1, 0, w - 1, h - 3, chVert);
    draw_char(0, 0, chCorn);
    draw_char(0, h-3, chCorn);
    draw_char(w-1, 0, chCorn);
    draw_char(w-1, h-3, chCorn);
}

/*char * concat(char * s1, char * s2) {
    char * result;
    strcpy(result, s1);
    strcat(result, s2);
    return result;
}

void update_status() {
    time = get_current_time() - (int) start_time;
    itoa(time, timeString, 10);
    timeString = concat("Elapsed time: ", timeString);

    
}*/

void draw_status_info(int wLeft, int wCenter) {
    draw_string(2, 1, "Student Number: n10225668");
    draw_string(2, 3, "Elapsed Time: ");
    draw_string(wLeft+2, 1, "Direction: ");
    draw_int(wLeft+13, 1, angle);
    draw_int(wLeft+15, 1, x);
    draw_int(wLeft+18, 1, y);
    draw_string(wLeft+2, 3, "Weight: ");
    draw_string(wLeft+wCenter+2, 1, "Battery Level: ");
    draw_string(wLeft+wCenter+2, 3, "Rubbish Remaining: ");
}

void draw_status() {
    int screenSize = screen_width();
    int wCenter = screenSize/3;
    int screenSizeS = screenSize - wCenter;
    int wLeft = screenSizeS/2;

    draw_line(wLeft, 0, wLeft, 4, '|');
    draw_line(wLeft+wCenter, 0, wLeft+wCenter, 4, '|');
    for(int i = 0; i <= 4; i+=2) {
        if(i > 1) {
            draw_line(0, i, screenSize-1, i, '-');
            draw_line(0, i, screenSize-1, i, '-');
            draw_char(0, i, '+');
            draw_char(screenSize-1, i, '+');
            draw_char(wLeft, i, '+');
            draw_char(wLeft+wCenter, i, '+');
        } else {
            draw_char(wLeft, i, '+');
            draw_char(wLeft+wCenter, i, '+');
        }
    }
    //update_status();

    draw_status_info(wLeft, wCenter);
}

bool timed_out() {
    return get_current_time() - start_time >= timeout;
}

void setup_vacuum() {
    int w, h;
    get_screen_size(&w, &h);
    x = w / 2;
    y = (h / 2);
}

bool is_opaque(int x, int y, int left, int top, int width, int height, char * pixels) {
  return
  x >= left &&
  x < left + width &&
  y >= top &&
  y < top + height &&
  pixels[(x-left)+(y-top)*width] != ' ';
}

bool pixel_collision(int x0, int y0, int w0, int h0, char * pixels0, int x1, int y1, int w1, int h1, char * pixels1) {
  for (int y = y0; y < y0+h0; y++) {
    for (int x = x0; x < x0+w0; x++) {
      if (is_opaque(x, y, x0, y0, w0, h0, pixels0) && is_opaque(x, y, x1, y1, w1, h1, pixels1)) {
        return true;
      }
    }
  }
  return false;
}

bool box_collision(int x0, int y0, int w0, int h0, int x1, int y1, int w1, int h1) {
  if (  x0 < x1 + w1 && 
        x0 + w0 > x1 && 
        y0 < y1+h1 && 
        y0 + h0 > y1) {
      return true;
  }
  return false;
}

void draw_vacuum(int centerX, int centerY, int width, int height, char * bitmap) {
  for (int j = 0; j < height; j++) {
    for (int i = 0; i < width; i++) {
      if (bitmap[i+j*width] != ' ') {
        draw_char(centerX+i-5, centerY+j-4, bitmap[i+j*width]);
      }
    }
  }
}

char * vacuum =
"    @@@    "
"  @@@@@@@  "
" @@@@@@@@@ "
"@@@@@@@@@@@"
"@@@@@@@@@@@"
"@@@@@@@@@@@"
" @@@@@@@@@ "
"  @@@@@@@  "
"    @@@    "
;

void drop_dust() {
    add_dust += get_int("How many pieces of dust?");
    if (add_dust > 1000) {
        add_dust = 1000;
    }
}
void drop_slime() {
    add_slime += get_int("How many pieces of dust?");
    if (add_slime > 1000) {
        add_slime = 1000;
    }
}
void drop_trash() {
    add_trash += get_int("How many pieces of dust?");
    if (add_trash > 1000) {
        add_trash = 1000;
    }
}

void dust() {
    int w, h;
    get_screen_size(&w, &h);
}

void slime() {

}

void trash() {

}

void drop() {
    for (int i = num_dust; i < add_dust; i++) {
        dust();
    }
    for (int i = num_slime; i < add_slime; i++) {
        slime();
    }
    for (int i = num_trash; i < add_trash; i++) {
        trash();
    }
}

// 15.i
bool is_rubish_type(int ch) {
    return (ch == 'd' || ch == 's' || ch == 't');
}
void drop_rubbish(int ch) {
    if (ch == 'd') {
        drop_dust();
        drop();
    }
    else if (ch == 's') {
        drop_slime();
        drop();
    }
    else if (ch == 't') {
        drop_trash();
        drop();
    }
}

// 15.ii
bool is_device_ctrl(int ch) {
    return (ch == 'i' || ch == 'j' || ch == 'k' || ch == 'l');
}
void update_hero(int ch) {
    if (ch == 'j' && x > (VACUUM_HEIGHT/2)+2) {
        x--;
    } else if (ch == 'i' && y > (VACUUM_WIDTH/2)+4) {
        y--;
    } else if (ch == 'l' && x < screen_width() - (VACUUM_WIDTH/2)-2) {
        x++;
    } else if (ch == 'k' && y < screen_height() - (VACUUM_HEIGHT/2)-4) {
        y++;
    }
}

// 15.iii
void do_delay() {
    delay = get_int("How much delay shall the program have?");
}

// 15.iv
void do_timeout() {
    timeout = get_int("How many seconds shall the program run?");
}

// 15.v
void do_farewell() {
    draw_formatted_lines(0, 0,
        "!!!!!!!!!!!!!!!!!!!!!!!!!\n"
        "!!! Robotic Vacuum Cleaner !!!\n"
        "!!!!!!!!!!!!!!!!!!!!!!!!!\n"
        "\n"
        "Goodbye\n"
        "Thanks for playing Robotic Vacuum Cleaner\n"
        "Press any key to exit."
    );
    show_screen();

    while (get_char() > 0) {}
    while (!timed_out() && wait_char() <= 0) {}
    exit(0);
}

void return_to_base() {

}

void setup_rubbish() {
    drop_dust();
    drop_slime();
    drop_trash();
    drop();
}

void setup () {
    start_time = get_current_time();
    setup_vacuum();
    setup_rubbish();
    angle = 0;
}

// 15.vi
void reset() {
    setup();
}

// 15.vii
void do_settings() {
    x = get_int("Enter new x value:");
    y = get_int("Enter new y value:");
    angle = get_int("Enter new angle value:");
}

// 15.viii
void do_weight() {
    load = get_int("Enter new load value:");
    if (load < 0) load = 0;
}

// 15.ix
void do_battery_level() {
    battery = get_int("Enter new battery level:");
    if (battery < 0) battery = 100;
}

// 15.x
void do_help() {
    clear_screen();
    draw_formatted_lines(0, 0,
        "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n"
        "!!! Robotic Vacuum Cleaner                            !!!\n"
        "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n"
        "\n"
        "b: immediately return to base\n"
        "i, j, k, l: push device 1 unit North, West, South, or East\n"
        "d, s, t: litter cheat - drop litter\n"
        "m: change the millisecond loop delay\n"
        "r: restart\n"
        "o: specify a time-out (seconds) after which the program terminates\n"
        "p: pause or resume vacuum movement\n"
        "q: quit\n"
        "v: vacuum cheat 1 - override the position of the vacuum\n"
        "w: vacuum cheat 2 - override the load of the vacuum (grams)\n"
        "y: vacuum cheat 3 - override battery level\n"
        "?: display this help screen\n\n\n"
        "\t\t\tPress a key to return to the game."
    );
    show_screen();

    while (get_char() > 0) {}
    while (!timed_out() && wait_char() <= 0) {}
}

void pause() {
    if (paused) {
        paused = false;
    }
    else {
        paused = true;
    }
}

// Specification 15
void do_operation(int ch) {
    if (is_rubish_type(ch)) { 
        drop_rubbish(ch);
    } else if (is_device_ctrl(ch)) { 
        update_hero(ch);
    } else if (ch == 'p') {
        pause();
    } else if (ch == 'b') { 
        return_to_base();
    } else if (ch == 'm') { 
        do_delay();
    } else if (ch == 'o') { 
        do_timeout();
    } else if (ch == 'q') { 
        do_farewell();
    } else if (ch == 'r') { 
        reset();
    } else if (ch == 'v') { 
        do_settings();
    } else if (ch == 'w') { 
        do_weight();
    } else if (ch == 'y') { 
        do_battery_level();
    } else if (ch == '?') { 
        do_help();
    }
}

int againstWall() {
    if (x == (VACUUM_HEIGHT/2)+2) {
        return 2;
    } else if (y == (VACUUM_WIDTH/2)+4) {
        return 3;
    } else if (x == screen_width() - (VACUUM_WIDTH/2)-2) {
        return 4;
    } else if (y == screen_height() - (VACUUM_HEIGHT/2)-4) {
        return 1;
    }
    return 0;
}

void rotate() {
    int r = rand() % 60;
    if (r < 30) {
        angle = (angle + r + 30)%360;
    } else {
        angle = (angle - r)%360;
    }
}

float toRadians(int degrees) {
    float radians = degrees*(M_PI/180);
    return radians;
}

int toDegrees(int radians) {
    int degrees = radians*(180/M_PI);
    return degrees;
}

void do_movement() {
    if (againstWall() == 1 && angle < 90 && angle > 270) {
        rotate();
    } else if (againstWall() == 2 && angle < 180) {
        rotate();
    } else if (againstWall() == 3 && angle > 90 && angle < 270) {
        rotate();
    } else if (againstWall() == 4 && angle > 180) {
        rotate();
    } else {
        y+=tan(toRadians(angle))*0.2;
        x+=sin(toRadians(angle))*0.2;
    }
    
}

void loop() {
    clear_screen();
    draw_border();
    draw_status();
    draw_vacuum(x, y, VACUUM_WIDTH, VACUUM_HEIGHT, vacuum);
    show_screen();
    int key = wait_char();
    do_operation(key);
    if (!paused) {
        do_movement();
        //check_collision();
    }
}

int main() {

    srand(get_current_time());

    setup_screen();
    setup();

    while (!game_over && !timed_out()) {
        loop();
        timer_pause(delay);
    }

    return 0;
}