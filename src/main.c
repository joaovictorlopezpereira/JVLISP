
// Includes
#include "repl.c"

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
