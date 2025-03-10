#pragma once

// Headers
#include "object.h"
#include "environment.h"

// Functions Signatures
SchemeObject* eval(SchemeObject* expr, Environment** env);
SchemeObject* apply(SchemeObject* func, SchemeObject* args);
SchemeObject* map_eval(SchemeObject* args, Environment** env);
