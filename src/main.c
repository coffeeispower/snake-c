#include "fruit.h"
#include "input.h"
#include "screen.h"
#include "snake.h"
#include "unistd.h"
#include "vectors.h"
#include <math.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include<sys/time.h>
#ifdef _WIN32
#include <windows.h>
#endif

enum GameState { Playing, Lost, Win };

const unsigned long initial_size = 2;
struct Snake snake = {0};
struct Fruit fruit = {0};
enum GameState state = Playing;
unsigned long score(void) { return snake.trail_max_size - initial_size; }
unsigned long current_time_millis(void) {
    struct timeval t;
    gettimeofday(&t, NULL);
    return (t.tv_sec * 1000) + (t.tv_usec / 1000);
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
  struct Vector2 dialog_size;
  dialog_size.x = 40;
  dialog_size.y = 6;
  struct Vector2 dialog_position =
      (struct Vector2){.x = get_terminal_size().x / 2 - dialog_size.x / 2,
                       .y = get_terminal_size().y / 2 - dialog_size.y / 2};
  draw_rectangle_border(dialog_position, dialog_size);
  text_center("Perdeu", 0, -1);
  text_center("Aperta R para jogar outra vez", 0, 0);
}
void render_win_screen(void) {

  struct Vector2 dialog_size;
  dialog_size.x = 55;
  dialog_size.y = 6;
  struct Vector2 dialog_position =
      (struct Vector2){.x = get_terminal_size().x / 2 - dialog_size.x / 2,
                       .y = get_terminal_size().y / 2 - dialog_size.y / 2};
  draw_rectangle_border(dialog_position, dialog_size);
  text_center("Parabens por fazer o minimo ⭐", 0, -1);
  text_center("Conseguiste preencher o ecrã inteiro com a cobra", 0, 0);
}
unsigned long digits_of_number(unsigned long n) {
  unsigned long state = n;
  unsigned long count = 0;
  do {
    state /= 10;
    count++;
  } while (state != 0);
  return count;
}
void render_score(void) {
  char* score_label = "Pontuação: ";
  unsigned long score_text_size = strlen(score_label) + digits_of_number(score());
  move_cursor(get_terminal_size().x-score_text_size, get_terminal_size().y - 1);
  printf("%s%lu", score_label, score());
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
  render_score();
  fflush(stdout);
}
void handle_resize(int _signal) { redraw(); }
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
    render_lost_screen();
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
    render_win_screen();
  }
}
int main(void) {
  #ifdef _WIN32
  // Set encoding to UTF-8 for windows
  SetConsoleOutputCP(65001);
  #endif
  enable_raw_mode();
  unsigned long last_update_time = current_time_millis();
  srand(last_update_time);

  switch_to_alternative_screen();
  atexit(exit_alternative_screen);

  hide_cursor();
  atexit(show_cursor);
  atexit(reset_screen);
  #ifndef _WIN32
  signal(SIGWINCH, handle_resize);
  #else
  struct Vector2 last_terminal_size = get_terminal_size();
  #endif
  reset_game();
  redraw();
  
  while (true) {
    unsigned long delta = current_time_millis() - last_update_time;
    enum SnakeInput input = read_input();
    if (input == QUIT) {
      break;
    }
    if (input == RESTART && state != Playing) {
      reset_game();
      redraw();
    }
    if ((delta >= 500 / (powf(2, score() + 1)) || input != NONE)) {
      if (state == Playing) {
        
        delete_character_at((struct Vector2) {
          .x = snake_head(&snake)->x*2,
          .y = snake_head(&snake)->y
        });
        delete_character_at((struct Vector2) {
          .x = snake_head(&snake)->x*2+1,
          .y = snake_head(&snake)->y
        });
        delete_character_at((struct Vector2) {
          .x = snake_tip(&snake)->x*2,
          .y = snake_tip(&snake)->y
        });
        delete_character_at((struct Vector2) {
          .x = snake_tip(&snake)->x*2+1,
          .y = snake_tip(&snake)->y
        });
        snake_update(&snake, input);
        handle_fruit_collision();
        handle_self_collision();
        handle_win();
        last_update_time = current_time_millis();
        snake_render(&snake);
        render_score();
        fflush(stdout);
      }
    }
    #ifdef _WIN32
    struct Vector2 current_terminal_size = get_terminal_size();
    if (last_terminal_size.x != current_terminal_size.x || last_terminal_size.y != current_terminal_size.y) {
      handle_resize(0);
      last_terminal_size = current_terminal_size;
    }
    #endif
    // Add a bit of delay to prevent the computer from catching fire
    usleep(16666L);
  }
}
