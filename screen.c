#include "screen.h"
#include <stdio.h>
#include <sys/ioctl.h>
void move_cursor(unsigned int x, unsigned int y) {
  printf(T_ESCAPE"[%d;%dH", y+1, x+1);
}
void hide_cursor() {
  printf(T_ESCAPE"[?25l");
}

void show_cursor() {
  printf(T_ESCAPE"[?25h");
}
void clear_screen() {
  printf(T_ESCAPE"[2J");
}
void reset_screen() {
  clear_screen();
  move_cursor(0, 0);
}
struct Vector2 get_terminal_size() {
  struct winsize sz;
  ioctl( 0, TIOCGWINSZ, &sz );
  return (struct Vector2) {
    .x = sz.ws_col,
    .y = sz.ws_row
  };
}
