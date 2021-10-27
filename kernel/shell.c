#include "shell.h"
#include "kstring.h"
#include "screen.h"
#include "kmem.h"

#define BUFFER_SIZE 32

static char buffer[BUFFER_SIZE];
static unsigned int bufferIdx;
static unsigned int messageEnd;

void shellInit() {
  bufferIdx = 0;
  messageEnd = 0;
  shellDisplay();
}

void shellInput(const char c) {
  if (bufferIdx >= BUFFER_SIZE) {
    return;
  }
  buffer[bufferIdx++] = c;
  messageEnd += 1;
  printChar(c);
}

void shellBackspace() {
  if (bufferIdx <= 0) {
    return;
  }
  buffer[--messageEnd] = 0;
  bufferIdx -= 1;
  backspace();
}

// Completely refresh the shell display
void shellDisplay() {
  printMessageOverLine("> ", FG_BRIGHT | FG_Red);
  if (messageEnd > 0) {
    buffer[messageEnd] = 0;
    printMessage(buffer);
  }
}

void shellRun() {
  newline();
  if (messageEnd > 0) {
    printMessageMode("Executed: ", FG_BRIGHT | FG_Magenta);
    buffer[messageEnd] = 0;
    printMessage(buffer);
    newline();
  }
  shellInit();
}
