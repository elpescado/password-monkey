#!/bin/bash

for test in test-*.c
do
	gcc -Wall -o `basename $test .c` ctest.c $test `pkg-config --cflags --libs glib-2.0`
done

