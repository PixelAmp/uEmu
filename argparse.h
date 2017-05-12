#pragma once

/*##################################################################################
     PURPOSE: Tokenizes a string into command name and arguments.
  PARAMETERS: A null terminated string, corresponding to a line of terminal
              input
      RETURN: An array of null terminated strings, one string per argument,
              beginning with the command. The user is not required to free this
              memory.
       NOTES: NOT threadsafe.
###################################################################################*/
char *const *parse_args(const char *line);
