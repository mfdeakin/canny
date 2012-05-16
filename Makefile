
CFLAGS=-g
LIBS=-lglut

canny: canny.c canny.h gaussian.h
	$(CC) $(CFLAGS) $(LIBS) canny.c -o canny
