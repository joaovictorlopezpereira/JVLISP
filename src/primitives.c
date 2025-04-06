#pragma once

// Includes
#include <stdio.h>
#include "object.c"


// Functions Signatures
SchemeObject* (*get_primitive_function(const char* symbol))(SchemeObject* args);
SchemeObject* primitive_add(SchemeObject* args);
SchemeObject* primitive_sub(SchemeObject* args);
SchemeObject* primitive_mul(SchemeObject* args);
SchemeObject* primitive_div(SchemeObject* args);
SchemeObject* primitive_equal_sign(SchemeObject* args);
SchemeObject* primitive_cons(SchemeObject* args);
SchemeObject* primitive_car(SchemeObject* args);
SchemeObject* primitive_cdr(SchemeObject* args);
SchemeObject* primitive_display(SchemeObject* args);

// Array of symbols and their respective functions
PrimitiveMapping primitives[] = {
  {"+", primitive_add},
  {"-", primitive_sub},
  {"*", primitive_mul},
  {"/", primitive_div},
  {"=", primitive_equal_sign},
  {"cons", primitive_cons},
  {"car", primitive_car},
  {"cdr", primitive_cdr},
  {"display", primitive_display},
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

// Scheme "+" procedure
SchemeObject* primitive_add(SchemeObject* args) {
  if (args == NULL || args->type != SCHEME_PAIR) {
    printf("Error: + requires at least one argument.");
    return NULL;
  }

  double acc = 0; // Accumulator

  while (args != NULL && args->type == SCHEME_PAIR) {
    if (args->value.pair.car->type != SCHEME_NUMBER) {
      printf("Error: + only operates on numbers.");
      return NULL;
    }
    acc += args->value.pair.car->value.number; // Accumulates the sum
    args = args->value.pair.cdr;
  }

  return make_number(acc);
}

// Scheme "-" procedure
SchemeObject* primitive_sub(SchemeObject* args) {
  if (args == NULL || args->type != SCHEME_PAIR) {
    printf("Error: - requires at least one argument.");
    return NULL;
  }

  if (args->value.pair.car->type != SCHEME_NUMBER) {
    printf("Error: - only operates on numbers.");
    return NULL;
  }

  double acc = args->value.pair.car->value.number; // Accumulator
  args = args->value.pair.cdr;

  while (args != NULL && args->type == SCHEME_PAIR) {
    if (args->value.pair.car->type != SCHEME_NUMBER) {
      printf("Error: - only operates on numbers.");
      return NULL;
    }
    acc -= args->value.pair.car->value.number; // Accumulates the sub
    args = args->value.pair.cdr;
  }

  return make_number(acc);
}

// Scheme "*" procedure
SchemeObject* primitive_mul(SchemeObject* args) {
  if (args == NULL || args->type != SCHEME_PAIR) {
    printf("Error: * requires at least one argument.");
    return NULL;
  }

  double acc = 1; // Accumulator

  while (args != NULL && args->type == SCHEME_PAIR) {
    if (args->value.pair.car->type != SCHEME_NUMBER) {
      printf("Error: * only operates on numbers.");
      return NULL;
    }
    acc *= args->value.pair.car->value.number; // Accumulates the product
    args = args->value.pair.cdr;
  }

  return make_number(acc);
}

// Scheme "/" procedure
SchemeObject* primitive_div(SchemeObject* args) {
  if (args == NULL || args->type != SCHEME_PAIR) {
    printf("Error: / requires at least one argument.");
    return NULL;
  }

  if (args->value.pair.car->type != SCHEME_NUMBER) {
    printf("Error: / only operates on numbers.");
    return NULL;
  }

  double acc = args->value.pair.car->value.number; // Accumulator
  args = args->value.pair.cdr;

  while (args != NULL && args->type == SCHEME_PAIR) {
    if (args->value.pair.car->type != SCHEME_NUMBER) {
      printf("Error: / only operates on numbers.");
      return NULL;
    }
    acc /= args->value.pair.car->value.number; // Accumulates the division
    args = args->value.pair.cdr;
  }

  return make_number(acc);
}

// Scheme "=" procedure
SchemeObject* primitive_equal_sign(SchemeObject* args) {
  if (args == NULL || args->type != SCHEME_PAIR) {
    printf("Error: = requires at least one argument.");
    return NULL;
  }

  if (args->value.pair.car->type != SCHEME_NUMBER) {
    printf("Error: = only operates on numbers.");
    return NULL;
  }

  double first = args->value.pair.car->value.number;
  args = args->value.pair.cdr;

  while (args != NULL && args->type == SCHEME_PAIR) {
    if (args->value.pair.car->type != SCHEME_NUMBER) {
      printf("Error: = only operates on numbers.");
      return NULL;
    }
    if (args->value.pair.car->value.number != first) {
      return make_boolean("#f");
    }
    args = args->value.pair.cdr;
  }

  return make_boolean("#t");
}

// Scheme "cons" procedure
SchemeObject* primitive_cons(SchemeObject* args) {
  if (args == NULL || args->type != SCHEME_PAIR) {
    printf("Error: cons requires at least 2 arguments.");
    return NULL;
  }

  if (args->value.pair.cdr == NULL || args->value.pair.cdr->type != SCHEME_PAIR) {
    printf("Error: cons requires a valid second argument.");
    return NULL;
  }

  if (args->value.pair.cdr->value.pair.cdr->type == SCHEME_PAIR) {
    printf("Error: cons does not require more than 2 arguments.");
    return NULL;
  }

  return make_pair(args->value.pair.car, args->value.pair.cdr->value.pair.car);
}

// Scheme "car" procedure
SchemeObject* primitive_car(SchemeObject* args) {
  if (args == NULL || args->type != SCHEME_PAIR) {
    printf("Error: car requires at least 1 arguments.");
    return NULL;
  }

  if (args->value.pair.car->type != SCHEME_PAIR) {
    printf("Error: car only operates on pairs.");
    return NULL;
  }

  return args->value.pair.car->value.pair.car;
}

// Scheme "cdr" procedure
SchemeObject* primitive_cdr(SchemeObject* args) {
  if (args == NULL || args->type != SCHEME_PAIR) {
    printf("Error: cdr requires at least 1 arguments.");
    return NULL;
  }

  if (args->value.pair.car->type != SCHEME_PAIR) {
    printf("Error: cdr only operates on pairs.");
    return NULL;
  }

  return args->value.pair.car->value.pair.cdr;
}

// Scheme "display" procedure
SchemeObject* primitive_display(SchemeObject* args) {
  while (args != NULL && args->type == SCHEME_PAIR) {
    // If it's a string, just print it without the parenthesis
    if (args->value.pair.car->type == SCHEME_STRING) {
      printf("%s", args->value.pair.car->value.string);
    }
    else {
      print_scheme_object(args->value.pair.car);
    }
    printf(" ");
    args = args->value.pair.cdr;
  }
  printf("\n");
  return NULL;
}
