
// Includes
#include "repl.c"

// TODO: free tokens after parsing
// TODO: remove exits and replace with NULL pointers
// TODO: freeing the expr and the result make recursion not working. Why would free a SchemeObject make the environment stop working as it should? Maybe it has to do with the pointers. Maybe creating a copy of the expression in eval makes it possible for me to free it in main?
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
