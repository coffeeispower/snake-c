#ifndef FRUIT_H
#define FRUIT_H
#include "vectors.h"
#include "snake.h"
#include <stdbool.h>
struct Fruit {
  struct Vector2 position;
};
void fruit_render(struct Fruit* self);
bool fruit_snake_collision(struct Fruit* self, struct Snake *snake);
#endif
