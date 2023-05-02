PROJECT 		:= OpenGL_Tutorial
TESTS			:= Tests

INSTALL_DIR 	:= ../$(PROJECT)
BUILD_DIR 		:= ../build
TEST_DIR 		:= ../build_tests
LCOV_DIR 		:= ../report
DOCUMENT_DIR 	:= ../Documentation
TAR_DIR 		:= ../$(PROJECT).tar.gz
LIBS_DIR 		:= ../s21_libs
SOURCES 		:= ./headers/*.h ./sources/*.cpp ./tests/*.cpp ./dependencies/*.h
CLEAN 			:= rm -rf

ifeq ($(shell uname), Darwin)
LEAKS 			:= leaks --atExit -- 
else
LEAKS 			:= valgrind --leak-check=full --show-leak-kinds=all -s
endif

all: run 

build:
	cmake -B $(BUILD_DIR)
	cmake --build $(BUILD_DIR)

run: build
	clear
	$(BUILD_DIR)/$(PROJECT)

clean: 
	$(CLEAN) $(BUILD_DIR)

.PHONY: all build run clean
