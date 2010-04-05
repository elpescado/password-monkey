CFLAGS=-Wall `pkg-config --cflags glib-2.0`  -I../cli
LDFLAGS=-Wall `pkg-config --libs glib-2.0`  

all: test-cli

test-cli: ../cli/pwmonkey.o test-cli.o ctest.o csv.o
