PROJ=l2
CC=g++
CFLAGS=-Wall -std=c++17
DEPS= safer.h util_bmp.h util.h
OBJ= main.o safer.o util_bmp.o util.o

all: $(PROJ)

%.o : %.cpp %(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(PROJ): $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

