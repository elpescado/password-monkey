CFLAGS=`pkg-config --cflags glib-2.0` -DUSE_OPENSSL
LDFLAGS=`pkg-config --libs glib-2.0`  -lcrypto

all: pwmonkey

pwmonkey: pwmonkey.o main.o
