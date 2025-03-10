# Variables
CC = gcc                     # Compiler
CFLAGS = -I./include -Wall   # Compilation flags
SRC = src/*.c                # All files.c in src
OBJ = $(SRC:.c=.o)           # .o files
TARGET = jvlisp.exe          # Executable name

# Rule to create the executable
$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $(TARGET)

# Rule to compile .c files into .o
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean the generated files
clean:
	rm -f $(OBJ) $(TARGET)
