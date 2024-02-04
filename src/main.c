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
const unsigned long initial_size = 2;
struct Snake snake = {0};
struct Fruit fruit = {0};
enum GameState state = Playing;

unsigned long score(void) { return snake.trail_max_size - initial_size; }

unsigned long current_time_millis(void) {
  struct timespec t;
  clock_gettime(CLOCK_MONOTONIC_RAW, &t);
  return (t.tv_sec * 1000) + (t.tv_nsec / 1000000);
}
struct Vector2 random_position(void) {
  return (struct Vector2){.x = rand() % (get_terminal_size().x / 2),
                          .y = rand() % get_terminal_size().y};
}
void randomize_fruit(void) { fruit.position = random_position(); }
void text_center(const char *text, int offset_x, int offset_y) {
  struct Vector2 text_position = (struct Vector2){
      .x = get_terminal_size().x / 2 - strlen(text) / 2 + offset_x,
      .y = get_terminal_size().y / 2 + offset_y};

  move_cursor(text_position.x, text_position.y);
  puts(text);
}
void render_lost_screen(void) {
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
void render_win_screen(void) {

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
void redraw(void) {
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
  move_cursor(0, get_terminal_size().y - 1);
  printf("Pontuação: %lu", score());
  fflush(stdout);
}
void handle_resize(int signal) {
  redraw();
}
void reset_game(void) {
  snake.trail_max_size = 2;
  snake.direction = (struct Vector2){1, 0};
  snake.trail_size = 0;
  snake_move_to(&snake, random_position());
  randomize_fruit();
  state = Playing;
}
void handle_self_collision(void) {
  if (snake_check_self_collision(&snake) ||
      snake_head(&snake)->x > get_terminal_size().x / 2 - 1 ||
      snake_head(&snake)->y > get_terminal_size().y - 1 ||
      snake_head(&snake)->x < 0 || snake_head(&snake)->y < 0) {
    state = Lost;
  }
}
void handle_fruit_collision(void) {
  if (fruit_snake_collision(&fruit, &snake)) {
    randomize_fruit();
    snake.trail_max_size++;
  }
}
void handle_win(void) {
  if (snake.trail_max_size >=
      (get_terminal_size().x / 2) * get_terminal_size().y) {
    state = Win;
  }
}
int main(void) {
  unsigned long last_update_time = current_time_millis();
  srand(last_update_time);

  hide_cursor();
  enable_raw_mode();
  atexit(show_cursor);
  atexit(reset_screen);
  signal(SIGWINCH, handle_resize);

  reset_game();

  while (true) {
    unsigned long delta = current_time_millis() - last_update_time;
    enum SnakeInput input = read_input();
    if (input == QUIT) {
      break;
    }
    if (input == RESTART && state != Playing) {
      reset_game();
    }
    if ((delta >= 500 / (score() + 1) || input != NONE)) {
      if (state == Playing) {
        snake_update(&snake, input);
        handle_fruit_collision();
        handle_self_collision();
        handle_win();
        last_update_time = current_time_millis();
      }
      redraw();
    }
    struct timespec sleep_duration = {0};
    sleep_duration.tv_sec = 0;
    sleep_duration.tv_nsec = 50000L;
    nanosleep(&sleep_duration, NULL);
  }
}
