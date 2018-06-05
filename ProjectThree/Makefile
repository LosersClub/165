CC=g++
CFLAGS=-std=c++0x -Wall -O2
BIN_DIR = bin
OBJ_DIR = obj

LZSRC = file.cpp window.cpp bit_stream.cpp hash_encoder.cpp compress.cpp
EXPANDSRC = file.cpp window.cpp bit_stream.cpp expand.cpp
LZOBJ = $(LZSRC:%.cpp=$(OBJ_DIR)/%.o)
EXPANDOBJ = $(EXPANDSRC:%.cpp=$(OBJ_DIR)/%.o)

all: dir LZ EXPAND

LZ: $(LZOBJ)
	$(CC) $(CLFAGS) $(LZOBJ) -o $@

EXPAND: $(EXPANDOBJ)
	$(CC) $(CFLAGS) $(EXPANDOBJ) -o $@

$(OBJ_DIR)/%.o: %.cpp
	$(CC) $(CFLAGS) -o $@ -c $<

dir:
	mkdir -p $(OBJ_DIR)

.PHONY: dir all