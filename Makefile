CC = gcc
FLAGS = -Wall

program:
	$(CC) $(FLAGS) const.c -o const
	$(CC) $(FLAGS) filter.c -o filter
	$(CC) $(FLAGS) spawn.c -o spawn
	$(CC) $(FLAGS) window.c -o window
	$(CC) $(FLAGS) controlador.c -o controlador
