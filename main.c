#include "parser.h"
#include "eval.h"

// TODO: free tokens after parsing

int main() {
  // const char* input = "(define x 10)";   // working
  // const char* input = "(/ 5 2 2)";       // working
  // const char* input = "(* 1 3 7)";       // working
  // const char* input = "(+ 5 (- 4 3) 1)"; // working
  // const char* input = "(- 5 2 1)";       // working
  // const char* input = "(+ 2 3 7)";       // working
  // const char* input = "+";               // working
  // const char* input = "-";               // working
  // const char* input = "*";               // working
  // const char* input = "4";               // working

  char* input = malloc(256 * sizeof(char));
  Environment* global_environment = init_environment();

  while(1){                                                 // LOOP
    printf("\n\nJVLISP >> "); fgets(input, 256, stdin);     // READ
    Token* tokens = tokenize(input);                        // READ
    SchemeObject* expr = parser(&tokens);                   // READ
    SchemeObject* result = eval(expr, &global_environment); // EVAL
    print_scheme_object(result);                            // PRINT
    // free_tokens(tokens);
    free_object(expr);
    free_object(result);
  }                                                         // LOOP

  return 0;
}
