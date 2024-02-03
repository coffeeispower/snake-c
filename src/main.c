#include "input.h"
#include "screen.h"
#include "snake.h"
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
struct Snake snake = {0};
void redraw() { snake_render(&snake); }
unsigned long current_time_millis() {
  struct timespec t;
  clock_gettime(CLOCK_MONOTONIC_RAW, &t);
  return (t.tv_sec * 1000) + (t.tv_nsec / 1000000);
}
int main() {
  hide_cursor();
  enable_raw_mode();
  unsigned long last_update_time = current_time_millis();
  atexit(show_cursor);
  signal(SIGWINCH, redraw);
  snake.trail_max_size = 5;
  snake.direction = (struct Vector2){1, 0};
  snake_move_to(&snake, (struct Vector2){.x = 6, .y = 4});
  while (1) {
    unsigned long delta = current_time_millis() - last_update_time;
    enum SnakeInput input = read_input();
    if(input == QUIT) {
      break;
    }
    if (delta >= 500 || input != NONE) {
      snake_update(&snake, input);
      last_update_time = current_time_millis();
    }
    reset_screen();
    snake_render(&snake);
    struct timespec sleep_duration = {0};
    sleep_duration.tv_sec = 0;
    sleep_duration.tv_nsec = 50000L;
    nanosleep(&sleep_duration, NULL);
  }
}
