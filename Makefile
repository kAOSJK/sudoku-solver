CC = gcc
CFLAGS = -std=c89 --pedantic -g3
RM = rm -rf

FILE = sudoku.c

valgrind: debug
	valgrind --leak-check=full --track-origins=yes --leak-check=full --show-leak-kinds=all ./debug grid_a.sudoku test

debug: sudoku-solver.o
	$(CC) ${CFLAGS} -o$@ $^

sudoku: sudoku-solver.o
	$(CC) ${CFLAGS} -o$@ $^ && make clear

sudoku-solver.o: ${FILE}
	$(CC) ${CFLAGS} -o$@ -c $^

clear:
	${RM} *.o

clean:
	${RM} *.o sudoku
