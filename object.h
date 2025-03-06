#pragma once

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

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
      struct SchemeObject *env;
    } lambda;
    int boolean;
  } value;
} SchemeObject;

// Functions Signatures
SchemeObject* make_number(double number);
SchemeObject* make_symbol(const char* symbol);
SchemeObject* make_string(const char* string);
SchemeObject* make_pair(SchemeObject* car, SchemeObject* cdr);
SchemeObject* make_primitive(SchemeObject* (*func)(SchemeObject* args));
SchemeObject* make_lambda(SchemeObject* params, SchemeObject* body, SchemeObject* env);
SchemeObject* make_boolean(int value);
SchemeObject* make_nil();
void print_scheme_object(SchemeObject* object);

// Creates a Scheme number
SchemeObject* make_number(double number) {
  SchemeObject* object = (SchemeObject*)malloc(sizeof(SchemeObject));
  object->type = SCHEME_NUMBER;
  object->value.number = number;
  return object;
}

// Creates a Scheme symbol
SchemeObject* make_symbol(const char* symbol) {
  SchemeObject* object = (SchemeObject*)malloc(sizeof(SchemeObject));
  object->type = SCHEME_SYMBOL;
  object->value.symbol = strdup(symbol); // allocs memory and returns a pointer to the char* symbol
  return object;
}

// Creates a Scheme string
SchemeObject* make_string(const char* string) {
  SchemeObject* object = (SchemeObject*)malloc(sizeof(SchemeObject));
  object->type = SCHEME_STRING;
  object->value.string = strdup(string); // allocs memory and returns a pointer to the char* string
  return object;
}

// Creates a Scheme pair
SchemeObject* make_pair(SchemeObject* car, SchemeObject* cdr) {
  SchemeObject* object = (SchemeObject*)malloc(sizeof(SchemeObject));
  object->type = SCHEME_PAIR;
  object->value.pair.car = car;
  object->value.pair.cdr = cdr;
  return object;
}

// Creates a Scheme primitive
SchemeObject* make_primitive(SchemeObject* (*func)(SchemeObject* args)) {
  SchemeObject* object = (SchemeObject*)malloc(sizeof(SchemeObject));
  object->type = SCHEME_PRIMITIVE;
  object->value.primitive = func;
  return object;
}

// Creates a Scheme lambda
SchemeObject* make_lambda(SchemeObject* params, SchemeObject* body, SchemeObject* env) {
  SchemeObject* object = (SchemeObject*)malloc(sizeof(SchemeObject));
  object->type = SCHEME_LAMBDA;
  object->value.lambda.params = params;
  object->value.lambda.body = body;
  object->value.lambda.env = env;
  return object;
}

// Creates a Scheme boolean
SchemeObject* make_boolean(int value) {
  SchemeObject* object = (SchemeObject*)malloc(sizeof(SchemeObject));
  object->type = SCHEME_BOOLEAN;
  object->value.boolean = (value != 0);  // Makes every number be false except for 0
  return object;
}

// Creates a Scheme nil. A possibility is to make this object a singleton so that memory won't be allocated whenever nil is created
SchemeObject* make_nil() {
  SchemeObject* object = (SchemeObject*)malloc(sizeof(SchemeObject));
  object->type = SCHEME_NIL;
  return object;
}

// Frees the memory allocated by the SchemeObject
void free_object(SchemeObject* object) {
  switch (object->type) {
    case SCHEME_SYMBOL:
      free(object->value.symbol);
      break;
    case SCHEME_STRING:
      free(object->value.string);
      break;
    case SCHEME_PAIR:
      free_object(object->value.pair.car);
      free_object(object->value.pair.cdr);
      break;
    case SCHEME_LAMBDA:
      free_object(object->value.lambda.body);
      free_object(object->value.lambda.env);
      free_object(object->value.lambda.params);
      break;
    default:
      break; // SCHEME_NUMBER, SCHEME_BOOLEAN, SCHEME_PRIMITIVE and SCHEME_NIL
  }
  free(object);
}

// Prints the SchemeObject
void print_scheme_object(SchemeObject* object) {
  switch (object->type) {
    case SCHEME_NUMBER:
      printf("%f", object->value.number);
      break;

    case SCHEME_SYMBOL:
      printf("%s", object->value.symbol);
      break;

    case SCHEME_STRING:
      printf("\"%s\"", object->value.string);
      break;

    case SCHEME_PAIR:
      printf("(");
      print_scheme_object(object->value.pair.car);
      if (object->value.pair.cdr != NULL) {
        if (object->value.pair.cdr->type == SCHEME_PAIR) {
          printf(" ");
          print_scheme_object(object->value.pair.cdr);
        }
        else {
          printf(" . ");
          print_scheme_object(object->value.pair.cdr);
        }
      }
      printf(")");
      break;

    case SCHEME_PRIMITIVE:
      printf("<primitive>");
      break;

    case SCHEME_LAMBDA:
      printf("<lambda>");
      break;

    case SCHEME_BOOLEAN:
      printf(object->value.boolean ? "#t" : "#f");
      break;

    case SCHEME_NIL:
      printf("()");
      break;

    default:
      printf("<unknown type>");
      break;
  }
}
