
// Includes
#include "environment.c"


// Functions Signatures
SchemeObject* eval(SchemeObject* expr, Environment** env);
SchemeObject* apply(SchemeObject* func, SchemeObject* args);
SchemeObject* map_eval(SchemeObject* args, Environment** env);


// Evaluates a given expression in respect to an environment
SchemeObject* eval(SchemeObject* expr, Environment** env) {
  // in case the parser launches a error that is passed to eval
  if (expr == NULL) {
    return NULL;
  }

  switch (expr->type) {
    case SCHEME_NUMBER:
    case SCHEME_STRING:
    case SCHEME_BOOLEAN:
    case SCHEME_NIL:
      return expr;

    case SCHEME_SYMBOL:
    case SCHEME_PRIMITIVE:
      SchemeObject* value = lookup_variable(expr->value.symbol, *env);
      if (value == NULL) {
        printf("Error: undefined variable '%s'", expr->value.symbol);
      }
      return value;

    case SCHEME_PAIR:
      // In case the user enters a input in which the first element is not a symbol
      if (expr->value.pair.car->type != SCHEME_SYMBOL) {
        printf("Error: application not a procedure.");
        return NULL;
      }

      // quote
      if (strcmp(expr->value.pair.car->value.symbol, "quote") == 0) {
        return expr->value.pair.cdr;
      }

      // define
      if (strcmp(expr->value.pair.car->value.symbol, "define") == 0) {
        SchemeObject* symbol = expr->value.pair.cdr->value.pair.car;
        SchemeObject* value = eval(expr->value.pair.cdr->value.pair.cdr->value.pair.car, env);
        if (value->type == SCHEME_LAMBDA) {
          value->value.lambda.env = add_variable(value->value.lambda.env, symbol->value.symbol, value);
        }
        if (lookup_variable(symbol->value.symbol, *env) != NULL) {
          printf("Error: variable '%s' already defined.", symbol->value.symbol);
          return NULL;
        }
        *env = add_variable(*env, symbol->value.symbol, value);
        printf("Variable '%s' defined in the global environment.", symbol->value.symbol);
        return NULL;
      }

      // lambda
      if (strcmp(expr->value.pair.car->value.symbol, "lambda") == 0) {
        SchemeObject* params = expr->value.pair.cdr->value.pair.car;
        SchemeObject* body = expr->value.pair.cdr->value.pair.cdr;
        return make_lambda(params, body, *env);
      }

      // if
      if (strcmp(expr->value.pair.car->value.symbol, "if") == 0) {
        SchemeObject* condition = eval(expr->value.pair.cdr->value.pair.car, env);
        if (condition->type == SCHEME_BOOLEAN && condition->value.boolean == 1) {
          return eval(expr->value.pair.cdr->value.pair.cdr->value.pair.car, env);
        }
        else {
          return eval(expr->value.pair.cdr->value.pair.cdr->value.pair.cdr->value.pair.car, env);
        }
      }

      // Primitive or Lambda
      SchemeObject* first = eval(expr->value.pair.car, env);
      if (first->type == SCHEME_PRIMITIVE || first->type == SCHEME_LAMBDA) {
        SchemeObject* args = map_eval(expr->value.pair.cdr, env);
        return apply(first, args);
      }

      return NULL;

    default:
      return NULL;
  }
}

// Applies a given function to a list of arguments
SchemeObject* apply(SchemeObject* func, SchemeObject* args) {
  // if it's a primitive, then simply applies the function to the arguments
  if (func->type == SCHEME_PRIMITIVE) {
    SchemeObject* (*f)(SchemeObject* args) = get_primitive_function(func->value.symbol);
    return f(args);
  }

  // if it's a lambda, extends the environment and then apply the function to the respect in respect to the new environment
  else if (func->type == SCHEME_LAMBDA) {
    Environment* new_env = extend_environment(func->value.lambda.params, args, func->value.lambda.env);
    return eval(func->value.lambda.body->value.pair.car, &new_env);
  }

  else {
    printf("Error: the procedure being applied is neither a primitive or a lambda.");
  }
  return NULL;
}

// Maps eval to a list of arguments in respect to an environment
SchemeObject* map_eval(SchemeObject* args, Environment** env) {
  if (args == NULL || args->type == SCHEME_NIL) {
    return make_nil();
  }
  return make_pair(eval(args->value.pair.car, env), map_eval(args->value.pair.cdr, env));
}
