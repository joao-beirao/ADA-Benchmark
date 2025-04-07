# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -g  # Enable warnings and debug symbols

# Project files
SRCS = test.c unzipper.c delete.c run_program.c test.c    # List all .c files
OBJS = $(SRCS:.c=.o)       # Convert .c to .o (object files)
TARGET = tester        # Output executable name

# Default target (run with 'make')
all: $(TARGET)

# Link object files to create executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

# Compile each .c file to .o
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean build artifacts (run with 'make clean')
clean:
	rm -f $(OBJS) $(TARGET)

# Rebuild everything (run with 'make rebuild')
rebuild: clean all