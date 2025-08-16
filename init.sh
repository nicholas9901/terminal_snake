#!/bin/bash
export PATH_BUILD="$(realpath ./build)"
export PATH_ROOT="$(pwd)"

alias e='stty echo'
t () { make -C $PATH_ROOT && $PATH_BUILD/snake; }
v () { valgrind --leak-check=full --track-origins=yes --log-file=$PATH_ROOT/vout $PATH_BUILD/snake; }
