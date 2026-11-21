#pragma once

// Includes
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "types.h"

// Functions Signatures
int are_objects_equal(SchemeObject* a, SchemeObject* b);
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

// Immutable singletons
static SchemeObject nil_singleton   = { .type = SCHEME_NIL };
static SchemeObject true_singleton  = { .type = SCHEME_BOOLEAN, .value.boolean = 1 };
static SchemeObject false_singleton = { .type = SCHEME_BOOLEAN, .value.boolean = 0 };

// Auxiliar function that compares structural equality between scheme objects
int are_objects_equal(SchemeObject* a, SchemeObject* b) {
  if (a == b) {
    return 1;
  }

  if (a == NULL || b == NULL) {
    return 0;
  }

  if (a->type != b->type) {
    return 0;
  }

  switch (a->type) {
    case SCHEME_NUMBER:
      return a->value.number == b->value.number;

    case SCHEME_BOOLEAN:
      return a->value.boolean == b->value.boolean;

    case SCHEME_SYMBOL:
      return strcmp(a->value.symbol, b->value.symbol) == 0;

    case SCHEME_STRING:
      return strcmp(a->value.string, b->value.string) == 0;

    case SCHEME_NIL:
      return 1;

    case SCHEME_PAIR:
      return are_objects_equal(a->value.pair.car, b->value.pair.car) &&
             are_objects_equal(a->value.pair.cdr, b->value.pair.cdr);

    default:
      return 0;
  }
}


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
SchemeObject* make_primitive(SchemeObject* (*func)(SchemeObject* args), const char* string) {
  SchemeObject* object = (SchemeObject*)malloc(sizeof(SchemeObject));
  object->type = SCHEME_PRIMITIVE;
  object->value.primitive.function = func;
  object->value.primitive.symbol = strdup(string);
  return object;
}

// Creates a Scheme lambda
SchemeObject* make_lambda(SchemeObject* params, SchemeObject* body, Environment* env) {
  SchemeObject* object = (SchemeObject*)malloc(sizeof(SchemeObject));
  object->type = SCHEME_LAMBDA;
  object->value.lambda.params = params;
  object->value.lambda.body = body;
  object->value.lambda.env = env;
  return object;
}

// Returns a reference to the singleton which represents a Scheme boolean
SchemeObject* make_boolean(const char* string) {
  if (strcmp(string, "#t") == 0) {
    return &true_singleton;
  }
  return &false_singleton;
}

// Returns a reference to the singleton which represents a Scheme nil
SchemeObject* make_nil() {
  return &nil_singleton;
}

// Frees the memory allocated by the SchemeObject
void free_scheme_object(SchemeObject* object) {
  if (object == NULL ||
      object == &nil_singleton ||
      object == &true_singleton ||
      object == &false_singleton) {
    return;
  }
  switch (object->type) {
    case SCHEME_SYMBOL:
      free(object->value.symbol);
      break;

    case SCHEME_STRING:
      free(object->value.string);
      break;

    case SCHEME_PRIMITIVE:
      free(object->value.primitive.symbol);
      break;

    case SCHEME_PAIR:
      free_scheme_object(object->value.pair.car);
      free_scheme_object(object->value.pair.cdr);
      break;

    case SCHEME_LAMBDA:
      free_scheme_object(object->value.lambda.params);
      free_scheme_object(object->value.lambda.body);
      break;

    default: // SCHEME_NUMBER and SCHEME_PRIMITIVE
      break; // These types do not allocate memory dynamically, so nothing to free
  }
  free(object);
}

// Prints the SchemeObject
void print_scheme_object(SchemeObject* object) {
  if (object == NULL){
    return;
  }
  switch (object->type) {
    case SCHEME_NUMBER:
      printf("%g", object->value.number);
      break;

    case SCHEME_SYMBOL:
      printf("%s", object->value.symbol);
      break;

    case SCHEME_STRING:
      printf("\"%s\"", object->value.string);
      break;

    case SCHEME_PAIR: {
      printf("(");
      SchemeObject* curr = object;
      while (curr->type == SCHEME_PAIR) {
        print_scheme_object(curr->value.pair.car);
        curr = curr->value.pair.cdr;
        if (curr == NULL) {
          break;
        }

        if (curr->type == SCHEME_PAIR) {
          printf(" ");
        }

        else if (curr->type != SCHEME_NIL) {
          printf(" . ");
          print_scheme_object(curr);
          break;
        }
      }
      printf(")");
      break;
    }

    case SCHEME_PRIMITIVE:
      printf("<primitive:%s>", object->value.primitive.symbol);
      break;

    case SCHEME_LAMBDA:
      printf("<lambda>");
      break;

    case SCHEME_BOOLEAN:
      printf(object->value.boolean == 1 ? "#t" : "#f");
      break;

    case SCHEME_NIL:
      printf("()");
      break;

    default:
      printf("<unknown>");
      break;
  }
}
