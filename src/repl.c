
// Includes
#include "parser.c"
#include "eval.c"


// Functions Signatures
void REPL();


void REPL() {
  char* input = malloc(256 * sizeof(char));
  Environment* global_environment = init_environment();
  Token* tokens;

  printf("\nWelcome to JVLISP! :)");

  while(1) {
    // Gets the user input and checks if it's a "(bye)"
    printf("\n\nJVLISP >> ");
    fgets(input, 256, stdin);
    input[strcspn(input, "\n")] = 0;
    if (strcmp(input, "(bye)") == 0) break;

    // Tokenize the input
    tokens = tokenize(input);

    // // Remove syntactic sugar
    // desugar(&tokens);

    // Parse, evaluate and print the result
    SchemeObject* expr = parser(&tokens);
    SchemeObject* result = eval(expr, &global_environment);
    print_scheme_object(result);
  }

  // Frees some allocated memory
  free_tokens(tokens);
  free(input);
  free_environment(global_environment);

  printf("\nThanks for using JVLISP! :)\n");
}