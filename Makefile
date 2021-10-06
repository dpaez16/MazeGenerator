EXENAME=maze

CC=g++
CC_FLAGS=-O2 -Wall

SRC=src
SRCS=$(wildcard $(SRC)/*.cpp)
OBJS=$(patsubst $(SRC)/%.cpp, %.o, $(SRCS))

INCLUDE=include
INC=-I $(INCLUDE)

LIBPNG_OBJ=libpng-config --cflags
LIBPNG_FINAL=libpng-config --ldflags --libs

debug: CC_FLAGS=-g
debug: clean
debug: build

build: $(OBJS)
	$(CC) $(INC) $(CC_FLAGS) -o $(EXENAME) $(OBJS) `$(LIBPNG_FINAL)`

maze.o: $(SRC)/maze.cpp
	$(CC) $(INC) $(CC_FLAGS) -c $(<) `$(LIBPNG_OBJ)`

%.o: $(SRC)/%.cpp
		$(CC) $(INC) $(CC_FLAGS) -c $(<) -o $(@)

clean:
	rm -rf $(EXENAME) *.o *.gch *.png

