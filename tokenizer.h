#pragma once

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include "object.h"

// A Token type is either (, ), a symbol, a number a EOF or invalid
typedef enum {
  TOKEN_LEFTPAREN,  // (
  TOKEN_RIGHTPAREN,  // )
  TOKEN_SYMBOL,  // Symbol name (e.g., "+", "x")
  TOKEN_NUMBER,  // Number (e.g., "1", "42")
  TOKEN_EOF,     // End of input
} TokenType;

// A Token has a TokenType and a Token
typedef struct {
  TokenType type;
  char* value;
} Token;

// Functions Signatures
Token* tokenize(const char* input);
void free_tokens(Token* tokens);

// Tokenizes the input
Token* tokenize(const char* input) {
  Token* tokens = (Token*)malloc(sizeof(Token) * 100); // 100 tokens por input
  int token_count = 0;

  // Allocs a copy of the input
  char* str = strdup(input);
  char* current = str;

  while (*current != '\0') {
    // Ignores whitespaces
    if (isspace(*current)) {
      current++;
      continue;
    }
    // If it is a (, the token is a Scheme (
    if (*current == '(') {
      tokens[token_count++] = (Token){TOKEN_LEFTPAREN, "("};
      current++;
    }
    // If it is a ), the token is a Scheme )
    else if (*current == ')') {
      tokens[token_count++] = (Token){TOKEN_RIGHTPAREN, ")"};
      current++;
    }
    // If it is a number, the token is a Scheme number
    else if (isdigit(*current)) {
      char* start = current;
      while (isdigit(*current)) {
        current++;
      }
      int len = current - start;
      char* num_str = (char*)malloc(len + 1);
      strncpy(num_str, start, len);
      num_str[len] = '\0';
      tokens[token_count++] = (Token){TOKEN_NUMBER, num_str};
    }
    // Else, the token is a Scheme symbol
    else {
      char* start = current;
      while (!isspace(*current) && *current != '(' && *current != ')' && *current != '\0') {
        current++;
      }
      int len = current - start;
      char* symbol_str = (char*)malloc(len + 1);
      strncpy(symbol_str, start, len);
      symbol_str[len] = '\0';
      tokens[token_count++] = (Token){TOKEN_SYMBOL, symbol_str};
    }
  }

  // Adds the EOF token at the end
  tokens[token_count++] = (Token){TOKEN_EOF, NULL};

  free(str); // Frees the temporary string allocated by strdup
  return tokens;
}

// Frees memory allocated to tokens
void free_tokens(Token* tokens) {
  int i = 0;
  while (tokens[i].type != TOKEN_EOF) {
    free(tokens[i].value);
    i++;
  }
  free(tokens);
}
