#pragma once

#include "object.h"

// Scheme "-" procedure
SchemeObject* primitive_sub(SchemeObject* args) {
  double acc = args->value.pair.car->value.number;
  args = args->value.pair.cdr;

  while (args->type == SCHEME_PAIR) {
    if (args->value.pair.car->type != SCHEME_NUMBER) {
      printf("Error: - only subtracts numbers.\n");
      exit(1);
    }
    acc -= args->value.pair.car->value.number;
    args = args->value.pair.cdr;
  }
  return make_number(acc);
}


// Scheme "+" procedure
SchemeObject* primitive_add(SchemeObject* args) {
  double acc = 0;

  while (args->type == SCHEME_PAIR) {
      if (args->value.pair.car->type != SCHEME_NUMBER) {
          printf("Error: + only adds numbers.\n");
          exit(1);
      }
      acc += args->value.pair.car->value.number;
      args = args->value.pair.cdr;
  }

  return make_number(acc);
}

PrimitiveMapping primitives[] = {
  {"+", primitive_add},
  {"-", primitive_sub},
};

// Gets a primitive function given its "name"
SchemeObject* (*get_primitive_function(const char* symbol))(SchemeObject* args) {
  for (int i = 0; i < sizeof(primitives) / sizeof(PrimitiveMapping); i++) {
    if (strcmp(symbol, primitives[i].symbol) == 0) {
      return primitives[i].function;
    }
  }
  return NULL;
}

// Checks if a string is actually a primitive
int is_primitive(char str[10]) {
  if(strcmp(str, "+") == 0 ||
     strcmp(str, "-") == 0){
      return 1;
  }
  return 0;
}