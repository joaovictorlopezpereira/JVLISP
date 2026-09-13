
// Includes
#include "primitives.c"

// Functions Signatures
SchemeObject* lookup_variable(char* name, Environment* env);
Environment* add_variable(Environment* env, char* name, SchemeObject* value);
Environment* init_environment();
Environment* extend_environment(SchemeObject* params, SchemeObject* args, Environment* env);
void free_environment(Environment* env);

// Looks up the value of a variable in the environment
SchemeObject* lookup_variable(char* name, Environment* env) {
  while (env != NULL) {
    if (strcmp(env->name, name) == 0) {
      return env->value;
    }
    env = env->next;
  }
  return NULL;
}

// Inserts a new frame to the environment
Environment* add_variable(Environment* env, char* name, SchemeObject* value) {
  Environment* new_env = (Environment*)malloc(sizeof(Environment));
  new_env->name = strdup(name);
  new_env->value = value;
  new_env->next = env;
  return new_env;
}

// Initializes the environment
Environment* init_environment() {
  Environment* env = NULL;

  // Adds the primitive functions to the environment
  env = add_variable(env, "+", make_primitive(primitive_add, "+"));
  env = add_variable(env, "-", make_primitive(primitive_sub, "-"));
  env = add_variable(env, "*", make_primitive(primitive_mul, "*"));
  env = add_variable(env, "/", make_primitive(primitive_div, "/"));
  env = add_variable(env, "=", make_primitive(primitive_equal_sign, "="));
  env = add_variable(env, "cons", make_primitive(primitive_cons, "cons"));
  env = add_variable(env, "car", make_primitive(primitive_car, "car"));
  env = add_variable(env, "cdr", make_primitive(primitive_cdr, "cdr"));
  env = add_variable(env, "null?", make_primitive(primitive_is_null, "null?"));
  env = add_variable(env, "display", make_primitive(primitive_display, "display"));
  env = add_variable(env, "pair?", make_primitive(primitive_is_pair, "pair?"));
  env = add_variable(env, "number?", make_primitive(primitive_is_number, "number?"));
  env = add_variable(env, "symbol?", make_primitive(primitive_is_symbol, "symbol?"));
  env = add_variable(env, "boolean?", make_primitive(primitive_is_boolean, "boolean?"));
  env = add_variable(env, "<",  make_primitive(primitive_lt, "<"));
  env = add_variable(env, ">",  make_primitive(primitive_gt, ">"));
  env = add_variable(env, "<=", make_primitive(primitive_lte, "<="));
  env = add_variable(env, ">=", make_primitive(primitive_gte, ">="));

  return env;
}

// Auxiliar function to check if a list has ended
static inline int is_empty_list(SchemeObject* obj) {
  return obj == NULL || obj->type == SCHEME_NIL;
}

// Extends the environment given a list of parameters, arguments and the environment
Environment* extend_environment(SchemeObject* params, SchemeObject* args, Environment* env) {

  // If they are both NULL or NIL, returns the current environment
  if (is_empty_list(params) && is_empty_list(args)) {
    return env;
  }

  // If one is either nil or null and the other isn't, returns a error
  if (is_empty_list(params) || is_empty_list(args)) {
    printf("Error: mismatch in parameter and argument lengths.\n");
    return NULL;
  }

  // Valids the types in the list
  if (params->type != SCHEME_PAIR || args->type != SCHEME_PAIR) {
    printf("Error: invalir parameter or argument structure.\n");
    return NULL;
  }

  SchemeObject* param_sym = params->value.pair.car;
  if (param_sym == NULL || param_sym->type != SCHEME_SYMBOL) {
    printf("Error: formal parameters must be symbols\n");
    return NULL;
  }

  // Adds a relation between parameter and argument to the environment
  Environment* new_env = add_variable(env, param_sym->value.symbol, args->value.pair.car);

  // Recursively extends the environment to the remaining variables
  return extend_environment(params->value.pair.cdr, args->value.pair.cdr, new_env);
}

// Frees the memory allocated by the environment
void free_environment(Environment* env) {
  while (env != NULL) {
    free(env->name); // Frees strdup done in add_variable
    Environment* temp = env;
    env = env->next;
    free(temp);
  }
}
