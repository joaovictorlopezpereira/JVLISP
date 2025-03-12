#pragma once

// A Scheme type is either a number, a symbol, a string, a pair, a primitive, a lambda, a boolean or a nil.
typedef enum {
  SCHEME_NUMBER,
  SCHEME_SYMBOL,
  SCHEME_STRING,
  SCHEME_PAIR,
  SCHEME_PRIMITIVE,
  SCHEME_LAMBDA,
  SCHEME_BOOLEAN,
  SCHEME_NIL,
} SchemeType;

// A Scheme object has a SchemeType and a value
typedef struct SchemeObject {
  // The SchemeObject's type
  SchemeType type;
  // The SchemeObject's value
  union {
    double number;
    char *symbol;
    char *string;
    struct {
      struct SchemeObject *car;
      struct SchemeObject *cdr;
    } pair;
    struct SchemeObject *(*primitive)(struct SchemeObject *args);
    struct {
      struct SchemeObject *params;
      struct SchemeObject *body;
      struct Environment *env;
    } lambda;
    int boolean;
  } value;
} SchemeObject;

// A Scheme Environment (frame) has a name related to a value and a pointer to the next frame
typedef struct Environment {
  char* name;
  struct SchemeObject* value;
  struct Environment* next;
} Environment;

// A PrimitiveMapping has a symbol and a related function
typedef struct {
  const char* symbol;
  SchemeObject* (*function)(SchemeObject* args);
} PrimitiveMapping;

// A Token type is either (, ), a symbol, a number a EOF or invalid
typedef enum {
  TOKEN_LEFTPAREN,  // (
  TOKEN_RIGHTPAREN, // )
  TOKEN_BOOLEAN,    // #t #f
  TOKEN_SYMBOL,     // Symbol name (e.g., "+", "x")
  TOKEN_NUMBER,     // Number (e.g., "1", "42")
  TOKEN_NIL,        // '() nil
  TOKEN_EOF,        // End of input
} TokenType;

// A Token has a TokenType and a value
typedef struct {
  TokenType type;
  char* value;
} Token;
