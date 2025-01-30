# Compiler settings
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 
LDFLAGS = -ljansson -ljwt

# Directories
SRC_DIR = .
BIN_DIR = bin
JSON_PARSING_DIR = json_parsing
JWT_DIR = jwt
OBJ_DIR = obj
REQUEST_HANDLER_DIR = request_handler
ROUTER_DIR = router
SOCKET_DIR = socket

# Source files
SRCS = $(wildcard $(SRC_DIR)/*.c) $(wildcard $(SOCKET_DIR)/*.c) $(wildcard $(JSON_PARSING_DIR)/*.c) \
 $(wildcard $(REQUEST_HANDLER_DIR)/*.c) $(wildcard $(ROUTER_DIR)/*.c) $(wildcard $(JWT_DIR)/*.c)
OBJS = $(SRCS:%.c=$(OBJ_DIR)/%.o)

# Binary name
TARGET = $(BIN_DIR)/main.exe

# Create necessary directories
$(shell mkdir -p $(BIN_DIR) $(OBJ_DIR) $(OBJ_DIR)/$(SOCKET_DIR) $(OBJ_DIR)/$(JSON_PARSING_DIR) \
$(OBJ_DIR)/$(REQUEST_HANDLER_DIR) $(OBJ_DIR)/$(ROUTER_DIR) $(OBJ_DIR)/$(JWT_DIR)) 

# Default target
all: $(TARGET)

# Linking
$(TARGET): $(OBJS)
	$(CC) $(OBJS) $(LDFLAGS) -o $@

# Compilation
$(OBJ_DIR)/%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

# Clean and rebuild
rebuild: clean all

# Phony targets
.PHONY: all clean rebuild