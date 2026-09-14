#pragma once

#include <stdio.h>
#include <string.h>
#include "types.h"

// Auxiliar function that compares structural equality between scheme objects
int are_objects_equal(SchemeObject* a, SchemeObject* b) {
  if (a == b) return 1;
  if (a == NULL || b == NULL) return 0;
  if (a->type != b->type) return 0;

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
      return are_objects_equal(a->value.pair.car, b->value.pair.car) && are_objects_equal(a->value.pair.cdr, b->value.pair.cdr);

    default:
      return 0;
  }
}

// Evals a string of Scheme code in a given environment
static SchemeObject* eval_string(const char* code, Environment** env) {
  Token* tokens = tokenize(code);
  Token* original = tokens;
  SchemeObject* expr = parser(&tokens);
  SchemeObject* result = eval(expr, env);
  free_tokens(original);
  return result;
}

// Structure of tests
typedef struct {
  const char* expr;
  const char* expected;
} TestCase;

// Primitive executed by the REPL
SchemeObject* primitive_test_primitives(SchemeObject* args) {
  TestCase tests[] = {
    // Arithmetic
    { "(+ 1 2 3)", "6" },
    { "(- 10 3 2)", "5" },
    { "(- 5)", "-5" },
    { "(* 2 3 4)", "24" },
    { "(/ 12 3)", "4" },
    { "(/ 2)", "0.5" },

    // Types predicates
    { "(null? (quote ()))", "#t" },
    { "(null? 1)", "#f" },
    { "(pair? (cons 1 2))", "#t" },
    { "(pair? (quote ()))", "#f" },
    { "(number? 42)", "#t" },
    { "(number? #t)", "#f" },
    { "(boolean? #t)", "#t" },
    { "(boolean? 0)", "#f" },
    { "(symbol? (quote x))", "#t" },

    // Numerical comparison
    { "(= 5 5 5)", "#t" },
    { "(= 5 4)", "#f" },
    { "(< 1 2 3)", "#t" },
    { "(< 3 2 1)", "#f" },
    { "(> 3 2 1)", "#t" },
    { "(<= 1 1 2)", "#t" },
    { "(>= 2 2 1)", "#t" },

    // Lists
    { "(car (cons 10 20))", "10" },
    { "(cdr (cons 10 20))", "20" },

    // Ifs and Lambdas
    { "(if #t 10 20)", "10" },
    { "(if #f 10 20)", "20" },
    { "((lambda (x) (* x x)) 5)", "25" },
    { "((lambda (x y) (+ x y)) 3 4)", "7" },
  };

  int total = sizeof(tests) / sizeof(TestCase);
  int passed = 0;

  printf("\n-8<--------Executing-Primitive-Functions-Tests-------->8-\n\n");

  Environment* shared_env = init_environment();
  for (int i = 0; i < total; i++) {

    SchemeObject* actual = eval_string(tests[i].expr, &shared_env);
    SchemeObject* expected = eval_string(tests[i].expected, &shared_env);

    if (are_objects_equal(actual, expected)) {
      printf("  [PASS] %-40s => ", tests[i].expr);
      print_scheme_object(actual);
      printf("\n");
      passed++;
    }
    else {
      printf("  [FAIL] %-40s => Got: ", tests[i].expr);
      print_scheme_object(actual);
      printf(" | Expected: ");
      print_scheme_object(expected);
      printf("\n");
    }
  }
  free_environment(shared_env);

  printf("\n-8<--------------------------------------------------->8-\n");
  printf("              Result: %d/%d tests successful.\n", passed, total);
  printf("-8<--------------------------------------------------->8-\n\n");

  return make_boolean(passed == total ? "#t" : "#f");
}