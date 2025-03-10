
// Headers
#include "../include/parser.h"
#include "eval.h"

// TODO: free tokens after parsing
// TODO: remove exits and replace with NULL pointers
// TODO: (define f (lambda (n) (if (= n 1) 1 (* n (f (- n 1)))))) not working
// TODO: (define f (lambda (n) (if (= n 1) 1 (if (= n 2) 1 (+ (f (- n 1)) (f (- n 2))))))) not working

int main() {
  char* input = malloc(256 * sizeof(char));
  Environment* global_environment = init_environment();

  while(1){
    printf("\n\nJVLISP >> "); fgets(input, 256, stdin);
    Token* tokens = tokenize(input);
    SchemeObject* expr = parser(&tokens);
    SchemeObject* result = eval(expr, &global_environment);
    print_scheme_object(result);

    free_scheme_object(expr);
    free_scheme_object(result);
  }

  free_environment(global_environment);

  printf("\n\nThanks for using JVLISP! :)\n");
  return 0;
}
