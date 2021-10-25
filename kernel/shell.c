#include "shell.h"
#include "screen.h"
#include "kmem.h"

#define BUFFER_SIZE 256

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
  char refreshDisplay = bufferIdx < messageEnd;
  if (refreshDisplay) {
    // Offset the whole buffer, we're inserting
    memmove(buffer+bufferIdx+1, buffer+bufferIdx, messageEnd - bufferIdx);
  }
  buffer[bufferIdx++] = c;
  messageEnd += 1;
  if (refreshDisplay) {
    shellDisplay();
  } else {
    printChar(c);
  }
}

void shellBackspace() {
  if (bufferIdx <= 0) {
    return;
  }
  const char refreshDisplay = bufferIdx < messageEnd;
  if (refreshDisplay) {
    memmove(buffer+bufferIdx, buffer+bufferIdx+1, messageEnd - bufferIdx);
  }
  buffer[--messageEnd] = 0;
  bufferIdx -= 1;
  if (refreshDisplay) {
    shellDisplay();
  } else {
    backspace();
  }
}

// Completely refresh the shell display
void shellDisplay() {
  setVideoMode('B');
  printMessageOverLine("> ");
  buffer[messageEnd] = 0;
  setVideoMode(WHITE_ON_BLACK);
  printMessage(buffer);
}

void shellRun() {
  newline();
  if (messageEnd > 0) {
    printMessage("Executed: ");
    printMessage(buffer);
    newline();
  }
  shellInit();
}
