
CFLAGS=-g
LIBS=-lglut

canny: canny.c gaussian.h
	$(CC) $(CFLAGS) $(LIBS) canny.c -o canny
