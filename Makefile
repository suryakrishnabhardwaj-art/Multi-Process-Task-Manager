CC = gcc
CFLAGS = -Wall -Wextra -O2

TARGET = task_manager
SOURCE = task_manager.c

all: $(TARGET)

$(TARGET): $(SOURCE)
	$(CC) $(CFLAGS) $(SOURCE) -o $(TARGET)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)

.PHONY: all run clean
