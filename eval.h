#pragma once

#include "object.h"
#include "environment.h"

// Functions Signatures
SchemeObject* eval(SchemeObject* expr, Environment* env);
SchemeObject* apply(SchemeObject* func, SchemeObject* args);
SchemeObject* map_eval(SchemeObject* args, Environment* env);


SchemeObject* eval(SchemeObject* expr, Environment* env) {
  switch (expr->type) {
    case SCHEME_NUMBER:
    case SCHEME_STRING:
    case SCHEME_BOOLEAN:
    case SCHEME_NIL:
      return expr;

    // Define must come before symbol!

    case SCHEME_SYMBOL:
    case SCHEME_PRIMITIVE:
      return lookup_variable(expr->value.symbol, env);

    case SCHEME_PAIR:
      SchemeObject* first = eval(expr->value.pair.car, env);

      if (first->type == SCHEME_PRIMITIVE || first->type == SCHEME_LAMBDA) {
        SchemeObject* args = map_eval(expr->value.pair.cdr, env);
        return apply(first, args);
      }

      // if (first->type == SCHEME_SYMBOL && strcmp(first->value.symbol, "define") == 0) {
      //   SchemeObject* symbol = expr->value.pair.cdr->value.pair.car;
      //   SchemeObject* value = eval(expr->value.pair.cdr->value.pair.cdr->value.pair.car, env);
      //   env = add_variable(env, symbol->value.symbol, value);
      //   return value;
      // }

      return NULL;

    default:
      return NULL;
  }
}


SchemeObject* apply(SchemeObject* func, SchemeObject* args) {
  if (func->type == SCHEME_PRIMITIVE) {
    SchemeObject* (*f)(SchemeObject* args) = get_primitive_function(func->value.symbol);
    return f(args);
  }

  // if (func->type == SCHEME_LAMBDA) {
  //   Environment* new_env = extend_environment(func->value.lambda.params, args, func->value.lambda.env);
  //   return eval(func->value.lambda.body, new_env);
  // }

  return NULL;
}

SchemeObject* map_eval(SchemeObject* args, Environment* env){
  if (args == NULL || args->type == SCHEME_NIL) {
    return NULL;
  }
  return make_pair(eval(args->value.pair.car, env), map_eval(args->value.pair.cdr, env));
}