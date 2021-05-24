


# CHECK ABOUT DEFAULT BUILD FOR MAKEFILE



run: solution.o
	gcc -o solution solution.o -lm -lpthread

solution.o: solution.c
	gcc -c solution.c -o solution.o

clean: 
	rm -f *.o solution