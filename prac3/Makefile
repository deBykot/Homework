CC = gcc
CFLAGS = -Wall 

all: utf16_to_utf8 utf8_to_utf16

utf16_to_utf8 : utf16_to_utf8.o common.o
	 $(CC) $(CFLAGS) -o utf16_to_utf8 utf16_to_utf8.o common.o

utf8_to_utf16 : utf8_to_utf16.o common.o
	 $(CC) $(CFLAGS) -o utf8_to_utf16 utf8_to_utf16.o common.o
	
utf16_to_utf8.o : utf16_to_utf8.c common.h
	 $(CC) $(CFLAGS) -c utf16_to_utf8.c

utf8_to_utf16.o : utf8_to_utf16.c common.h
	 $(CC) $(CFLAGS) -c utf8_to_utf16.c

common.o : common.c common.h
	$(CC) $(CFLAGS) -c common.c

clean:
	 rm -f *.o utf16_to_utf8 utf8_to_utf16
