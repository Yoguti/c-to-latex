# C to LaTeX Transpiler

![Language](https://img.shields.io/badge/language-C-blue.svg)
![Build](https://img.shields.io/badge/build-passing-brightgreen.svg)

A lightweight transpiler written in **C** that converts C source code into formatted LaTeX. Designed to assist in embedding code into academic papers

## Key Features
* **Zero Dependencies:** Built with standard C libraries only.
* **Custom Lexer:** Handwritten lexical analyzer (state machine based) for accurate tokenization.
* **Syntax Support:** Handles preprocessor directives (`#define`, `#include`), pointer arithmetic, bitwise operators, and string literals.
* **Line Tracking:** Precise error reporting with line-number context.

