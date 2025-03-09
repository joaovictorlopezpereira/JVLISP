#pragma once

#include "object.h"
#include <stdio.h>

// Functions Signatures
SchemeObject* primitive_sub(SchemeObject* args);
SchemeObject* primitive_add(SchemeObject* args);
SchemeObject* (*get_primitive_function(const char* symbol))(SchemeObject* args);


// Scheme "-" procedure
SchemeObject* primitive_sub(SchemeObject* args) {
  if (args == NULL || args->type != SCHEME_PAIR) {
    printf("Error: - requires at least one argument.\n");
    exit(1);
  }

  if (args->value.pair.car->type != SCHEME_NUMBER) {
    printf("Error: - only operates on numbers.\n");
    exit(1);
  }

  double acc = args->value.pair.car->value.number; // Accumulator
  args = args->value.pair.cdr;

  while (args != NULL && args->type == SCHEME_PAIR) {
    if (args->value.pair.car->type != SCHEME_NUMBER) {
      printf("Error: - only operates on numbers.\n");
      exit(1);
    }
    acc -= args->value.pair.car->value.number; // Accumulates the sub
    args = args->value.pair.cdr;
  }

  return make_number(acc);
}


// Scheme "+" procedure
SchemeObject* primitive_add(SchemeObject* args) {
  if (args == NULL || args->type != SCHEME_PAIR) {
    printf("Error: - requires at least one argument.\n");
    exit(1);
  }

  double acc = 0; // Accumulator

  while (args != NULL && args->type == SCHEME_PAIR) {
    if (args->value.pair.car->type != SCHEME_NUMBER) {
      printf("Error: + only operates on numbers.\n");
      exit(1);
    }
    acc += args->value.pair.car->value.number; // Accumulates the sum
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

