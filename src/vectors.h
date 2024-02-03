#ifndef VECTORS_H
#define VECTORS_H
struct Vector2 {
  long x, y;
};

void vec2_debug(struct Vector2 v);
struct Vector2 vec2_add(struct Vector2 v1, struct Vector2 v2);
#endif
