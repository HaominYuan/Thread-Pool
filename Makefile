INCS = ./squeue.h
CFLAGS = -Wall


main : main.c squeue.c
	gcc $^ -o $@ -I $(INCS) $(CFLAGS)
