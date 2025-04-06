
// Includes
#include "repl.c"

//TODO: fix the SchemeExpression data structure that somehow has multiple field filled when it clearly shouldn't (example: symbol whenever it is a function)

int main(int argc, char* argv[]) {

  if (argc == 1) {
    REPL();
  }
  else {
    printf("Error: file as input not implemented yet.\n");
    exit(1);
  }

  return 0;
}
