# JVLISP

## Description

A mini ``Scheme`` interpreter in ``C`` heavily inspired by the metacircular evaluator from ``Structure and Interpretation of Computer Programs``.

----

## How to Compile

There are 2 possible ways to compile the code:

1. **Using build.sh**:
   Run the `build.sh` script in your terminal.

2. **Using GCC directly**:
   Run the following command in your terminal:
   ```bash
   gcc -I./include -o jvlisp.exe src/*.c
   ```

-----

## How to Run

The ``jvlisp.exe`` file can be downloaded and run directly from the releases tab.

If you opted for compiling the files, you can run the compiled program with the following command:
```bash
./jvlisp.exe
```

-----

## Features

By now, the interpreter is still in development. However, a few special forms and primitives are available:

- Special forms: ``define``, ``lambda``, ``if`` and ``quote``;
- Primitives: ``+``, ``-``, ``*``, ``/``, ``=``, ``cons``, ``car`` and ``cdr``.
- Other features:  ``nil`` or ``'()``, recursion and dynamic scope.

-----

## See Also

My other works around ``Scheme``'s metacircular evaluator can be found at [Metacircular-Evaluator](https://github.com/joaovictorlopezpereira/Metacircular-Evaluator).
