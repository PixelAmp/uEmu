#include "argparse.h"
#include "ui.h"
#include <assert.h>
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

  // Main loop.
  for (;;) {
    // Get a line of input from the user.
    size_t size;
    const char *line = ui_get_line(&ui, &size);

    // Quit?
    if (strcmp(line, "quit\n") == 0 || strcmp(line, "exit\n") == 0)
      break;
    // User requests history? print history and loop.
    if (strcmp(line, "history\n") == 0) {
      ui_history(&ui);
      continue;
    }

    // Parse the string into command+arguments.
    char *const *args = parse_args(line);
    if (*args == NULL)
      continue;

    // cd is a builtin, not an application. We must handle directory changes.
    if (!strcmp(*args, "cd")) {
      if (args[1]) {
        // If the user has given us a directory to change to, cd to that.
        if (chdir(args[1]))
          printf("Unable to change directory.\n%m\n");
      } else {
        // Otherwise, change to the user's home directory.
        if (chdir(getenv("HOME")))
          printf("Unable to change directory.\n%m\n");
      }
      continue;
    }

    // Make sure we flush all buffers before we fork, otherwise both processes
    // may double the output.
    fflush(0);

    // Fork!
    pid_t child = fork();

    if (child < 0) {
      printf("Error, unable to fork.\n%m\n");
      break;
    } else if (child == 0) {
      // We are the child. Free all resources used by us...
      ui_free(&ui);

      // And execute the user's command.
      execvp(*args, args);
      puts("Bad command or file name.");
      exit(EXIT_FAILURE);
    } else {
      // We are the parent.
      bool wait_for_child;
      assert(args);
      for (char *const *p = args; *p; ++p)
        wait_for_child = strcmp("&", *p);

// wait_for_child is uninitialized if and only if args is NULL. args is checked
// for NULL immediately after being retrieved.
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmaybe-uninitialized"
      // Wait for the child if we must wait for the child.
      if (wait_for_child)
        while (child != wait(NULL))
          ;
#pragma GCC diagnostic pop
    }
  }

  ui_free(&ui);

  return 0;
}
