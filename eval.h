#pragma once

#include "object.h"
#include "environment.h"

SchemeObject* eval(SchemeObject* expr, Environment* env) {
  printf("\neval!\n");
  switch (expr->type) {
    case SCHEME_NUMBER:
    case SCHEME_STRING:
    case SCHEME_BOOLEAN:
    case SCHEME_NIL:
      return expr;

    case SCHEME_SYMBOL:
      return lookup_variable(expr->value.symbol, env);

    case SCHEME_PAIR:
      SchemeObject* first = eval(expr->value.pair.car, env);

      if (first->type == SCHEME_PRIMITIVE || first->type == SCHEME_LAMBDA) {
        SchemeObject* args = eval(expr->value.pair.cdr, env);
        return apply(first, args);
      }

      if (first->type == SCHEME_SYMBOL && strcmp(first->value.symbol, "define") == 0) {
        SchemeObject* symbol = expr->value.pair.cdr->value.pair.car;
        SchemeObject* value = eval(expr->value.pair.cdr->value.pair.cdr->value.pair.car, env);
        env = add_variable(env, symbol->value.symbol, value);
        return value;
      }

      return NULL;

    default:
      return NULL;
  }
}


SchemeObject* apply(SchemeObject* func, SchemeObject* args) {
  if (func->type == SCHEME_PRIMITIVE) {
    SchemeObject* (*f)(SchemeObject* args) = get_primitive_function(func->value.symbol);
    return f(args);  // Chama a função primitiva com os argumentos
  }

  if (func->type == SCHEME_LAMBDA) {
    Environment* new_env = extend_environment(func->value.lambda.params, args, func->value.lambda.env);
    return eval(func->value.lambda.body, new_env);
  }

  return NULL;
}

