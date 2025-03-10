#include "parser.h"
#include "eval.h"

// TODO: free tokens after parsing
// TODO: free environment
// TODO: free Scheme expressions
// TODO: remove exits and replace with NULL pointers

int main() {
  char* input = malloc(256 * sizeof(char));
  Environment* global_environment = init_environment();

  while(1){                                                 // LOOP
    printf("\n\nJVLISP >> "); fgets(input, 256, stdin);     // READ
    Token* tokens = tokenize(input);                        // READ
    SchemeObject* expr = parser(&tokens);                   // READ
    SchemeObject* result = eval(expr, &global_environment); // EVAL
    print_scheme_object(result);                            // PRINT
  }                                                         // LOOP

  return 0;
}
