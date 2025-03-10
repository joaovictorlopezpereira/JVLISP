#pragma once

// Headers
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include "object.h"
#include "types.h"

// Functions Signatures
Token* tokenize(const char* input);
void free_tokens(Token* tokens);
