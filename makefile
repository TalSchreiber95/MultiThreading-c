


# CHECK ABOUT DEFAULT BUILD FOR MAKEFILE



run: solution.o
	gcc -o run solution.o -lm -lpthread -lrt

solution.o: solution.c
	gcc -g3 -c solution4.c -o solution.o


clean: 
	rm -f *.o run