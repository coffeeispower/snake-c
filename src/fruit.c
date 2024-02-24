#include "fruit.h"
#include "screen.h"
#include <stdio.h>

void fruit_render(struct Fruit* self) {
  move_cursor(self->position.x*2, self->position.y);
  #ifdef _WIN32
  set_bg_color_256(196);
  printf("  ");
  #else
  printf("🍎");
  #endif
}
bool fruit_snake_collision(struct Fruit* self, struct Snake *snake){
  return snake_is_head(snake, self->position);
}
