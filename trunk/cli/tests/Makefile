CFLAGS=`pkg-config --cflags glib-2.0` -DUSE_OPENSSL -I..
LDFLAGS=`pkg-config --libs glib-2.0`  -lcrypto

all: test-pwmonkey

test-pwmonkey: ../pwmonkey.o test-pwmonkey.o ctest.o
