#pragma once

#include "types.h"
#include "primitives.h"

SchemeObject* eval(SchemeObject* expr, Environment* env);
SchemeObject* apply(SchemeObject* func, SchemeObject* args);

// Looks up the value of a variable in the environment
SchemeObject* lookup_variable(char* name, Environment* env) {
  while (env != NULL) {
    if (strcmp(env->name, name) == 0) {
      return env->value;
    }
    env = env->next;
  }
  printf("Error: variable '%s' not found\n", name);
  exit(1);
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

  return env;
}

// Extends the environment by associating parameters with arguments
Environment* extend_environment(SchemeObject* params, SchemeObject* args, Environment* env) {
  Environment* new_env = NULL;

  while (params != NULL && args != NULL) {
    // Assumes that params is a pair of symbols and args are evaluated
    new_env = add_variable(new_env, params->value.pair.car->value.symbol, eval(args->value.pair.car, env));
    params = params->value.pair.cdr;
    args = args->value.pair.cdr;
  }

  new_env = add_variable(new_env, "env", make_symbol("env"));
  new_env->next = env;

  return new_env;
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
