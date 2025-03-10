#pragma once

// Headers
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include "primitives.h"
#include "tokenizer.h"

// Functions Signatures
SchemeObject* parse_list(Token** tokens);
SchemeObject* parser(Token** tokens);
