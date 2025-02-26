# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -pthread
LDFLAGS = -lgd

# Executable name
EXEC = old-photo-parallel-B

# Source files
SRCS = main.c old-photo-serial.c image-lib.c sort.c Handler.c Dynamic.c
OBJS = $(SRCS:.c=.o)

# Header files
HEADERS = old-photo-serial.h image-lib.h sort.h Handler.h Dynamic.h

# Default target
all: $(EXEC)

# Link object files to create the main executable
$(EXEC): main.o old-photo-serial.o image-lib.o sort.o Handler.o Dynamic.o
	$(CC) $(CFLAGS) -o $(EXEC) main.o old-photo-serial.o image-lib.o sort.o Handler.o Dynamic.o $(LDFLAGS)

# Compile main.c into main.o
main.o: main.c $(HEADERS)
	$(CC) $(CFLAGS) -c main.c -o main.o

# Compile old-photo-serial.c into old-photo-serial.o
old-photo-serial.o: old-photo-serial.c old-photo-serial.h image-lib.h
	$(CC) $(CFLAGS) -c old-photo-serial.c -o old-photo-serial.o

# Compile image-lib.c into image-lib.o
image-lib.o: image-lib.c image-lib.h
	$(CC) $(CFLAGS) -c image-lib.c -o image-lib.o

sort.o: sort.c sort.h Handler.h
	$(CC) $(CFLAGS) -c sort.c -o sort.o

Handler.o: Handler.c Handler.h Dynamic.h
	$(CC) $(CFLAGS) -c Handler.c -o Handler.o

Dynamic.o: Dynamic.c Dynamic.h 
	$(CC) $(CFLAGS) -c Dynamic.c -o Dynamic.o
# Clean up build artifacts
clean:
	rm -f $(OBJS)
clean_all:
	rm -fr ./Old-image-dir

valgrind:
	valgrind --leak-check=yes ./$(EXEC) ./Nova 10 -size

run:
	./$(EXEC) ./Nova/Dataset1 10 -size

cleanP:
	rm -rf Ficheiros-relatorio/Datasets-relatório/Dataset-A/old_photo_PAR_A/

# Phony targets
.PHONY: all clean cleanP
