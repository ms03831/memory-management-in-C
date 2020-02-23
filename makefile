all:
	gcc -o malloc.o -Wall Lec2_ms03831_A3_main.c Lec2_ms03831_A3_malloc.c 

run:
	./malloc.o

clean: 
	rm *.o
