#include "snake.h"
#include "screen.h"
#include "vectors.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

void snake_move_to(struct Snake *self, struct Vector2 new_head_position) {
  if (self->trail_data == 0 || self->trail_capacity == 0) {
    self->trail_capacity = 10;
    self->trail_size = 1;
    self->trail_data = calloc(self->trail_capacity, sizeof(struct Vector2));
    *snake_head(self) = new_head_position;
    return;
  }
  snake_push(self, new_head_position);
  if (self->trail_size >= self->trail_max_size) {
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
  if (self->trail_capacity == self->trail_size) {
    self->trail_capacity *= 2;
    self->trail_data = realloc(self->trail_data, self->trail_capacity);
  }
  self->trail_data[self->trail_size] = new_head_position;
  self->trail_size += 1;
}

struct Vector2 *snake_head(struct Snake *snake) {
  return &snake->trail_data[snake->trail_size - 1];
}
struct Vector2 *snake_tip(struct Snake *snake) {
  return &snake->trail_data[0];
}

void snake_render(struct Snake *self) {
  for (unsigned long i = 0; i < self->trail_size; i++) {
    struct Vector2 pos = self->trail_data[i];
    move_cursor(pos.x, pos.y);
    printf("#");
  }
  fflush(stdout);
}
void snake_update(struct Snake *self) {
  snake_move_to(self, vec2_add(*snake_head(self), self->direction));
}
