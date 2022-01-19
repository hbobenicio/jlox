#!/bin/bash
set -eu -o pipefail

echo "=== main ==="
clang -Wall -Wextra -Wpedantic -g -O0 -fsanitize=address -o main             clox/*.c main.c
echo "ok."

echo "=== test-ast-printer ==="
clang -Wall -Wextra -Wpedantic -g -O0 -fsanitize=address -o test-ast-printer clox/*.c test-ast-printer.c
echo "ok."
