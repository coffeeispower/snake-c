#include "input.h"
#include <assert.h>
#include <ctype.h>
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#ifdef _WIN32
#include <windows.h>
#ifndef ENABLE_VIRTUAL_TERMINAL_INPUT
#define ENABLE_VIRTUAL_TERMINAL_INPUT 0x0200
#endif
#ifndef ENABLE_VIRTUAL_TERMINAL_PROCESSING
#define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004
#endif
#include <conio.h>
#else
#include <termios.h>
#endif
#include <unistd.h>

bool __raw_mode_enabled = false;
typedef struct {
  char c;
  bool has_input;
} ReadChar;
ReadChar _read_char() {
#ifdef _WIN32
  if(!_kbhit()) {
    return (ReadChar) {.has_input = false};
  }
  
  char c;
  assert(read(STDIN_FILENO, &c, 1));
  return (ReadChar){.has_input = true, .c = c};
#else
  char c;
  bool has_input = read(STDIN_FILENO, &c, 1) == 1;
  return (ReadChar){.has_input = has_input, .c = c};
#endif
}

enum SnakeInput read_input(void) {
  assert(__raw_mode_enabled && "Tried to read input with raw mode disabled");
  ReadChar c = _read_char();
  if (!c.has_input) {
    return NONE;
  }
  switch (c.c) {
  case 'q':
    return QUIT;
  case 'r':
    return RESTART;
  case 'c':
    return CREDITS;
  case '\e':
    c = _read_char();
    /// User pressed escape
    if (!c.has_input) {
      return QUIT;
    }
    /// Is arrow key
    if (c.c == '[') {
      c = _read_char();
      assert(c.has_input);
      switch (c.c) {
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
  }
  return NONE;
}
#ifdef _WIN32
DWORD original_stdout_console_mode;
DWORD original_stdin_console_mode;
void __at_exit_cleanup_raw_mode(void) {
  if (__raw_mode_enabled) {
    disable_raw_mode();
  }
}
void enable_raw_mode(void) {
  assert(!__raw_mode_enabled && "Double enable raw mode");
  __raw_mode_enabled = true;

  HANDLE consoleOut = GetStdHandle(STD_OUTPUT_HANDLE);
  HANDLE consoleIn = GetStdHandle(STD_INPUT_HANDLE);
  assert(GetConsoleMode(consoleOut, &original_stdout_console_mode));
  assert(GetConsoleMode(consoleIn, &original_stdin_console_mode));
  
  atexit(__at_exit_cleanup_raw_mode);
  // Disables all flags except virtual terminal input
  // Documentation: https://learn.microsoft.com/en-us/windows/console/setconsolemode
  assert(SetConsoleMode(consoleIn, ENABLE_VIRTUAL_TERMINAL_INPUT));

  
  assert(SetConsoleMode(consoleOut, ENABLE_PROCESSED_OUTPUT | ENABLE_VIRTUAL_TERMINAL_PROCESSING));
}
void disable_raw_mode(void) {
  assert(__raw_mode_enabled &&
         "Disabled raw mode when it was already disabled");
  HANDLE consoleOut = GetStdHandle(STD_OUTPUT_HANDLE);
  HANDLE consoleIn = GetStdHandle(STD_INPUT_HANDLE);
  assert(SetConsoleMode(consoleOut, original_stdout_console_mode));
  assert(SetConsoleMode(consoleIn, original_stdin_console_mode));
}
#else
struct termios __orig_termios;
void __at_exit_cleanup_raw_mode(void) {
  if (__raw_mode_enabled) {
    disable_raw_mode();
  }
}
void enable_raw_mode(void) {
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
void disable_raw_mode(void) {
  assert(__raw_mode_enabled &&
         "Disabled raw mode when it was already disabled");
  assert(tcsetattr(STDIN_FILENO, TCSAFLUSH, &__orig_termios) != -1);
}
#endif
