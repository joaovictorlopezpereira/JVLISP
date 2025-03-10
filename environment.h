#pragma once

#include "types.h"
#include "primitives.h"


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
  printf("Error: variable '%s' not found\n", name);
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

  return env;
}

// Extends the environment by associating parameters with arguments
Environment* extend_environment(SchemeObject* params, SchemeObject* args, Environment* env) {
  Environment* new_env = NULL;

  while (params != NULL && params->type != SCHEME_NIL && args != NULL && args->type != SCHEME_NIL) {
    new_env = add_variable(new_env, params->value.pair.car->value.symbol, args->value.pair.car);
    params = params->value.pair.cdr;
    args = args->value.pair.cdr;
  }

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
