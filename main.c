#include "parser.h"
#include "eval.h"

int main() {
  // const char* input = "(+ 2 3)"; // not working
  // const char* input = "+"; // not working
  // const char* input = "*"; // working!
  const char* input = "4";    // working!

  Token* tokens = tokenize(input);

  printf("TOKENIZING:\n");
  for (int i = 0; tokens[i].type != TOKEN_EOF; i++) {
    printf("Token Type %d: %s\n", tokens[i].type, tokens[i].value);
  }

  printf("\n\nPARSING:\n");
  SchemeObject* expr = parser(&tokens);
  print_scheme_object(expr); printf("\n");

  printf("\n\nEVALUATING:\n");
  SchemeObject* result = eval(expr, init_environment());
  print_scheme_object(result);

  printf("\n\nFinished executing.");
  return 0;
}