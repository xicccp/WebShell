CC      = gcc
CFLAGS  = -Wall -Wextra -g

SRC     = main.c                          \
          tests/allocator_test.c           \
          allocator/allocator.c            \
          server/http.c                    \
          server/router.c                  \
          server/socket.c                  \
          benchmarks/benchmark_menu.c      \
          benchmarks/bench_allocator.c     \
          benchmarks/bench_http.c          \
          benchmarks/bench_shell.c         \
          benchmarks/bench_kv.c            \
          benchmarks/bench_compressor.c

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