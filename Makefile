INCS = ./
CFLAGS = -Wall

run : main.c squeue.c zem.c tpool.c
	gcc $^ -o $@ -I $(INCS) $(CFLAGS)
