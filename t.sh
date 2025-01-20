#!/bin/bash
if [ $# == 0 ]; then
    echo "no args"
    exit 1
fi

clang $1.c -o $1 

if [ $? == 0 ]; then
    ./$1
else
    echo bad input
fi