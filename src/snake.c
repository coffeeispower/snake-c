#include "snake.h"
#include "input.h"
#include "screen.h"
#include "vectors.h"
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

void snake_move_to(struct Snake *self, struct Vector2 new_head_position) {
  snake_push(self, new_head_position);
  if (self->trail_size > self->trail_max_size) {
    snake_pop_tip(self);
  }
}

struct Vector2 snake_pop_tip(struct Snake *self) {
  assert(self->trail_size > 0 && "trail was popped when the trail was empty");
  struct Vector2 popped_element = *snake_tip(self);
  for (unsigned long i = 1; i < self->trail_size; i++) {
    self->trail_data[i - 1] = self->trail_data[i];
  }
  self->trail_size--;
  return popped_element;
}

void snake_push(struct Snake *self, struct Vector2 new_head_position) {
  if (self->trail_capacity <= self->trail_size) {
    self->trail_capacity += 2*((self->trail_capacity/4)+1);
    self->trail_data = realloc(self->trail_data, self->trail_capacity * sizeof(struct Vector2));
  }
  self->trail_data[self->trail_size] = new_head_position;
  self->trail_size += 1;
}

struct Vector2 *snake_head(struct Snake *self) {
  return &self->trail_data[self->trail_size - 1];
}
struct Vector2 *snake_tip(struct Snake *self) { return &self->trail_data[0]; }
bool snake_is_head(struct Snake *self, struct Vector2 v) {
  struct Vector2 head = *snake_head(self);
  return head.x == v.x && head.y == v.y;
}
bool snake_check_self_collision(struct Snake *self) {
  for (int i = 0; i < self->trail_size - 1; i++) {
    if (snake_is_head(self, self->trail_data[i])) {
      return true;
    }
  }
  return false;
}
void snake_render(struct Snake *self) {
  set_bg_color_256(41);
  set_color_256(0);
  for (unsigned long i = 0; i < self->trail_size; i++) {
    struct Vector2 pos = self->trail_data[i];
    if (pos.x < 0 || pos.y < 0 || pos.x * 2 >= get_terminal_size().x ||
        pos.y >= get_terminal_size().y) {
      continue;
    }
    bool is_next_to_the_last_pos = i > 0 && self->trail_data[i-1].x == pos.x-1;
    if(!is_next_to_the_last_pos){
      move_cursor(pos.x * 2, pos.y);
    }
    if (snake_is_head(self, pos)) {
      printf("··");
    } else {
      printf("  ");
    }
  }
  reset_styles();
}
void snake_update(struct Snake *self, enum SnakeInput input) {
  switch (input) {
  case UP:
    if (self->direction.x != 0)
      self->direction = (struct Vector2){0, -1};
    break;
  case DOWN:
    if (self->direction.x != 0)
      self->direction = (struct Vector2){0, 1};
    break;
  case LEFT:
    if (self->direction.y != 0)
      self->direction = (struct Vector2){-1, 0};
    break;
  case RIGHT:
    if (self->direction.y != 0)
      self->direction = (struct Vector2){1, 0};
    break;
  case QUIT:
  case NONE:
  case RESTART:
    break;
  }
  snake_move_to(self, vec2_add(*snake_head(self), self->direction));
}
