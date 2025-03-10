
// Headers
#include "../include/parser.h"
#include "eval.h"

// TODO: free tokens after parsing
// TODO: remove exits and replace with NULL pointers
// TODO: freeing the expr and the result make recursion not working. Why would free a SchemeObject make the environment stop working as it should? Maybe it has to do with the pointers. Maybe creating a copy of the expression in eval makes it possible for me to free it in main?

int main() {
  char* input = malloc(256 * sizeof(char));
  Environment* global_environment = init_environment();

  while(1){
    printf("\n\nJVLISP >> "); fgets(input, 256, stdin);
    Token* tokens = tokenize(input);
    SchemeObject* expr = parser(&tokens);
    SchemeObject* result = eval(expr, &global_environment);
    print_scheme_object(result);

    // free_scheme_object(input);
    // free_scheme_object(result);
  }

  free_environment(global_environment);

  printf("\n\nThanks for using JVLISP! :)\n");
  return 0;
}
