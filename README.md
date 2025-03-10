# JVLISP

## Description

A mini ``Scheme`` interpreter in ``C`` heavily inspired by the metacircular evaluator from ``Structure and Interpretation of Computer Programs``.

----

## How to Compile

There are 3 possible ways of compiling the code:

- The files can be downloaded and compiled by running ``Makefile`` in your terminal;

- The files can be downloaded and compiled by running ``build.sh`` in your terminal;

- The files can be downloaded and compiled by running ``gcc -I./include -o jvlisp.exe src/*.c`` in your terminal.

-----

## How to Run

The ``jvlisp.exe`` file can be downloaded and ran directly since it is available in the releases tab.

If you opted for compiling the files, then you can simply run it by running ``jvlisp.exe`` in your terminal.

-----

## Features

By now, the interpreter is still in development. However, a few special forms and primitives are available:

- Special forms: ``define``, ``lambda``and ``if``;
- Primitives: ``+``, ``-``, ``*``, ``/``, ``=``, ``cons``, ``car`` and ``cdr``.

-----

## See Also

My other works around ``Scheme``'s metacircular evaluator can be found at [Metacircular-Evaluator](https://github.com/joaovictorlopezpereira/Metacircular-Evaluator).
