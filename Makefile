CC      = gcc
CFLAGS  = -Wall -Wextra -g

SRC     = main.c \
          tests/allocator_test.c \
          allocator/allocator.c

OBJ     = $(SRC:.c=.o)
TARGET  = webshell

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)
.PHONY: all clean