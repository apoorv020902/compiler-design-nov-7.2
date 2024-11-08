# Compiler and Compilation Flags
CC = gcc
CFLAGS = -std=c99 -Wall

# Output Executable Name
EXEC = cooke_analyzer

# Source and Object Files
SRCS = front.c
OBJS = $(SRCS:.c=.o)

# Default Target to Build the Executable
all: $(EXEC)

# Rule to Link Object Files and Create the Executable
$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) -o $(EXEC) $(OBJS)

# Rule to Compile Source Files into Object Files
%.o: %.c front.h
	$(CC) $(CFLAGS) -c $< -o $@

# Clean-Up Target to Remove Generated Files
clean:
	rm -f $(OBJS) $(EXEC)
