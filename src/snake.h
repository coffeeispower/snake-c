#ifndef SNAKE_H
#include "vectors.h"
#define SNAKE_H
struct Snake {
  struct Vector2* trail_data;
  unsigned long trail_size;
  unsigned long trail_capacity;
  unsigned long trail_max_size;
  struct Vector2 direction;
};

struct Vector2 snake_pop_tip(struct Snake* self);

void snake_move_to(struct Snake* self, struct Vector2 new_head_position);

struct Vector2 snake_pop_tip(struct Snake* self);

void snake_push(struct Snake* self, struct Vector2 new_head_position);

struct Vector2* snake_head(struct Snake* snake);

struct Vector2* snake_tip(struct Snake* snake);
void snake_render(struct Snake* self);
void snake_update(struct Snake* self);
#endif
