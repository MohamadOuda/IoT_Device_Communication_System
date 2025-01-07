# Main Directories
ROOT_DIR = ..
APP_DIR = $(ROOT_DIR)/Application
MODULES_DIR = $(ROOT_DIR)/Modules
# Modules Directories
MYSOCKET_DIR = $(MODULES_DIR)/MySocket
# Modules MakeFiles
MYSOCKET_MK = $(MYSOCKET_DIR)/my_socket.mk
# Application Directories
APP_OUT_DIR = $(APP_DIR)/out
APP_OBJ_DIR = $(APP_OUT_DIR)/gen
APP_LIB_DIR = $(APP_OUT_DIR)/lib
APP_BIN = $(APP_DIR)/client1


# Application Source files
APP_SRC = $(APP_DIR)/client.cpp
# Application Object files
APP_OBJ = $(APP_OBJ_DIR)/client.o

# Compiler and flags
CC = g++
CFLAGS = -Wall -std=c++17 #enable all warnings
LDFLAGS = -L$(APP_LIB_DIR) -lMySocket

all: my_socket $(APP_BIN)

my_socket: 
	$(MAKE) -f $(MYSOCKET_MK)

$(APP_BIN): $(APP_OBJ)
	$(CC) $(APP_OBJ) $(LDFLAGS) -o $@ 
# $@ gives target name

$(APP_OBJ): $(APP_SRC)
	mkdir -p $(APP_OBJ_DIR)
	$(CC) $(CFLAGS) -I$(MYSOCKET_DIR)/inc -c $(APP_SRC) -o $@

clean:
	$(MAKE) -f $(MYSOCKET_MK) clean
	rm -rf $(APP_OUT_DIR) $(APP_BIN)

.PHONY: clean all my_socket 
#In the Makefile you provided, you use .PHONY to declare that clean, all, my_socket are not actual files
