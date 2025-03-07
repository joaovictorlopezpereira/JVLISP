#include "parser.h"
#include "eval.h"

int main() {
  const char* input = "(+ 2 3)";
  // const char* input = "3";
  // const char* input = "+";

  Token* tokens = tokenize(input);

  for (int i = 0; tokens[i].type != TOKEN_EOF; i++) {
    printf("Token Type %d: %s\n", tokens[i].type, tokens[i].value);
  }

  SchemeObject* expr = parser(&tokens);

  print_scheme_object(expr); printf("\n");

  SchemeObject* result = eval(expr, init_environment());

  print_scheme_object(result);

  printf("\n\nFinished executing.");
  return 0;
}