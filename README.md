# JVLISP

## Description

A mini `Scheme` interpreter in `C` heavily inspired by the metacircular evaluator from *Structure and Interpretation of Computer Programs* (SICP).

----

## How to Compile

There are 2 possible ways to compile the code depending on your operating system:

1. **Windows**:
   Run the `build.bat` script or compile directly in your terminal:
   ```bash
   gcc -I./include -o jvlisp.exe src/*.c
   ```

2. **Linux**:
   Run the `build.sh` script or compile directly in your terminal:
   ```bash
   gcc -I./include -o jvlisp.out src/*.c
   ```

-----

## How to Run

The `jvlisp.exe` binary can be downloaded and run directly from the releases tab, or executed after compiling from source.

### Interactive REPL

Run the executable without arguments to start the interactive Read-Eval-Print Loop (type `(bye)` to exit):

```bash
./jvlisp.exe
```

### Running `.jlp` Files

Pass a `.jlp` source file as a command-line argument to evaluate all expressions sequentially:

```bash
./jvlisp.exe script.jlp
```

*(On Linux, replace `./jvlisp.exe` with `./jvlisp.out`.)*

-----

## Features

JVLISP is an interpreted Scheme dialect implemented in C. The engine implements core Lisp semantics, lexical closures, first-class procedures, and syntactic desugaring:

- **Special Forms & Syntactic Sugar:**
  - `define` (both variable bindings and abbreviated function definitions `(define (f x y) ...)`).
  - `lambda`, `if`, `begin`, `cond` (with `else` and multi-expression clauses), and `let`.
  - `quote` and the `'` reader macro shorthand (`'x`, `'(1 2 3)`).
- **Arithmetic:** `+`, `-`, `*`, `/` (supporting variadic arguments, unary negation, and unary reciprocal).
- **Numerical Comparison:** `=`, `<`, `>`, `<=`, `>=` (multi-argument monotonic checks).
- **Equality & Type Predicates:** `equal?` (deep structural equality across lists, strings, symbols, and atoms), `null?`, `pair?`, `number?`, `symbol?`, `boolean?`.
- **Pairs & Lists:** `cons`, `car`, `cdr`, `list`, and the empty list `'()`.
- **Data Types & Lexical Syntax:** Numbers (integers and floating-point), booleans (`#t`, `#f`), symbols, double-quoted strings (with `\"` and `\\` escapes), and `;` single-line comments.
- **I/O:** `display`.
- **Core Semantics:**
  - **Lexical Scope & Closures:** Lambdas capture their defining environment (`lambda.env`).
  - **First-Class Procedures:** Functions and primitives can be bound, passed as arguments, and returned from other procedures.
  - **Recursion:** Full support for direct recursive definitions.
- **Tooling:**
  - Interactive REPL and `.jlp` script execution.
  - Built-in automated test suite via the `(test-primitives)` primitive.

-----

## Example

```scheme
; Factorial using abbreviated define and cond
(define (factorial n)
  (cond ((<= n 1) 1)
        (else (* n (factorial (- n 1))))))

; Higher-order function and lexical closure
(define (make-multiplier factor)
  (lambda (x) (* x factor)))

(define (map f xs)
  (if (null? xs)
      '()
      (cons (f (car xs)) (map f (cdr xs)))))

(let ((nums (list 1 2 3 4))
      (triple (make-multiplier 3)))
  (display "Factorial of 5:" (factorial 5))
  (display "Tripled list:" (map triple nums))
  (display "Structural check:" (equal? '(3 6 9 12) (map triple nums))))
```

-----

## See Also

Another project around `Scheme`'s metacircular evaluator can be found at [Metacircular-Evaluator](https://github.com/joaovictorlopezpereira/Metacircular-Evaluator).