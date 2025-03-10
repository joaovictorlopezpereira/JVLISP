#pragma once

// Headers
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "types.h"

// Functions Signatures
SchemeObject* make_number(double number);
SchemeObject* make_symbol(const char* symbol);
SchemeObject* make_string(const char* string);
SchemeObject* make_pair(SchemeObject* car, SchemeObject* cdr);
SchemeObject* make_primitive(SchemeObject* (*func)(SchemeObject* args), const char* string);
SchemeObject* make_lambda(SchemeObject* params, SchemeObject* body, Environment* env);
SchemeObject* make_boolean(const char* string);
SchemeObject* make_nil();
void free_scheme_object(SchemeObject* object);
void print_scheme_object(SchemeObject* object);
