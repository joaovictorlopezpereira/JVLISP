
// Includes
#include "repl.c"

int main(int argc, char* argv[]) {
  if (argc == 1) {
    REPL();
  }
  else if (argc == 2) {
    run_file(argv[1]);
  }
  else {
    printf("Usage: %s [file.jlp]\n", argv[0]);
    exit(1);
  }

  return 0;
}
