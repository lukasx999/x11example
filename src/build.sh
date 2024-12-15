#!/usr/bin/env bash
set -euxo pipefail

CC=gcc
CFLAGS="-Wall -Wextra -ggdb -std=c99 -pedantic"
LIBS="-lX11 -lm"

FILES="main.c"

${CC} ${CFLAGS} ${LIBS} ${FILES} -o out
