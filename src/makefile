TARGET = main
CC = mpicc
CFLAGS = -Wall -lm -O3
SRC = main.c maxHeap.c knn.c arrayUtils.c 

$(TARGET): $(SRC)
	$(CC) $(SRC) -o $(TARGET) $(CFLAGS)

clean:
	rm -f $(TARGET)
