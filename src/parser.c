
// Includes
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include "primitives.c"
#include "tokenizer.c"


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
      printf("Error: there's a left parenthesis missing.");
      return NULL;

    case TOKEN_BOOLEAN:
      expression = make_boolean(current_token->value);
      break;

    case TOKEN_NUMBER:
      expression = make_number(strtod(current_token->value, NULL));
      break;

    case TOKEN_NIL:
      expression = make_nil();
      break;

    case TOKEN_SYMBOL:
      expression = make_symbol(current_token->value);
      break;

    case TOKEN_EOF:
      expression = make_nil();
      break;

    case TOKEN_STRING:
      expression = make_string(current_token->value);
      break;

    default:
      printf("Error: unexpected token.");
      return NULL;
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
    printf("Error: expected closing parenthesis.");
    return NULL;
  }

  return head->value.pair.cdr;
}
