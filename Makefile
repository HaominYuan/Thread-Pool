INCS = ./
CFLAGS = -Wall

run : main.c squeue.c zemaphore.c
	gcc $^ -o $@ -I $(INCS) $(CFLAGS)
