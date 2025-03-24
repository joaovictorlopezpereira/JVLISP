
// Includes
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include "object.c"
#include "types.h"


// Functions Signatures
Token* tokenize(const char* input);
void free_tokens(Token* tokens);


// Tokenizes the input
Token* tokenize(const char* input) {
  int capacity = 100;
  Token* tokens = (Token*)malloc(sizeof(Token) * capacity);

  int token_count = 0;
  const char* current = input;

  while (*current != '\0') {
    // Ignores whitespaces
    if (isspace(*current)) {
      current++;
      continue;
    }

    // Enlarges the tokens if necessary
    if (token_count >= capacity - 1) {
      capacity *= 2;
      tokens = (Token*)realloc(tokens, sizeof(Token) * capacity);
    }

    // Scheme's (
    if (*current == '(') {
      tokens[token_count++] = (Token){TOKEN_LEFTPAREN, _strdup("(")};
      current++;
    }

    // Scheme's )
    else if (*current == ')') {
      tokens[token_count++] = (Token){TOKEN_RIGHTPAREN, _strdup(")")};
      current++;
    }

    // Scheme's #t and #f
    else if (*current == '#' && (current[1] == 't' || current[1] == 'f')) {
      char* bool_str = (char*)malloc(3);
      if (bool_str) {
          bool_str[0] = current[0];
          bool_str[1] = current[1];
          bool_str[2] = '\0';
      }
      tokens[token_count++] = (Token){TOKEN_BOOLEAN, bool_str};
      current += 2;
    }

    // Scheme's nil
    else if (strncmp(current, "'()", 3) == 0) {
      tokens[token_count++] = (Token){TOKEN_NIL, _strdup("'()")};
      current += 3;
    }

    // Scheme's number
    else if (isdigit(*current) || (*current == '-' && isdigit(current[1]))) {
      const char* start = current;
      if (*current == '-') current++; // Negative numbers

      while (isdigit(*current)) current++;
      if (*current == '.') { // Decimal numbers
        current++;
        while (isdigit(*current)) current++;
      }

      char* num_str = (char*)malloc(current - start + 1);
      memcpy(num_str, start, current - start);
      num_str[current - start] = '\0';
      tokens[token_count++] = (Token){TOKEN_NUMBER, num_str};
    }

    // Scheme's string
    else if (*current == '"') {
      current++; // Skip opening quote
      const char* start = current;

      while (*current != '\0' && *current != '"') {
        if (*current == '\\' && (current[1] == '"' || current[1] == '\\')) current++; // Suporte a escapes
        current++;
      }

      if (*current == '"') {
        char* str_val = (char*)malloc(current - start + 1);
        memcpy(str_val, start, current - start);
        str_val[current - start] = '\0';
        tokens[token_count++] = (Token){TOKEN_STRING, str_val};
        current++; // Skip closing quote
      }
      else {
        printf("Error: string not closed.\n");
      }
    }

    // Else, the token is a Scheme symbol
    else {
      const char* start = current;
      while (!isspace(*current) && *current != '(' && *current != ')' && *current != '\0') {
        current++;
      }
      char* symbol_str = (char*)malloc(current - start + 1);
      memcpy(symbol_str, start, current - start);
      symbol_str[current - start] = '\0';
      tokens[token_count++] = (Token){TOKEN_SYMBOL, symbol_str};
    }
  }

  // Adds the EOF token at the end
  tokens[token_count++] = (Token){TOKEN_EOF, NULL};
  return tokens;
}

// Frees memory allocated to tokens
void free_tokens(Token* tokens) {
  if (!tokens) return;
  for (int i = 0; tokens[i].type != TOKEN_EOF; i++) {
    if (tokens[i].value) {
      free(tokens[i].value);
    }
  }
  free(tokens);
}

