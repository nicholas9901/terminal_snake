#!/bin/bash
# v.sh: run valgrind and log the output
valgrind --leak-check=full --track-origins=yes --log-file=$PATH_ROOT/vout $PATH_BUILD/snake