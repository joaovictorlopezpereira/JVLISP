#pragma once

// Headers
#include "object.h"
#include <stdio.h>

// Functions Signatures
SchemeObject* primitive_add(SchemeObject* args);
SchemeObject* primitive_sub(SchemeObject* args);
SchemeObject* primitive_mul(SchemeObject* args);
SchemeObject* primitive_div(SchemeObject* args);
SchemeObject* primitive_equal_sign(SchemeObject* args);
SchemeObject* primitive_cons(SchemeObject* args);
SchemeObject* primitive_car(SchemeObject* args);
SchemeObject* primitive_cdr(SchemeObject* args);
SchemeObject* (*get_primitive_function(const char* symbol))(SchemeObject* args);
