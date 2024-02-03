#include "fruit.h"
#include "screen.h"
#include <stdio.h>

void fruit_render(struct Fruit* self) {
  move_cursor(self->position.x*2, self->position.y);
  printf("🍎");
}
bool fruit_snake_collision(struct Fruit* self, struct Snake *snake){
  return snake_is_head(snake, self->position);
}
