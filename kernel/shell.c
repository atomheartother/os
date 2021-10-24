#include "shell.h"
#include "screen.h"
#include "kmem.h"

#define BUFFER_SIZE 256

static char buffer[BUFFER_SIZE];
static unsigned int bufferIdx;
static unsigned int messageEnd;

static void initShellIdx() {
  bufferIdx = 2;
  messageEnd = 2;
}

void shellInit() {
  // The shel buffer includes the prompt so displaying the shell is faster
  buffer[0] = '>';
  buffer[1] = ' ';
  initShellIdx();
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
  if (bufferIdx <= 2) {
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
  buffer[messageEnd] = 0;
  printMessageOverLine(buffer);
}

void shellRun() {
  newline();
  printMessage("Executed: ");
  printMessage(buffer + 2);
  newline();
  shellInit();
}
