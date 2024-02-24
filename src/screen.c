#include "screen.h"
#include "vectors.h"
#include <stdio.h>
#ifdef _WIN32
#include <windows.h>
#else
#include <sys/ioctl.h>
#endif

void move_cursor(int x, int y) {
  if(x < 0) x = 0;
  if(y < 0) y = 0;
  if(x > get_terminal_size().x) x = get_terminal_size().x;
  if(y > get_terminal_size().y) y = get_terminal_size().y;
  printf("\e[%d;%dH", y+1, x+1);
}
void hide_cursor(void) {
  printf("\e[?25l");
}

void show_cursor(void) {
  printf("\e[?25h");
}
void clear_screen(void) {
  printf("\e[2J");
  printf("\e[3J");
}
void reset_screen(void) {
  clear_screen();
  move_cursor(0, 0);
}
void reset_styles(void) {
  printf("\e[2;0;0m\e[0m");
}
void switch_to_alternative_screen(void) {
  printf("\e[?1049h");
}
void exit_alternative_screen(void) {
  printf("\e[?1049l");
}
struct Vector2 get_terminal_size(void) {
  struct Vector2 result;
  #ifdef _WIN32
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    int columns, rows;

    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    result.x = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    result.y = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
  #else
  
  struct winsize sz;
  ioctl( 0, TIOCGWINSZ, &sz );
  result.x = sz.ws_col;
  result.y = sz.ws_row;
  #endif
  return result;
}

void set_color_rgb(unsigned int color) {
  printf("\e[38;2;%d;%d;%dm", (color & 0xff0000) >> 16, (color & 0xff00) > 8, color & 0xff);
}

void set_bg_color_rgb(unsigned int color) {
  printf("\e[48;2;%d;%d;%dm", (color & 0xff0000) >> 16, (color & 0xff00) > 8, color & 0xff);
}

void set_color_256(unsigned char color) {
  printf("\e[38;5;%dm", color);
}

void set_bg_color_256(unsigned char color) {
  printf("\e[48;5;%dm", color);
}

void draw_rectangle_border(struct Vector2 position, struct Vector2 size){
  move_cursor(position.x, position.y);
  printf("┏");
  for (int i = 1; i < size.x-1; i++) {
    printf("━");
  }
  printf("┓");
  for(int i = 1; i < size.y-1; i++) {  
    move_cursor(position.x, position.y+i);
    printf("┃");
    for (int i = 1; i < size.x-1; i++) {
      printf(" ");
    }
    printf("┃");
  }
  
  move_cursor(position.x, position.y+size.y-1);
  printf("┗");
  for (int i = 1; i < size.x-1; i++) {
    printf("━");
  }
  printf("┛");
}
void delete_character_at(struct Vector2 position) {
  move_cursor(position.x, position.y);
  reset_styles();
  printf(" ");
}
