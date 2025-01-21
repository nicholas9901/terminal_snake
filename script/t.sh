#!/bin/bash
make

if [ $? == 0 ]; then
    ./snake
else
    echo bad input
fi