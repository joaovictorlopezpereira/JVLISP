#include "parser.h"
#include "eval.h"

int main() {
  const char* input = "(define x 10)"; // not working
  // const char* input = "(+ 5 (- 4 3) 1)"; //working
  // const char* input = "(- 5 2 1)"; // working
  // const char* input = "(+ 2 3 7)"; // working
  // const char* input = "+"; // working
  // const char* input = "-"; // working
  // const char* input = "*"; // working!
  // const char* input = "4";    // working!


  printf("INPUT EXPRESSION:\n");
  printf("%s", input);

  printf("\n\nTOKENIZING:\n");
  Token* tokens = tokenize(input);
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