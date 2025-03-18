
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
  env = add_variable(env, "print", make_primitive(primitive_print, "print"));

  return env;
}

// Extends the environment given a list of parameters, arguments and the environment
Environment* extend_environment(SchemeObject* params, SchemeObject* args, Environment* env) {
  // If they are both NULL, return a error
  if (params == NULL && args == NULL) {
    printf("error: parameters and arguments are both NULL");
    return NULL;
  }

  // If one is nil or null and the other isn't, return a error
  if (((params->type == SCHEME_NIL && args->type != SCHEME_NIL) || (params->type != SCHEME_NIL && args->type == SCHEME_NIL)) || (params == NULL || args == NULL)) {
    printf("Error: Mismatch in parameter and argument lengths.\n");
    return NULL;
  }

  // If they are both nil, the list of parameters and arguments is over
  if (params->type == SCHEME_NIL && args->type == SCHEME_NIL) {
    return env;
  }

  // Add a relation between parameter and argument to the environment
  Environment* new_env = add_variable(env, params->value.pair.car->value.symbol, args->value.pair.car);

  // Recursively extend the environment to the remaining variables
  return extend_environment(params->value.pair.cdr, args->value.pair.cdr, new_env);
}

// Frees the memory allocated by the environment
void free_environment(Environment* env) {
  while (env != NULL) {
    free(env->name);   // Free the name string
    free(env->value);  // Free the SchemeObject value
    Environment* temp = env;
    env = env->next;
    free(temp);
  }
}
