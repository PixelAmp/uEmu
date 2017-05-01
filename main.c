#include "argparse.h"
#include "ui.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {
  struct ui ui;
  ui_init(&ui, "uEmu>");

  for (;;) {
    size_t size;
    const char *line = ui_get_line(&ui, &size);

    if (strcmp(line, "quit\n") == 0 || strcmp(line, "exit\n") == 0)
      break;
    if (strcmp(line, "history\n") == 0) {
      ui_history(&ui);
      continue;
    }

    char *const *args = parse_args(line);
    if (*args == NULL)
      continue;

    if (!strcmp(*args, "cd")) {
      if (args[1]) {
        if (chdir(args[1]))
          printf("Unable to change directory.\n%m\n");
      } else {
        if (chdir(getenv("HOME")))
          printf("Unable to change directory.\n%m\n");
      }
      continue;
    }

    fflush(0);
    pid_t child = fork();

    if (child < 0) {
      printf("Error, unable to fork.\n%m\n");
      break;
    } else if (child == 0) {
      // We are the child.
      ui_free(&ui);

      execvp(*args, args);
      puts("Bad command or file name.");
      exit(EXIT_FAILURE);
    } else {
      // We are the parent.
      bool wait_for_child;
      for (char *const *p = args; *p; ++p)
        wait_for_child = strcmp("&", *p);

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmaybe-uninitialized"
      if (wait_for_child)
        while (child != wait(NULL))
          ;
#pragma GCC diagnostic pop
    }
  }

  ui_free(&ui);

  return 0;
}
