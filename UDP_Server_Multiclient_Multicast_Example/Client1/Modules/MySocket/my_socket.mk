ROOT_DIR = ../
MODULES_DIR = $(ROOT_DIR)/Modules
MY_SOCKET_DIR = $(MODULES_DIR)/MySocket
MYSOCKET_INC_DIR = $(MY_SOCKET_DIR)/inc
MYSOCKET_SRC_DIR = $(MY_SOCKET_DIR)/src
MYSOCKET_OBJ_DIR = $(ROOT_DIR)/Application/out/gen
MYSOCKET_LIB_DIR = $(ROOT_DIR)/Application/out/lib

MYSOCKET_SRC = $(MYSOCKET_SRC_DIR)/TCPSocket.cpp $(MYSOCKET_SRC_DIR)/UDPSocket.cpp $(MYSOCKET_SRC_DIR)/ServerChannel.cpp $(MYSOCKET_SRC_DIR)/ClientChannel.cpp
MYSOCKET_OBJ = $(MYSOCKET_OBJ_DIR)/TCPSocket.o $(MYSOCKET_OBJ_DIR)/UDPSocket.o $(MYSOCKET_OBJ_DIR)/ServerChannel.o $(MYSOCKET_OBJ_DIR)/ClientChannel.o
# Static Library File
MYSOCKET_LIB = $(MYSOCKET_LIB_DIR)/libMySocket.a

# Compiler and flags
CC = g++
CFLAGS = -Wall -std=c++17

all: $(MYSOCKET_LIB)

$(MYSOCKET_OBJ_DIR)/%.o: $(MYSOCKET_SRC_DIR)/%.cpp
	mkdir -p $(MYSOCKET_OBJ_DIR)
	@echo "Compiling $< into $@"
	$(CC) $(CFLAGS) -I$(MYSOCKET_INC_DIR) -c $< -o $@

# $< refers to the first prerequisite of the rule, which in this case is a .cpp source file.
# $@ refers to the target of the rule, which is the corresponding .o object file.

$(MYSOCKET_LIB): $(MYSOCKET_OBJ)
	mkdir -p $(MYSOCKET_LIB_DIR)
	ar rcs $(MYSOCKET_LIB) $(MYSOCKET_OBJ)

#This rule creates the static library (libMySocket.a) by combining the object files.
#ar rcs: The archiver command to create a static library.
#r inserts the object files into the archive.
#c creates the archive if it doesn’t exist.
#s writes an index into the archive.

clean:
	rm -rf $(MYSOCKET_OBJ) $(MYSOCKET_LIB)

.PHONY: all clean