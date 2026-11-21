
// Includes
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include "primitives.c"
#include "tokenizer.c"

// Functions Signatures
static SchemeObject* parse_list(Token** tokens);
static SchemeObject* parse_quote(Token** tokens);
SchemeObject* parser(Token** tokens);

// Parses a Scheme list
static SchemeObject* parse_list(Token** tokens) {

  // Base case: empty list
  if ((*tokens)->type == TOKEN_RIGHTPAREN) {
    (*tokens)++;
    return make_nil();
  }

  SchemeObject* head = NULL;
  SchemeObject** curr = &head;

  while ((*tokens)->type != TOKEN_RIGHTPAREN && (*tokens)->type != TOKEN_EOF) {
    SchemeObject* element = parser(tokens);

    // Syntax error
    if (element == NULL) {
      return NULL;
    }

    *curr = make_pair(element, make_nil());
    curr = &((*curr)->value.pair.cdr);
  }

  if ((*tokens)->type == TOKEN_RIGHTPAREN) {
    (*tokens)++;
  }

  else {
    printf("Error: expected closing parenthesis.\n");
    return NULL;
  }

  return head;
}

// Parses a quote reader macro ('expr -> (quote expr))
static SchemeObject* parse_quote(Token** tokens) {
  if ((*tokens)->type == TOKEN_EOF) {
    printf("Error: expected expression after quote.\n");
    return NULL;
  }

  SchemeObject* quoted_expr = parser(tokens);
  if (quoted_expr == NULL) {
    return NULL;
  }

  return make_pair(make_symbol("quote"), make_pair(quoted_expr, make_nil()));
}

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
      printf("Error: there's a left parenthesis missing.\n");
      return NULL;

    case TOKEN_BOOLEAN:
      expression = make_boolean(current_token->value);
      break;

    case TOKEN_NUMBER:
      expression = make_number(strtod(current_token->value, NULL));
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

    case TOKEN_QUOTE:
      expression = parse_quote(tokens);
      break;

    default:
      printf("Error: unexpected token.\n");
      return NULL;
  }
  return expression;
}
