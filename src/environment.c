
// Includes
#include "primitives.c"

// Functions Signatures
SchemeObject* lookup_variable(const char* name, Environment* env);
Environment* add_variable(Environment* env, const char* name, SchemeObject* value);
static Environment* init_environment();
Environment* extend_environment(SchemeObject* params, SchemeObject* args, Environment* env);
void free_environment(Environment* env);

// Looks up the value of a variable in the environment
SchemeObject* lookup_variable(const char* name, Environment* env) {
  while (env != NULL) {
    if (strcmp(env->name, name) == 0) {
      return env->value;
    }
    env = env->next;
  }
  return NULL;
}

// Inserts a new frame to the environment
Environment* add_variable(Environment* env, const char* name, SchemeObject* value) {
  Environment* new_env = (Environment*)malloc(sizeof(Environment));
  new_env->name = strdup(name);
  new_env->value = value;
  new_env->next = env;
  return new_env;
}

// Initializes the environment
static Environment* init_environment() {
  Environment* env = NULL;
  size_t num_primitives = sizeof(primitives) / sizeof(primitives[0]);

  for (size_t i = 0; i < num_primitives; i++) {
    env = add_variable(
      env,
      primitives[i].symbol,
      make_primitive(primitives[i].function, primitives[i].symbol)
    );
  }

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
