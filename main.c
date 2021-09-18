#include "chibicc.h"

// Reports an error and exit.
void error(char *fmt, ...)
{
  va_list ap;
  va_start(ap, fmt);
  vfprintf(stderr, fmt, ap);
  fprintf(stderr, "\n");
  exit(1);
}

int main(int argc, char **argv)
{
  if (argc != 2)
    error("%s: invalid number of arguments", argv[0]);

  // Tokenize and parse.
  Token *token = tokenize(argv[1]);
  Node **code = parse(token);
  generate(code);

  return 0;
}