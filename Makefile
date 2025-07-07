# Compiler and flags
CC       := gcc
CFLAGS   := -Wall -Wextra -std=c11 -g
LDFLAGS  := -lm

# Output
OUTPUT   := paging

# Sources and objects
SRC      := paging.c
OBJ      := $(SRC:.c=.o)

# Default target (build everything)
all: $(OUTPUT)

# Link step
$(OUTPUT): $(OBJ)
	$(CC) $(OBJ) -o $@ $(LDFLAGS)

# Compile step
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Run with arguments (usage: make run ARGS="1024 12 64 p1.proc p2.proc")
run: $(OUTPUT)
	./$(OUTPUT) $(ARGS)

# Clean up build artifacts
clean:
	rm -f $(OBJ) $(OUTPUT)

.PHONY: all run clean
