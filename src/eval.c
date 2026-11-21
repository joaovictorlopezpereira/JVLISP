
// Includes
#include "environment.c"

// Functions Signatures
static int is_truthy(SchemeObject* obj);
static SchemeObject* eval_symbol(SchemeObject* expr, Environment* env);
static SchemeObject* eval_quote(SchemeObject* args);
static SchemeObject* eval_define(SchemeObject* args, Environment** env);
static SchemeObject* eval_if(SchemeObject* args, Environment** env);
static SchemeObject* eval_lambda(SchemeObject* args, Environment* env);
static SchemeObject* cond_to_if(SchemeObject* clauses);
static SchemeObject* eval_cond(SchemeObject* clauses, Environment** env);
static SchemeObject* let_to_combination(SchemeObject* args);
static SchemeObject* eval_let(SchemeObject* args, Environment** env);
static SchemeObject* eval_sequence(SchemeObject* exps, Environment** env);
static SchemeObject* map_eval(SchemeObject* args, Environment** env);
SchemeObject* eval(SchemeObject* expr, Environment** env);
SchemeObject* apply(SchemeObject* func, SchemeObject* args);

// Verifies if a given Scheme Object is true or false
static int is_truthy(SchemeObject* obj) {
  if (obj != NULL && obj->type == SCHEME_BOOLEAN && obj->value.boolean == 0) {
    return 0;
  }
  return 1;
}

// Evaluates a given symbol (looks for its value in the environment)
static SchemeObject* eval_symbol(SchemeObject* expr, Environment* env) {
  SchemeObject* value = lookup_variable(expr->value.symbol, env);
  if (value == NULL) {
    printf("Error: undefined variable '%s'.\n", expr->value.symbol);
  }
  return value;
}

// Evaluates a quote special form
static SchemeObject* eval_quote(SchemeObject* args) {
  if (args == NULL || args->type != SCHEME_PAIR ||
      (args->value.pair.cdr != NULL && args->value.pair.cdr->type != SCHEME_NIL)) {
    printf("Error: quote takes exactly 1 argument.\n");
    return NULL;
  }
  return args->value.pair.car;
}

// Evaluates a define special form
static SchemeObject* eval_define(SchemeObject* args, Environment** env) {
  if (args == NULL || args->type != SCHEME_PAIR ||
      args->value.pair.cdr == NULL || args->value.pair.cdr->type != SCHEME_PAIR) {
    printf("Error: malformed define.\n");
    return NULL;
  }

  SchemeObject* first = args->value.pair.car;
  SchemeObject* symbol = NULL;
  SchemeObject* value = NULL;

  // Abbreviated form
  if (first != NULL && first->type == SCHEME_PAIR) {
    symbol = first->value.pair.car;
    if (symbol == NULL || symbol->type != SCHEME_SYMBOL) {
      printf("Error: function name in define must be a symbol.\n");
      return NULL;
    }

    SchemeObject* params = first->value.pair.cdr;
    SchemeObject* body = args->value.pair.cdr;
    SchemeObject* lambda_args = make_pair(params, body);

    value = eval_lambda(lambda_args, *env);
    free(lambda_args);
  }
  // Standard form
  else if (first != NULL && first->type == SCHEME_SYMBOL) {
    if (args->value.pair.cdr->value.pair.cdr != NULL &&
        args->value.pair.cdr->value.pair.cdr->type != SCHEME_NIL) {
      printf("Error: define takes exactly 2 arguments.\n");
      return NULL;
    }

    symbol = first;
    value = eval(args->value.pair.cdr->value.pair.car, env);
  }
  else {
    printf("Error: define expects a symbol or function signature.\n");
    return NULL;
  }

  if (value == NULL) {
    return NULL;
  }

  if (value->type == SCHEME_LAMBDA) {
    value->value.lambda.env = add_variable(value->value.lambda.env, symbol->value.symbol, value);
  }

  *env = add_variable(*env, symbol->value.symbol, value);
  return symbol;
}

// Evalues a if special form
static SchemeObject* eval_if(SchemeObject* args, Environment** env) {
  if (args == NULL || args->type != SCHEME_PAIR ||
      args->value.pair.cdr == NULL || args->value.pair.cdr->type != SCHEME_PAIR) {
    printf("Error: malformed if expression.\n");
    return NULL;
  }

  SchemeObject* then_branch = args->value.pair.cdr->value.pair.car;
  SchemeObject* alt_branch = args->value.pair.cdr->value.pair.cdr;

  if (alt_branch != NULL && alt_branch->type != SCHEME_NIL) {
    if (alt_branch->type != SCHEME_PAIR ||
        (alt_branch->value.pair.cdr != NULL && alt_branch->value.pair.cdr->type != SCHEME_NIL)) {
      printf("Error: malformed if expression (too many arguments).\n");
      return NULL;
    }
  }

  SchemeObject* condition = eval(args->value.pair.car, env);
  if (condition == NULL) {
    return NULL;
  }

  // Only evals the branch the if expression will return
  if (is_truthy(condition)) {
    return eval(then_branch, env);
  }
  else if (alt_branch != NULL && alt_branch->type == SCHEME_PAIR) {
    return eval(alt_branch->value.pair.car, env);
  }

  return make_nil();
}

// Evaluates a lambda special form
static SchemeObject* eval_lambda(SchemeObject* args, Environment* env) {
  if (args == NULL || args->type != SCHEME_PAIR ||
      args->value.pair.cdr == NULL || args->value.pair.cdr->type != SCHEME_PAIR) {
    printf("Error: malformed lambda expression.\n");
    return NULL;
  }

  SchemeObject* params = args->value.pair.car;
  SchemeObject* curr = params;
  while (curr != NULL && curr->type == SCHEME_PAIR) {
    if (curr->value.pair.car == NULL || curr->value.pair.car->type != SCHEME_SYMBOL) {
      printf("Error: lambda parameters must be symbols.\n");
      return NULL;
    }
    curr = curr->value.pair.cdr;
  }
  if (curr != NULL && curr->type != SCHEME_NIL) {
    printf("Error: malformed lambda parameter list.\n");
    return NULL;
  }

  SchemeObject* body = args->value.pair.cdr;
  return make_lambda(params, body, env);
}

// Transforms a cond special form to nested ifs
static SchemeObject* cond_to_if(SchemeObject* clauses) {
  if (clauses == NULL || clauses->type == SCHEME_NIL) {
    return make_nil();
  }
  if (clauses->type != SCHEME_PAIR) {
    printf("Error: malformed cond expression.\n");
    return NULL;
  }

  SchemeObject* clause = clauses->value.pair.car;
  if (clause == NULL || clause->type != SCHEME_PAIR ||
      clause->value.pair.cdr == NULL || clause->value.pair.cdr->type != SCHEME_PAIR) {
    printf("Error: malformed cond clause.\n");
    return NULL;
  }

  SchemeObject* predicate = clause->value.pair.car;
  SchemeObject* actions = clause->value.pair.cdr;
  SchemeObject* rest = clauses->value.pair.cdr;
  SchemeObject* consequence;

  if (actions->value.pair.cdr != NULL && actions->value.pair.cdr->type != SCHEME_NIL) {
    consequence = make_pair(make_symbol("begin"), actions);
  }
  else {
    consequence = actions->value.pair.car;
  }

  if (predicate != NULL && predicate->type == SCHEME_SYMBOL && strcmp(predicate->value.symbol, "else") == 0) {
    if (rest != NULL && rest->type != SCHEME_NIL) {
      printf("Error: else clause must be last in cond.\n");
      return NULL;
    }
    return consequence;
  }

  SchemeObject* alt = cond_to_if(rest);
  if (alt == NULL) {
    return NULL;
  }

  return make_pair(make_symbol("if"), make_pair(predicate, make_pair(consequence, make_pair(alt, make_nil()))));
}

// Evaluates a cond special form
static SchemeObject* eval_cond(SchemeObject* clauses, Environment** env) {
  SchemeObject* expanded = cond_to_if(clauses);
  if (expanded == NULL) {
    return NULL;
  }
  return eval(expanded, env);
}

// Transforms a let special form to nested lambdas
static SchemeObject* let_to_combination(SchemeObject* args) {
  if (args == NULL || args->type != SCHEME_PAIR ||
      args->value.pair.cdr == NULL || args->value.pair.cdr->type != SCHEME_PAIR) {
    printf("Error: malformed let expression.\n");
    return NULL;
  }

  SchemeObject* bindings = args->value.pair.car;
  SchemeObject* body = args->value.pair.cdr;

  SchemeObject* vars_head = make_nil();
  SchemeObject** vars_tail = &vars_head;

  SchemeObject* vals_head = make_nil();
  SchemeObject** vals_tail = &vals_head;

  while (bindings != NULL && bindings->type == SCHEME_PAIR) {
    SchemeObject* binding = bindings->value.pair.car;
    if (binding == NULL || binding->type != SCHEME_PAIR ||
        binding->value.pair.cdr == NULL || binding->value.pair.cdr->type != SCHEME_PAIR ||
        (binding->value.pair.cdr->value.pair.cdr != NULL &&
         binding->value.pair.cdr->value.pair.cdr->type != SCHEME_NIL)) {
      printf("Error: malformed binding in let.\n");
      return NULL;
    }

    SchemeObject* var = binding->value.pair.car;
    if (var == NULL || var->type != SCHEME_SYMBOL) {
      printf("Error: let binding variable must be a symbol.\n");
      return NULL;
    }

    SchemeObject* val = binding->value.pair.cdr->value.pair.car;

    *vars_tail = make_pair(var, make_nil());
    vars_tail = &((*vars_tail)->value.pair.cdr);

    *vals_tail = make_pair(val, make_nil());
    vals_tail = &((*vals_tail)->value.pair.cdr);

    bindings = bindings->value.pair.cdr;
  }

  if (bindings != NULL && bindings->type != SCHEME_NIL) {
    printf("Error: malformed bindings list in let.\n");
    return NULL;
  }

  SchemeObject* lambda_expr = make_pair(make_symbol("lambda"), make_pair(vars_head, body));
  return make_pair(lambda_expr, vals_head);
}

// Evaluates a let special form
static SchemeObject* eval_let(SchemeObject* args, Environment** env) {
  SchemeObject* lambda_call = let_to_combination(args);
  if (lambda_call == NULL) {
    return NULL;
  }
  return eval(lambda_call, env);
}

// Evaluates a list of expressions in order and returns the return of the last expression
static SchemeObject* eval_sequence(SchemeObject* exps, Environment** env) {
  if (exps == NULL || exps->type == SCHEME_NIL) {
    return make_nil();
  }

  SchemeObject* result = NULL;
  while (exps != NULL && exps->type == SCHEME_PAIR) {
    result = eval(exps->value.pair.car, env);
    if (result == NULL) {
      return NULL;
    }
    exps = exps->value.pair.cdr;
  }
  return result;
}

// Maps eval to a list of arguments in respect to an environment
static SchemeObject* map_eval(SchemeObject* args, Environment** env) {

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

    case SCHEME_SYMBOL:
      return eval_symbol(expr, *env);

    case SCHEME_PAIR: {
      SchemeObject* head = expr->value.pair.car;
      SchemeObject* args = expr->value.pair.cdr;

      // If its head is a symbol, dispatch the special forms
      if (head != NULL && head->type == SCHEME_SYMBOL) {
        const char* name = head->value.symbol;

        if (strcmp(name, "quote")  == 0) return eval_quote(args);
        if (strcmp(name, "define") == 0) return eval_define(args, env);
        if (strcmp(name, "if")     == 0) return eval_if(args, env);
        if (strcmp(name, "lambda") == 0) return eval_lambda(args, *env);
        if (strcmp(name, "begin")  == 0) return eval_sequence(args, env);
        if (strcmp(name, "cond")   == 0) return eval_cond(args, env);
        if (strcmp(name, "let")    == 0) return eval_let(args, env);
      }

      // Primitive or Lambda
      SchemeObject* proc = eval(head, env);
      if (proc == NULL) {
        return NULL;
      }

      if (proc->type == SCHEME_PRIMITIVE || proc->type == SCHEME_LAMBDA) {
        SchemeObject* evaled_args = map_eval(args, env);

        if (evaled_args == NULL && expr->value.pair.cdr != NULL && expr->value.pair.cdr->type != SCHEME_NIL) {
          return NULL;
        }

        return apply(proc, evaled_args);
      }

      printf("Error: variable is not a procedure.\n");
      return NULL;
    }

    default:
      printf("Error: unexpected case of eval.\n");
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

    return eval_sequence(func->value.lambda.body, &new_env);
  }

  else {
    printf("Error: the procedure being applied is neither a primitive nor a lambda.\n");
  }
  return NULL;
}
