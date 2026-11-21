#pragma once

// Includes
#include <stdio.h>
#include "object.c"

// Functions Signatures
static int check_unary_args(SchemeObject* args, const char* name);
static int check_binary_args(SchemeObject* args, const char* name);
SchemeObject* primitive_add(SchemeObject* args);
SchemeObject* primitive_sub(SchemeObject* args);
SchemeObject* primitive_mul(SchemeObject* args);
SchemeObject* primitive_div(SchemeObject* args);
SchemeObject* primitive_equal_sign(SchemeObject* args);
SchemeObject* primitive_equal(SchemeObject* args);
SchemeObject* primitive_cons(SchemeObject* args);
SchemeObject* primitive_list(SchemeObject* args);
SchemeObject* primitive_car(SchemeObject* args);
SchemeObject* primitive_cdr(SchemeObject* args);
SchemeObject* primitive_is_null(SchemeObject* args);
SchemeObject* primitive_display(SchemeObject* args);
SchemeObject* primitive_is_pair(SchemeObject* args);
SchemeObject* primitive_is_number(SchemeObject* args);
SchemeObject* primitive_is_symbol(SchemeObject* args);
SchemeObject* primitive_is_boolean(SchemeObject* args);
SchemeObject* primitive_lt(SchemeObject* args);
SchemeObject* primitive_gt(SchemeObject* args);
SchemeObject* primitive_lte(SchemeObject* args);
SchemeObject* primitive_gte(SchemeObject* args);

// Array of symbols and their respective functions
PrimitiveMapping primitives[] = {
  {"+", primitive_add},
  {"-", primitive_sub},
  {"*", primitive_mul},
  {"/", primitive_div},
  {"=", primitive_equal_sign},
  {"equal?", primitive_equal},
  {"cons", primitive_cons},
  {"list", primitive_list},
  {"car", primitive_car},
  {"cdr", primitive_cdr},
  {"null?", primitive_is_null},
  {"display", primitive_display},
  {"pair?", primitive_is_pair},
  {"number?", primitive_is_number},
  {"symbol?", primitive_is_symbol},
  {"boolean?", primitive_is_boolean},
  {"<", primitive_lt},
  {">", primitive_gt},
  {"<=", primitive_lte},
  {">=", primitive_gte}
};

// Helper to validate unary primitives
static int check_unary_args(SchemeObject* args, const char* name) {
  if (args == NULL || args->type != SCHEME_PAIR) {
    printf("Error: %s requires 1 argument.\n", name);
    return 0;
  }
  if (args->value.pair.cdr != NULL &&
      args->value.pair.cdr->type != SCHEME_NIL) {
    printf("Error: %s takes exactly 1 argument.\n", name);
    return 0;
  }
  return 1;
}

// Helper to validate binary primitives
static int check_binary_args(SchemeObject* args, const char* name) {
  if (args == NULL || args->type != SCHEME_PAIR ||
      args->value.pair.cdr == NULL || args->value.pair.cdr->type != SCHEME_PAIR) {
    printf("Error: %s requires 2 arguments.\n", name);
    return 0;
  }
  if (args->value.pair.cdr->value.pair.cdr != NULL &&
      args->value.pair.cdr->value.pair.cdr->type != SCHEME_NIL) {
    printf("Error: %s takes exactly 2 arguments.\n", name);
    return 0;
  }
  return 1;
}

// Scheme "+" procedure
SchemeObject* primitive_add(SchemeObject* args) {
  if (args == NULL || args->type != SCHEME_PAIR) {
    printf("Error: + requires at least one argument.\n");
    return NULL;
  }

  double acc = 0; // Accumulator

  while (args != NULL && args->type == SCHEME_PAIR) {
    if (args->value.pair.car->type != SCHEME_NUMBER) {
      printf("Error: + only operates on numbers.\n");
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
    printf("Error: - requires at least one argument.\n");
    return NULL;
  }

  if (args->value.pair.car->type != SCHEME_NUMBER) {
    printf("Error: - only operates on numbers.\n");
    return NULL;
  }

  double acc = args->value.pair.car->value.number; // Accumulator
  args = args->value.pair.cdr;

  // Treats the case where there's only one input to the - operator
  if (args == NULL || args->type == SCHEME_NIL) {
    return make_number(-acc);
  }

  while (args != NULL && args->type == SCHEME_PAIR) {
    if (args->value.pair.car->type != SCHEME_NUMBER) {
      printf("Error: - only operates on numbers.\n");
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
    printf("Error: * requires at least one argument.\n");
    return NULL;
  }

  double acc = 1; // Accumulator

  while (args != NULL && args->type == SCHEME_PAIR) {
    if (args->value.pair.car->type != SCHEME_NUMBER) {
      printf("Error: * only operates on numbers.\n");
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
    printf("Error: / requires at least one argument.\n");
    return NULL;
  }

  if (args->value.pair.car->type != SCHEME_NUMBER) {
    printf("Error: / only operates on numbers.\n");
    return NULL;
  }

  double acc = args->value.pair.car->value.number; // Accumulator
  args = args->value.pair.cdr;

  // Treats the case where there's only one input to the / operator
  if (args == NULL || args->type == SCHEME_NIL) {
    if (acc == 0.0) {
      printf("Error: division by zero.\n");
      return NULL;
    }
    return make_number(1.0 / acc);
  }

  while (args != NULL && args->type == SCHEME_PAIR) {
    if (args->value.pair.car->type != SCHEME_NUMBER) {
      printf("Error: / only operates on numbers.\n");
      return NULL;
    }
    double divisor = args->value.pair.car->value.number;
    if (divisor == 0.0) {
      printf("Error: division by zero.\n");
      return NULL;
    }
    acc /= divisor; // Accumulates the division
    args = args->value.pair.cdr;
  }

  return make_number(acc);
}

// Scheme "=" procedure
SchemeObject* primitive_equal_sign(SchemeObject* args) {
  if (args == NULL || args->type != SCHEME_PAIR) {
    printf("Error: = requires at least one argument.\n");
    return NULL;
  }

  if (args->value.pair.car->type != SCHEME_NUMBER) {
    printf("Error: = only operates on numbers.\n");
    return NULL;
  }

  double first = args->value.pair.car->value.number;
  args = args->value.pair.cdr;

  while (args != NULL && args->type == SCHEME_PAIR) {
    if (args->value.pair.car->type != SCHEME_NUMBER) {
      printf("Error: = only operates on numbers.\n");
      return NULL;
    }
    if (args->value.pair.car->value.number != first) {
      return make_boolean("#f");
    }
    args = args->value.pair.cdr;
  }

  return make_boolean("#t");
}

// Scheme "equal?" procedure
SchemeObject* primitive_equal(SchemeObject* args) {
  if (!check_binary_args(args, "equal?")) {
    return NULL;
  }

  SchemeObject* first = args->value.pair.car;
  SchemeObject* second = args->value.pair.cdr->value.pair.car;

  return make_boolean(are_objects_equal(first, second) ? "#t" : "#f");
}

// Scheme "cons" procedure
SchemeObject* primitive_cons(SchemeObject* args) {
  if (args == NULL || args->type != SCHEME_PAIR) {
    printf("Error: cons requires at least 2 arguments.\n");
    return NULL;
  }

  if (args->value.pair.cdr == NULL || args->value.pair.cdr->type != SCHEME_PAIR) {
    printf("Error: cons requires a valid second argument.\n");
    return NULL;
  }

  SchemeObject* rest = args->value.pair.cdr->value.pair.cdr;
  if (rest != NULL && rest->type == SCHEME_PAIR) {
    printf("Error: cons takes exactly 2 arguments.\n");
    return NULL;
  }

  return make_pair(args->value.pair.car, args->value.pair.cdr->value.pair.car);
}

// Scheme's list procedure
SchemeObject* primitive_list(SchemeObject* args) {
  if (args == NULL) {
    return make_nil();
  }
  return args;
}

// Scheme "car" procedure
SchemeObject* primitive_car(SchemeObject* args) {
  if (!check_unary_args(args, "car")) {
    return NULL;
  }

  if (args->value.pair.car->type != SCHEME_PAIR) {
    printf("Error: car only operates on pairs.\n");
    return NULL;
  }

  return args->value.pair.car->value.pair.car;
}

// Scheme "cdr" procedure
SchemeObject* primitive_cdr(SchemeObject* args) {
  if (!check_unary_args(args, "cdr")) {
    return NULL;
  }

  if (args->value.pair.car->type != SCHEME_PAIR) {
    printf("Error: cdr only operates on pairs.\n");
    return NULL;
  }

  return args->value.pair.car->value.pair.cdr;
}

// Scheme "null?" procedure
SchemeObject* primitive_is_null(SchemeObject* args) {
  if (!check_unary_args(args, "null?")) {
    return NULL;
  }

  return make_boolean(args->value.pair.car->type == SCHEME_NIL ? "#t" : "#f");
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
  return make_nil();
}

// Scheme "pair?" procedure
SchemeObject* primitive_is_pair(SchemeObject* args) {
  if (!check_unary_args(args, "pair?")){
    return NULL;
  }

  return make_boolean(args->value.pair.car->type == SCHEME_PAIR ? "#t" : "#f");
}

// Scheme "number?" procedure
SchemeObject* primitive_is_number(SchemeObject* args) {
  if (!check_unary_args(args, "number?")) {
    return NULL;
  }

  return make_boolean(args->value.pair.car->type == SCHEME_NUMBER ? "#t" : "#f");
}

// Scheme "symbol?" procedure
SchemeObject* primitive_is_symbol(SchemeObject* args) {
  if (!check_unary_args(args, "symbol?")) {
    return NULL;
  }

  return make_boolean(args->value.pair.car->type == SCHEME_SYMBOL ? "#t" : "#f");
}

// Scheme "boolean?" procedure
SchemeObject* primitive_is_boolean(SchemeObject* args) {
  if (!check_unary_args(args, "boolean?")) {
    return NULL;
  }

  return make_boolean(args->value.pair.car->type == SCHEME_BOOLEAN ? "#t" : "#f");
}

// Scheme "<" procedure
SchemeObject* primitive_lt(SchemeObject* args) {
  if (args == NULL || args->type != SCHEME_PAIR || args->value.pair.cdr == NULL || args->value.pair.cdr->type != SCHEME_PAIR) {
    printf("Error: < requires at least 2 arguments.\n");
    return NULL;
  }
  if (args->value.pair.car->type != SCHEME_NUMBER) {
    printf("Error: < only operates on numbers.\n");
    return NULL;
  }

  double prev = args->value.pair.car->value.number;
  args = args->value.pair.cdr;

  while (args != NULL && args->type == SCHEME_PAIR) {
    if (args->value.pair.car->type != SCHEME_NUMBER) {
      printf("Error: < only operates on numbers.\n");
      return NULL;
    }
    double curr = args->value.pair.car->value.number;
    if (!(prev < curr)) return make_boolean("#f");
    prev = curr;
    args = args->value.pair.cdr;
  }
  return make_boolean("#t");
}

// Scheme ">" procedure
SchemeObject* primitive_gt(SchemeObject* args) {
  if (args == NULL || args->type != SCHEME_PAIR || args->value.pair.cdr == NULL || args->value.pair.cdr->type != SCHEME_PAIR) {
    printf("Error: > requires at least 2 arguments.\n");
    return NULL;
  }

  if (args->value.pair.car->type != SCHEME_NUMBER) {
    printf("Error: > only operates on numbers.\n");
    return NULL;
  }

  double prev = args->value.pair.car->value.number;
  args = args->value.pair.cdr;

  while (args != NULL && args->type == SCHEME_PAIR) {
    if (args->value.pair.car->type != SCHEME_NUMBER) {
      printf("Error: > only operates on numbers.\n");
      return NULL;
    }
    double curr = args->value.pair.car->value.number;
    if (!(prev > curr)) {
      return make_boolean("#f");
    }
    prev = curr;
    args = args->value.pair.cdr;
  }
  return make_boolean("#t");
}

// Scheme "<=" procedure
SchemeObject* primitive_lte(SchemeObject* args) {
  if (args == NULL || args->type != SCHEME_PAIR || args->value.pair.cdr == NULL || args->value.pair.cdr->type != SCHEME_PAIR) {
    printf("Error: <= requires at least 2 arguments.\n");
    return NULL;
  }

  if (args->value.pair.car->type != SCHEME_NUMBER) {
    printf("Error: <= only operates on numbers.\n");
    return NULL;
  }

  double prev = args->value.pair.car->value.number;
  args = args->value.pair.cdr;

  while (args != NULL && args->type == SCHEME_PAIR) {
    if (args->value.pair.car->type != SCHEME_NUMBER) {
      printf("Error: <= only operates on numbers.\n");
      return NULL;
    }
    double curr = args->value.pair.car->value.number;
    if (!(prev <= curr)) {
      return make_boolean("#f");
    }
    prev = curr;
    args = args->value.pair.cdr;
  }
  return make_boolean("#t");
}

// Scheme ">=" procedure
SchemeObject* primitive_gte(SchemeObject* args) {
  if (args == NULL || args->type != SCHEME_PAIR || args->value.pair.cdr == NULL || args->value.pair.cdr->type != SCHEME_PAIR) {
    printf("Error: >= requires at least 2 arguments.\n");
    return NULL;
  }

  if (args->value.pair.car->type != SCHEME_NUMBER) {
    printf("Error: >= only operates on numbers.\n");
    return NULL;
  }

  double prev = args->value.pair.car->value.number;
  args = args->value.pair.cdr;

  while (args != NULL && args->type == SCHEME_PAIR) {
    if (args->value.pair.car->type != SCHEME_NUMBER) {
      printf("Error: >= only operates on numbers.\n");
      return NULL;
    }
    double curr = args->value.pair.car->value.number;
    if (!(prev >= curr)) {
      return make_boolean("#f");
    }
    prev = curr;
    args = args->value.pair.cdr;
  }
  return make_boolean("#t");
}
