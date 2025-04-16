CC = gcc
CFLAGS = -Wall -Wextra -g
TARGET = main
OBJS = main.o dll.o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS)

main.o: main.c dll.h
dll.o: dll.c dll.h

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET) *.o

valgrind: $(TARGET)
	valgrind --leak-check=full ./$(TARGET)
