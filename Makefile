INCS = ./include
CFLAGS = -Wall

run : main.c $(INCS)/*.c
	gcc $^ -o $@ -I $(INCS) $(CFLAGS)

clean :
	rm run
