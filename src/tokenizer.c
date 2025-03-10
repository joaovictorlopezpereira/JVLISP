
// Headers
#include "tokenizer.h"

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
    // Scheme's (
    if (*current == '(') {
      tokens[token_count++] = (Token){TOKEN_LEFTPAREN, "("};
      current++;
    }
    // Scheme's)
    else if (*current == ')') {
      tokens[token_count++] = (Token){TOKEN_RIGHTPAREN, ")"};
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

    // Scheme's number
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
}
