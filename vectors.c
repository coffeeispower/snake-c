#include "vectors.h"
#include <stdio.h>
struct Vector2 vec2_add(struct Vector2 v1, struct Vector2 v2) {
  return (struct Vector2) {
    .x = v1.x + v2.x,
    .y = v1.y + v2.y
  };
}
void vec2_debug(struct Vector2 v) {
  printf("Vector2 {\n  .x = %lu,\n  .y = %lu\n}", v.x, v.y);
}
