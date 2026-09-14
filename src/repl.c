// Includes
#include "parser.c"
#include "eval.c"
#include "tests.c"

// Functions Signatures
void REPL();

// Does the READ, EVAL, PRINT, LOOP cycle
void REPL() {
  size_t buffer_size = 1024;
  char* input = (char*)malloc(buffer_size * sizeof(char));

  // Initialize the environment with primitives procedures
  Environment* global_environment = init_environment();

  // Expand the environment to contain test procedures
  global_environment = add_variable(
    global_environment,
    "test-primitives",
    make_primitive(primitive_test_primitives, "test-primitives")
  );

  printf("Welcome to JVLISP! :)\n");

  while (1) {
    printf("JVLISP >> ");
    fflush(stdout);

    // Captures user input
    if (fgets(input, buffer_size, stdin) == NULL) {
      printf("\n");
      break;
    }

    // Removes line break
    input[strcspn(input, "\r\n")] = '\0';

    // Ignore empty inputs
    if (input[0] == '\0') {
      continue;
    }

    // User command to leave the program
    if (strcmp(input, "(bye)") == 0) {
      break;
    }

    // Tokenizing
    Token* tokens = tokenize(input);
    Token* original_tokens = tokens;

    // Parsing
    SchemeObject* expr = parser(&tokens);

    if (expr != NULL) {
      // Evaluating
      SchemeObject* result = eval(expr, &global_environment);

      // Printing
      if (result != NULL) {
        print_scheme_object(result);
        printf("\n");
      }
    }

    // Free tokens
    free_tokens(original_tokens);
  }

  // Final free
  free(input);
  free_environment(global_environment);

  printf("Thanks for using JVLISP! :)\n");
}