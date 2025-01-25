CC= gcc#compiler

TARGET = navigate #target file name

all:
	$(CC) -Wall -std=c99 -O3 -o $(TARGET) navigate.c FILE_handling.c ALGORITHM_handling.c DATA_handling.c MATRIX_handling.c auxiliaries.c navigate.h

clean:
	rm $(TARGET)
