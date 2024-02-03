#include "fruit.h"
#include "input.h"
#include "screen.h"
#include "snake.h"
#include "vectors.h"
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
enum GameState { Playing, Lost, Win };
struct Snake snake = {0};
struct Fruit fruit = {0};
enum GameState state = Playing;
unsigned long current_time_millis() {
  struct timespec t;
  clock_gettime(CLOCK_MONOTONIC_RAW, &t);
  return (t.tv_sec * 1000) + (t.tv_nsec / 1000000);
}
struct Vector2 random_position() {
  return (struct Vector2){.x = rand() % (get_terminal_size().x / 2),
                          .y = rand() % get_terminal_size().y};
}
void randomize_fruit() { fruit.position = random_position(); }
void text_center(const char *text, int offset_x, int offset_y) {
  struct Vector2 text_position = (struct Vector2){
      .x = get_terminal_size().x / 2 - strlen(text) / 2 + offset_x,
      .y = get_terminal_size().y / 2 + offset_y};

  move_cursor(text_position.x, text_position.y);
  puts(text);
}
void render_lost_screen() {
  struct Vector2 dialog_size = get_terminal_size();
  dialog_size.x *= 0.3;
  dialog_size.y = 6;
  struct Vector2 dialog_position =
      (struct Vector2){.x = get_terminal_size().x / 2 - dialog_size.x / 2,
                       .y = get_terminal_size().y / 2 - dialog_size.y / 2};
  draw_rectangle_border(dialog_position, dialog_size);
  text_center("Burro do krl 🤦", 0, -1);
  text_center("Aperta R para jogar outra vez", 0, 0);
  fflush(stdout);
}
void render_win_screen() {

  struct Vector2 dialog_size = get_terminal_size();
  dialog_size.x *= 0.3;
  dialog_size.y = 6;
  struct Vector2 dialog_position =
      (struct Vector2){.x = get_terminal_size().x / 2 - dialog_size.x / 2,
                       .y = get_terminal_size().y / 2 - dialog_size.y / 2};
  draw_rectangle_border(dialog_position, dialog_size);
  text_center("Parabens por fazer o minimo ⭐", 0, -1);
  text_center("Conseguiste preencher o ecrã inteiro com a cobra", 0, 0);
  fflush(stdout);
}
void redraw() {
  reset_screen();
  snake_render(&snake);
  reset_styles();
  fruit_render(&fruit);

  switch (state) {
  case Playing:
    break;
  case Lost:
    render_lost_screen();
    break;
  case Win:
    render_win_screen();
    break;
  }
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
    if (input == QUIT) {
      break;
    }
    if ((delta >= 500 / snake.trail_max_size || input != NONE) &&
        state == Playing) {
      snake_update(&snake, input);
      if (snake_check_self_collision(&snake) ||
          snake_head(&snake)->x >
              get_terminal_size().x/2 - 1 ||
          snake_head(&snake)->y>
              get_terminal_size().y - 1 ||
          snake_head(&snake)->x < 0 ||
          snake_head(&snake)->y < 0) {
        state = Lost;
      }
      if (fruit_snake_collision(&fruit, &snake)) {
        randomize_fruit();
        snake.trail_max_size++;
      }
      if (snake.trail_max_size >=
          (get_terminal_size().x / 2) * get_terminal_size().y) {
        state = Win;
      }
      last_update_time = current_time_millis();
    }
    redraw();
    struct timespec sleep_duration = {0};
    sleep_duration.tv_sec = 0;
    sleep_duration.tv_nsec = 50000L;
    nanosleep(&sleep_duration, NULL);
  }
}
