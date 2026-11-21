#pragma once

// Includes
#include <stdio.h>
#include <string.h>
#include "types.h"

// Functions Signatures
int are_objects_equal(SchemeObject* a, SchemeObject* b);
static SchemeObject* eval_string(const char* code, Environment** env);
SchemeObject* primitive_test_primitives(SchemeObject* args);

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

  if (tokens == NULL) {
    return NULL;
  }

  Token* original = tokens;
  SchemeObject* expr = parser(&tokens);
  SchemeObject* result = eval(expr, env);
  free_tokens(original);
  return result;
}

// Structure of tests
typedef struct {
  const char* name;
  const char* expr;
  const char* expected;
} TestCase;

// Primitive executed by the REPL
SchemeObject* primitive_test_primitives(SchemeObject* args) {
  TestCase tests[] = {
    // Arithmetic
    {"+ with 3 arguments", "(+ 1 2 3)", "6" },
    {"- with 3 arguments", "(- 10 3 2)", "5" },
    {"- with 1 argument", "(- 5)", "-5" },
    {"* with 3 arguments", "(* 2 3 4)", "24" },
    {"/ with 2 arguments", "(/ 12 3)", "4" },
    {"/ with 1 argument", "(/ 2)", "0.5" },
    {"Nested arithmetic expression", "(* 2 5 (+ 1 2 3 0))", "60" },

    // Types predicates
    {"null? in empty list", "(null? (quote ()))", "#t" },
    {"null? in nil", "(null? '())", "#t"},
    {"null? in number", "(null? 1)", "#f" },
    {"pair? in cons", "(pair? (cons 1 2))", "#t" },
    {"pair? in empty list", "(pair? (quote ()))", "#f" },
    {"number? in number", "(number? 42)", "#t" },
    {"number? in bool", "(number? #t)", "#f" },
    {"boolean? in #t", "(boolean? #t)", "#t" },
    {"boolean? in #f", "(boolean? #f)", "#t" },
    {"boolean? in number", "(boolean? 0)", "#f" },
    {"symbol? in symbol", "(symbol? (quote x))", "#t" },
    {"symbol? in number", "(symbol? 2)", "#f" },

    // Numerical comparison
    {"= with 2 arguments", "(= 5 4)", "#f" },
    {"= with 3 arguments", "(= 5 5 5)", "#t" },
    {"< with 2 arguments", "(< 1 2)", "#t" },
    {"< with 3 arguments", "(< 3 2 1)", "#f" },
    {"> with 2 arguments", "(> 3 2)", "#t" },
    {"<= with 3 arguments", "(<= 1 1 2)", "#t" },
    {">= with 3 arguments", "(>= 2 2 1)", "#t" },

    // Lists
    {"car of a pair", "(car (cons 10 20))", "10" },
    {"cdr of a pair", "(cdr (cons 10 20))", "20" },
    {"car of the cdr of a list", "(car (cdr (cons 10 (cons 20 '()))))", "20" },

    // Ifs and Lambdas
    {"if with #t", "(if #t 10 20)", "10" },
    {"if with #f", "(if #f 10 20)", "20" },
    {"if with actual predicate", "(if (> 10 5) 1 2)", "1"},
    {"lambda with 1 argument", "((lambda (x) (* x x)) 5)", "25" },
    {"lambda with 2 arguments", "((lambda (x y) (+ x y)) 3 4)", "7" },

    // Define and Environment
    {"Variable definition", "(define x 2)", "(quote x)" },
    {"Variable lookup", "x", "2" },
    {"Variable evaluation in expression", "(* x 10)", "20" },
    {"Procedure with internal define", "(define g (lambda () (define pi 3) pi))", "(quote g)" },
    {"Local scope and sequence execution", "(g)", "3"},

    // Lexical Scope, Closures and Environment Stress Tests
    {
      "Lexical scoping preserves defining environment",
      "((lambda () (define base 10) (define add-base (lambda (y) (+ base y))) ((lambda (base) (add-base 1)) 99)))",
      "11"
    },
    {
      "Closures maintain independent state frames",
      "((lambda () (define make-adder (lambda (n) (lambda (v) (+ n v)))) (define add5 (make-adder 5)) (define add20 (make-adder 20)) (+ (add5 2) (add20 2))))",
      "29"
    },
    {
      "Deep nested parameter shadowing",
      "((((lambda (a) (lambda (b) (lambda (a) (+ a b)))) 1) 2) 3)",
      "5"
    },
    {
      "Closure passed as argument ignores caller bindings",
      "((lambda () (define x 42) (define reader (lambda () x)) ((lambda (y z) (z)) 999 reader)))",
      "42"
    },
    {
      "Curried procedure chain evaluation",
      "((((lambda (x) (lambda (y) (lambda (z) (* (+ x y) z)))) 2) 3) 4)",
      "20"
    },
    {
      "Sequence evaluation with multiple internal defines",
      "((lambda () (define a 10) (define b 20) (+ a b)))",
      "30"
    },
    {
      "Parameter shadows outer procedure name",
      "((lambda () (define op (lambda (n) (* n 2))) ((lambda (op) (+ op 5)) 10)))",
      "15"
    },

    // Begin
    {
      "begin evaluates sequence and returns last value",
      "(begin 1 2 3)",
      "3"
    },
    {
      "begin evaluates sequence with internal definitions",
      "(begin (define a 10) (define b 20) (+ a b))",
      "30"
    },
    {
      "Nested begin expressions",
      "(begin (begin (begin 42)))",
      "42"
    },

    // Let
    {
      "let binds single variable",
      "(let ((x 10)) (* x 2))",
      "20"
    },
    {
      "let binds multiple variables simultaneously",
      "(let ((x 2) (y 3)) (+ x y))",
      "5"
    },
    {
      "let shadows outer variable within its body",
      "((lambda () (define x 1) (let ((x 10)) x)))",
      "10"
    },
    {
      "let body does not leak bindings to outer environment",
      "((lambda () (define x 1) (let ((x 10)) x) x))",
      "1"
    },
    {
      "let bindings evaluate in outer environment (not let*)",
      "((lambda () (define x 5) (let ((x 2) (y x)) (+ x y))))",
      "7"
    },
    {
      "let supports multi-expression body",
      "(let ((x 10)) (define y 5) (+ x y))",
      "15"
    },

    // Cond
    {
      "cond executes first matching branch",
      "(cond ((= 1 1) 10) (else 20))",
      "10"
    },
    {
      "cond falls through to subsequent branch",
      "(cond ((= 1 2) 10) ((= 2 2) 20) (else 30))",
      "20"
    },
    {
      "cond falls through to else clause",
      "(cond ((= 1 2) 10) ((= 3 4) 20) (else 3))",
      "3"
    },
    {
      "cond treats zero as truthy (Scheme semantics)",
      "(cond (0 10) (else 0))",
      "10"
    },
    {
      "cond supports multi-expression consequence",
      "(cond ((= 1 1) (define a 10) (+ a 5)) (else 0))",
      "15"
    },

    // Quote
    {
      "Quote shortcut on symbol",
      "'x",
      "(quote x)"
    },
    {
      "Quote shortcut on list",
      "(car '(10 20 30))",
      "10"
    },
    {
      "Nested quote shortcuts",
      "''x",
      "(quote (quote x))"
    },

    // List
    {
      "list with no arguments",
      "(list)",
      "'()"
    },
    {
      "list with evaluated arguments",
      "(list 1 (+ 1 1) 3)",
      "'(1 2 3)"
    },
    {
      "nested list calls",
      "(list (list 1 2) 'a)",
      "'((1 2) a)"
    },

    // Abbreviated define
    {
      "Abbreviated define with 0 parameters",
      "(begin (define (const-five) 5) (const-five))",
      "5"
    },
    {
      "Abbreviated define with multiple parameters",
      "(begin (define (soma-quad a b) (+ (* a a) (* b b))) (soma-quad 3 4))",
      "25"
    },
    {
      "Abbreviated define supports recursion and multi-expression body",
      "(begin (define (fat n) (define zero 0) (if (<= n zero) 1 (* n (fat (- n 1))))) (fat 5))",
      "120"
    },

    // equal?
    {
      "equal? on identical lists",
      "(equal? '(1 (2 3)) (list 1 (list 2 3)))",
      "#t"
    },
    {
      "equal? on different lists",
      "(equal? '(1 2) '(1 3))",
      "#f"
    },
    {
      "equal? on strings",
      "(equal? \"jvlisp\" \"jvlisp\")",
      "#t"
    },
    {
      "equal? on symbols",
      "(equal? 'abc 'abc)",
      "#t"
    },
  };

  int total = sizeof(tests) / sizeof(TestCase);
  int passed = 0;

  printf("\n-8<--------Executing-Primitive-Functions-Tests-------->8-\n\n");

  Environment* shared_env = init_environment();
  for (int i = 0; i < total; i++) {

    // Evaluates the test expression
    SchemeObject* actual = eval_string(tests[i].expr, &shared_env);

    // Evaluates the expected test result (be careful with tests that should return symbols)
    SchemeObject* expected = eval_string(tests[i].expected, &shared_env);

    if (are_objects_equal(actual, expected)) {
      printf("  [PASS] %s\n", tests[i].name);
      passed++;
    }
    else {
      printf("  [FAIL] %s\n", tests[i].name);
      printf("         Expr:     %s\n", tests[i].expr);
      printf("         Got:      ");
      print_scheme_object(actual);
      printf("\n         Expected: ");
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