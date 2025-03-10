#pragma once

// Headers
#include "types.h"
#include "primitives.h"

// Functions Signatures
SchemeObject* lookup_variable(char* name, Environment* env);
Environment* add_variable(Environment* env, char* name, SchemeObject* value);
Environment* init_environment();
Environment* extend_environment(SchemeObject* params, SchemeObject* args, Environment* env);
void free_environment(Environment* env);

