#include "parser.h"

// TODO: Make parser work with nested lists. For example, "(+ (* 1 2) 3)";

int main() {
  const char* input = "(+ 1 2 3)";

  Token* tokens = tokenize(input);

  for (int i = 0; tokens[i].type != TOKEN_EOF; i++) {
    printf("Token Type %d: %s\n", tokens[i].type, tokens[i].value);
  }

  SchemeObject* expr = parser(&tokens);

  print_scheme_object(expr);

  printf("\n\nFinished executing.");
  return 0;
}