#include <stdio.h>
#include <string.h>

#include "ui.h"

int main(int argc, char **argv) {
  struct ui ui;
  ui_init(&ui, "uEmu>");

  for (;;) {
    size_t size;
    const char *line = ui_get_line(&ui, &size);

    if (strcmp(line, "quit\n") == 0)
      break;

    puts(line);
  }

  ui_free(&ui);

  return 0;
}