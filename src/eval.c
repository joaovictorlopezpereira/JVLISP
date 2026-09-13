
// Includes
#include "environment.c"

// Functions Signatures
SchemeObject* eval(SchemeObject* expr, Environment** env);
SchemeObject* apply(SchemeObject* func, SchemeObject* args);
SchemeObject* map_eval(SchemeObject* args, Environment** env);

// Evaluates a given expression in respect to an environment
SchemeObject* eval(SchemeObject* expr, Environment** env) {

  // In case the parser (or recursive calls of eval) launches a error that is passed to eval
  if (expr == NULL) {
    return NULL;
  }

  // Pattern matching on the input type
  switch (expr->type) {
    case SCHEME_NUMBER:
    case SCHEME_STRING:
    case SCHEME_BOOLEAN:
    case SCHEME_NIL:
    case SCHEME_PRIMITIVE:
    case SCHEME_LAMBDA:
      return expr;

    case SCHEME_SYMBOL: {
      SchemeObject* value = lookup_variable(expr->value.symbol, *env);
      if (value == NULL) {
        printf("Error: undefined variable '%s'.\n", expr->value.symbol);
      }
      return value;
    }

    case SCHEME_PAIR: {
      SchemeObject* head = expr->value.pair.car;
      int is_symbol = (head != NULL && head->type == SCHEME_SYMBOL);

      // quote
      if (is_symbol && strcmp(head->value.symbol, "quote") == 0) {
        SchemeObject* args = expr->value.pair.cdr;
        if (args == NULL || args->type != SCHEME_PAIR) {
          printf("Error: quote operates on a single argument.\n");
          return NULL;
        }
        return args->value.pair.car;
      }

      // define
      if (is_symbol && strcmp(head->value.symbol, "define") == 0) {
        SchemeObject* symbol = expr->value.pair.cdr->value.pair.car;
        SchemeObject* value = eval(expr->value.pair.cdr->value.pair.cdr->value.pair.car, env);

        if (value == NULL) {
          return NULL;
        }

        if (value->type == SCHEME_LAMBDA) {
          value->value.lambda.env = add_variable(value->value.lambda.env, symbol->value.symbol, value);
        }

        if (lookup_variable(symbol->value.symbol, *env) != NULL) {
          printf("Error: variable '%s' already defined.\n", symbol->value.symbol);
          return NULL;
        }

        *env = add_variable(*env, symbol->value.symbol, value);
        printf("Variable '%s' defined in the global environment.\n", symbol->value.symbol);
        return make_nil();
      }

      // lambda
      if (is_symbol && strcmp(head->value.symbol, "lambda") == 0) {
        SchemeObject* params = expr->value.pair.cdr->value.pair.car;
        SchemeObject* body = expr->value.pair.cdr->value.pair.cdr;
        return make_lambda(params, body, *env);
      }

      // if
      if (is_symbol && strcmp(head->value.symbol, "if") == 0) {
        SchemeObject* condition = eval(expr->value.pair.cdr->value.pair.car, env);
        if (condition == NULL) {
          return NULL;
        }

        int is_false = (condition->type == SCHEME_BOOLEAN && condition->value.boolean == 0);

        if (!is_false) {
          return eval(expr->value.pair.cdr->value.pair.cdr->value.pair.car, env);
        }

        else {
          SchemeObject* alt = expr->value.pair.cdr->value.pair.cdr->value.pair.cdr;

          if (alt != NULL && alt->type == SCHEME_PAIR) {
            return eval(alt->value.pair.car, env);
          }

          else {
            return make_nil();
          }
        }
      }

      // Primitive or Lambda
      SchemeObject* proc = eval(head, env);
      if (proc == NULL) {
        return NULL;
      }

      if (proc->type == SCHEME_PRIMITIVE || proc->type == SCHEME_LAMBDA) {
        SchemeObject* args = map_eval(expr->value.pair.cdr, env);

        if (args == NULL && expr->value.pair.cdr != NULL && expr->value.pair.cdr->type != SCHEME_NIL) {
          return NULL;
        }

        return apply(proc, args);
      }

      printf("Error: variable is not a procedure.\n");
      return NULL;
    }

    default:
      return NULL;
  }
}

// Applies a given function to a list of arguments
SchemeObject* apply(SchemeObject* func, SchemeObject* args) {

  // If it's a primitive function, apply it directly
  if (func->type == SCHEME_PRIMITIVE) {
    return func->value.primitive.function(args);
  }

  // If it's a lambda function, check arity and extend the environment before applying
  else if (func->type == SCHEME_LAMBDA) {

    // Counts the number of expected parameters
    int expected_args = 0;
    SchemeObject* param_list = func->value.lambda.params;
    while (param_list != NULL && param_list->type != SCHEME_NIL) {
      expected_args++;
      param_list = param_list->value.pair.cdr;
    }

    // Counts the number of provided arguments
    int given_args = 0;
    SchemeObject* arg_list = args;
    while (arg_list != NULL && arg_list->type != SCHEME_NIL) {
      given_args++;
      arg_list = arg_list->value.pair.cdr;
    }

    // Checks if the number of arguments matches the expected number
    if (given_args != expected_args) {
      printf("Error: function expected %d arguments but got %d.\n", expected_args, given_args);
      return NULL;
    }

    // Extends the environment and evaluates the function
    Environment* new_env = extend_environment(func->value.lambda.params, args, func->value.lambda.env);
    return eval(func->value.lambda.body->value.pair.car, &new_env);
  }

  else {
    printf("Error: the procedure being applied is neither a primitive nor a lambda.\n");
  }
  return NULL;
}

// Maps eval to a list of arguments in respect to an environment
SchemeObject* map_eval(SchemeObject* args, Environment** env) {

  // If args are null, return the empty list
  if (args == NULL || args->type == SCHEME_NIL) {
    return make_nil();
  }

  // Evaluates the head of the args
  SchemeObject* evaluated_arg = eval(args->value.pair.car, env);
  if (evaluated_arg == NULL) {
    return NULL;
  }

  // Recursive call to evaluate the tail of the args
  SchemeObject* rest_evaluated = map_eval(args->value.pair.cdr, env);
  if (rest_evaluated == NULL) {
    return NULL;
  }

  // Returns the list of evalued args
  return make_pair(evaluated_arg, rest_evaluated);
}
