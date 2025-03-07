#pragma once

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include "object.h"
#include "tokenizer.h"
#include "primitives.h"

// Functions Signatures
SchemeObject* parse_list(Token** tokens);
SchemeObject* parser(Token** tokens);

// Parses a list of given tokens
SchemeObject* parser(Token** tokens) {
  SchemeObject* expression;
  Token* current_token = *tokens;

  (*tokens)++;

  switch (current_token->type) {
    case TOKEN_LEFTPAREN:
      expression = parse_list(tokens);
      break;

    case TOKEN_RIGHTPAREN:
      printf("Error: there's a right parenthesis missing.\n");
      exit(1);

    case TOKEN_NUMBER:
      expression = make_number(strtod(current_token->value, NULL));
      break;

    case TOKEN_SYMBOL:
    SchemeObject* (*primitive_function)(SchemeObject* args) = get_primitive_function(current_token->value);
    if (primitive_function != NULL) {
      expression = make_primitive(primitive_function);
    }
    else {
      expression = make_symbol(current_token->value);
    }
    break;

    case TOKEN_EOF:
      expression = make_nil();
      break;

    default:
      printf("Error: Unexpected token.\n");
      exit(1);
  }

  return expression;
}

// Parses a Scheme list
SchemeObject* parse_list(Token** tokens) {
  Token* current_token = *tokens;

  SchemeObject* head = make_nil();
  SchemeObject* tail = head;

  while (current_token->type != TOKEN_RIGHTPAREN && current_token->type != TOKEN_EOF) {
    SchemeObject* element = parser(tokens);
    SchemeObject* pair = make_pair(element, make_nil());
    tail->value.pair.cdr = pair;
    tail = pair;
    current_token = *tokens;
  }

  if (current_token->type == TOKEN_RIGHTPAREN) {
    (*tokens)++;
  }
  else if (current_token->type == TOKEN_EOF) {
    printf("Error: expected closing parenthesis.\n");
    exit(1);
  }

  return head->value.pair.cdr;
}


