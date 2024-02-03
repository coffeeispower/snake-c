#include "input.h"
#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include <assert.h>
#include <stdbool.h>

bool __raw_mode_enabled = false;
char _read_char() {
  char c;
  assert(read(STDIN_FILENO, &c, 1) != -1 && "Failed to read character from stdin");
  return c;
}

enum SnakeInput read_input() {
  assert(__raw_mode_enabled && "Tried to read input with raw mode disabled");
  switch(_read_char()) {
    case 'q':
      return QUIT;
    case '\e':
      assert(_read_char() == '[' && "Expected '[' after escape char'");
      switch(_read_char()) {
        case 'A':
          return UP;
        case 'B':
          return DOWN;
        case 'C':
          return RIGHT;
        case 'D':
          return LEFT;
      }
  }
  return NONE;
}

struct termios __orig_termios;
void __at_exit_cleanup_raw_mode() {
  if(__raw_mode_enabled) {
    disable_raw_mode();
  }
}
void enable_raw_mode() {
  assert(!__raw_mode_enabled && "Double enable raw mode");
  __raw_mode_enabled = true;
  assert(tcgetattr(STDIN_FILENO, &__orig_termios) != -1);
  atexit(__at_exit_cleanup_raw_mode);
  struct termios raw = __orig_termios;
  raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
  raw.c_oflag &= ~(OPOST);
  raw.c_cflag |= (CS8);
  raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
  raw.c_cc[VMIN] = 0;
  raw.c_cc[VTIME] = 1;
  assert(tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) != -1);
}
void disable_raw_mode() {  
  assert(__raw_mode_enabled && "Disabled raw mode when it was already disabled");
  assert(tcsetattr(STDIN_FILENO, TCSAFLUSH, &__orig_termios) != -1);
}
