#include "input.h"
#include "screen.h"
#include "snake.h"
#include "vectors.h"
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "fruit.h"
struct Snake snake = {0};
struct Fruit fruit = {0};
void redraw() { snake_render(&snake); }
unsigned long current_time_millis() {
  struct timespec t;
  clock_gettime(CLOCK_MONOTONIC_RAW, &t);
  return (t.tv_sec * 1000) + (t.tv_nsec / 1000000);
}
struct Vector2 random_position() {
  return (struct Vector2){.x = rand() % (get_terminal_size().x/2), .y = rand() % get_terminal_size().y};
}
void randomize_fruit() {
  fruit.position = random_position();
}
int main() {
  hide_cursor();
  enable_raw_mode();
  atexit(show_cursor);
  atexit(reset_screen);
  signal(SIGWINCH, redraw);
  snake.trail_max_size = 2;
  snake.direction = (struct Vector2){1, 0};
  unsigned long last_update_time = current_time_millis();
  srand(last_update_time);
  snake_move_to(&snake, random_position());
  randomize_fruit();
  while (true) {
    unsigned long delta = current_time_millis() - last_update_time;
    enum SnakeInput input = read_input();
    if(input == QUIT) {
      break;
    }
    if (delta >= 500 || input != NONE) {
      snake_update(&snake, input);
      if(fruit_snake_collision(&fruit, &snake)) {
        randomize_fruit();
        snake.trail_max_size++;
      }
      last_update_time = current_time_millis();
    }
    reset_screen();
    snake_render(&snake);
    reset_styles();
    fruit_render(&fruit);
    struct timespec sleep_duration = {0};
    sleep_duration.tv_sec = 0;
    sleep_duration.tv_nsec = 50000L;
    nanosleep(&sleep_duration, NULL);
  }
}
