// Includes
#include "parser.c"
#include "eval.c"
#include "tests.c"

// Functions Signatures
void REPL();
static char* read_file(const char* filename);
void run_file(const char* filename);

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

    if (tokens == NULL) {
      continue;
    }

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

// Reads a file and return a dynamically allocated string
static char* read_file(const char* filename) {
  FILE* file = fopen(filename, "rb");
  if (file == NULL) {
    printf("Error: could not open file '%s'.\n", filename);
    return NULL;
  }

  fseek(file, 0, SEEK_END);
  long length = ftell(file);
  rewind(file);

  char* buffer = (char*)malloc(length + 1);
  if (buffer == NULL) {
    printf("Error: out of memory reading '%s'.\n", filename);
    fclose(file);
    return NULL;
  }

  size_t bytes_read = fread(buffer, 1, length, file);
  buffer[bytes_read] = '\0';
  fclose(file);
  return buffer;
}

// Execute expressions in a .jlp file sequentially
void run_file(const char* filename) {
  const char* ext = strrchr(filename, '.');
  if (ext == NULL || strcmp(ext, ".jlp") != 0) {
    printf("Error: expected a .jlp file.\n");
    exit(1);
  }

  char* source = read_file(filename);
  if (source == NULL) {
    exit(1);
  }

  Token* tokens = tokenize(source);
  free(source);
  if (tokens == NULL) {
    exit(1);
  }

  Token* original_tokens = tokens;
  Environment* global_environment = init_environment();

  while (tokens->type != TOKEN_EOF) {
    SchemeObject* expr = parser(&tokens);
    if (expr == NULL) {
      free_tokens(original_tokens);
      free_environment(global_environment);
      exit(1);
    }

    SchemeObject* result = eval(expr, &global_environment);
    if (result == NULL) {
      free_tokens(original_tokens);
      free_environment(global_environment);
      exit(1);
    }
  }

  free_tokens(original_tokens);
  free_environment(global_environment);
}