# JVLISP

## Description

A mini ``Scheme`` interpreter in ``C`` heavily inspired by the metacircular evaluator from ``Structure and Interpretation of Computer Programs``.

----

## How to Compile

There are 2 possible ways to compile the code depending on your operational system:

1. **Windows**:
   If you are using Windows, you can run the `build.bat` script or you can run
   ```bash
   gcc -I./include -o jvlisp.exe src/*.c
   ```
   directly in your terminal.

2. **Linux**:
   If you are using Linux, you can run the `build.sh` script or you can run
   ```bash
   gcc -I./include -o jvlisp.out src/*.c
   ```
   directly in your terminal.

-----

## How to Run

The ``jvlisp.exe`` file can be downloaded and run directly from the releases tab.

If you opted for compiling the files, you can run the compiled program with the following command:
```bash
./jvlisp.exe
```
or
```bash
./jvlisp.out
```
depending on which operational system the code was compiled.

-----

## Features

By now, the interpreter is still in development. However, a few special forms and primitives are available:

- Special forms: ``define``, ``lambda``, ``if`` and ``quote``;
- Primitives: ``+``, ``-``, ``*``, ``/``, ``=``, ``cons``, ``car`` and ``cdr``.
- Other features:  ``nil`` or ``'()``, recursion and dynamic scope.

-----

## See Also

One of my projects around ``Scheme``'s metacircular evaluator can be found at [Metacircular-Evaluator](https://github.com/joaovictorlopezpereira/Metacircular-Evaluator).
