
CFLAGS=-g
LIBS=-lglut

canny: canny.c
	$(CC) $(CFLAGS) $(LIBS) canny.c -o canny
