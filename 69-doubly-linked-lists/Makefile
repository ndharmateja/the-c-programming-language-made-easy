CC = gcc
CFLAGS = -Wall -Wextra -g
TARGET = main
OBJS = main.o dll.o utils.o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS)

main.o: main.c dll.h utils.h
dll.o: dll.c dll.h
utils.o: utils.c dll.h utils.h

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET) *.o

valgrind: $(TARGET)
	valgrind --leak-check=full ./$(TARGET)
