opt=0
CC = gcc
CFLAGS = -Wall
BUILD_DIR=build
TARGETS=print_opt $(BUILD_DIR) $(BUILD_DIR)/main/main.o build_executable

ifeq ($(opt),0)
    CFLAGS += -g -Wextra
else
    CFLAGS += -O3
endif

.phony: all
all: $(TARGETS)

$(BUILD_DIR)/main/main.o: src/main/main.c
	mkdir -p $(BUILD_DIR)/main
	$(CC) $(CFLAGS) -c src/main/main.c -o $(BUILD_DIR)/main/main.o

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

build_executable:
	$(CC) $(CFLAGS) $(BUILD_DIR)/main/main.o -o $(BUILD_DIR)/main.out

print_opt:
	@echo "Current opt value is: $(opt)"

clean:
	rm -rf ${BUILD_DIR}

run: $(BUILD_DIR)/main.out
	./$(BUILD_DIR)/main.out