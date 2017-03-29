CC = gcc

INC = -I ./lib
L = -L ./lib
## Objects that must be built in order to link
OBJECTS = bmp.o byte_op.o
libs = ./lib/bmp.c ./lib/byte_op.c

## Build
help:
	@echo "use \"make {filename}.a\" to complie .c to .hex in dir ./hex"

## lib OBJECTS Compile
bmp.o: ./lib/bmp.c
	${CC} $< ${INC} -c

byte_op.o: ./lib/byte_op.c
	${CC} $< ${INC} -c


%.o: %.c
	${CC} $< ${INC} -c

bmp2gray: bmp2gray.o ${OBJECTS}
	$(CC) -o $@ bmp2gray.o ${OBJECTS}

fliter: fliter.o ${OBJECTS}
	$(CC) -o $@ fliter.o ${OBJECTS}

.PHONY: clean
clean:
	-rm -rf *.o *.exe
