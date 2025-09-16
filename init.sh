#!/bin/bash
export PATH_BUILD="$(realpath ./build)"
export PATH_ROOT="$(pwd)"
export EXE="terminal_snake"

alias e='stty echo'
t () { make -C $PATH_ROOT && $PATH_BUILD/$EXE; }
v () { valgrind --leak-check=full --track-origins=yes --log-file=$PATH_ROOT/vout $PATH_BUILD/$EXE; cat $PATH_ROOT/vout > $1; }
d () { sudo gdb -p $(pidof $EXE); }
