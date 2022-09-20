#################################################
##              Makefile template              ##
#################################################

#     These special targets are called phony     #
.PHONY: all build test gcov_report clean rebuild dvi install uninstall

#################### Exec file ###################
EXEC := 3dviewer

# Choosing a compiler and setting the main flags #
CC := gcc
STANDART :=-std=c11
CFLAGS := -Wall -Werror -Wextra $(STANDART)
LKFLAGS := -g -fprofile-arcs -ftest-coverage
CPPCHECKFLAG := --enable=all -$(STANDART)
VFLAG := --leak-check=full --show-leak-kinds=all -s
LDFLAGS := -lcheck

OS := $(shell uname)

ifeq ($(OS), Linux)
	CHECK_LIBS=-lcheck -lm -lpthread -lsubunit -lrt
	LEAKS=valgrind --leak-check=full --show-leak-kinds=all $(BUILD_DIR)/test_runner
else
	CHECK_LIBS=-lcheck -lm -lpthread
	LEAKS=CK_FORK=no leaks --atExit -- $(BUILD_DIR)/test_runner
endif


##################      Path     #################
P_SRC1 := ./matrix_algebra/
P_SRC2 := ./ 
P_OBJ := ./bin/
P_LINT := ../materials/linters/
SRC_DIR=.
TEST_DIR=../tests
BUILD_DIR=../build
SOURCES=*.c matrix_algebra/*.c
HEADERS=*.h matrix_algebra/*.h
OBJ=*.o
LIB_NAME=s21_matrix.a
LIB_DIR=matrix_algebra
GCOV_DIR=$(BUILD_DIR)/gcov
INSTALL_DIR=$(HOME)/3D_Viewer_1.0

all: install run

################## Style check ###################
lint:
	cp $(P_LINT)CPPLINT.cfg .
	python3 $(P_LINT)cpplint.py  *.c $(LIB_DIR)/*.c
	rm -rf CPPLINT.cfg
	
################## Leaks check ###################
leaks: test
	$(LEAKS)

############ Cleaning up unused files ############
clean:
	rm -rf ../build
	rm -rf *.gcno *.gcda *.info
	rm -rf *.a *.o
	rm -rf *.gcov
	rm -rf *.tgz

####### Template for creating object files #######
%.o : $(P_SRC)%.c
	$(CC) $(CFLAGS) -c $< -o $@ -I ./include

##############  Project archiving   ##############
dist: install
ifeq ($(uname_S), Linux)
	tar --create --file=$(INSTALL_DIR)/SmartCalc.tar --directory=$(INSTALL_DIR) 3D_Viewer_v1 models shaders.txt
else
#	tar --create --file=$(INSTALL_DIR)/SmartCalc.tar --directory=$(INSTALL_DIR) 3D_Viewer_v1.app models shaders.txt
	tar -czvf $(INSTALL_DIR)/3D_Viewer_v1.tar  --directory=$(INSTALL_DIR) 3D_Viewer_v1.app shaders.txt 
endif

dvi:
	open Readme.pdf

cppcheck:
	cppcheck --std=c11 --enable=all --suppress=missingIncludeSystem  --suppress=unusedFunction *.c $(LIB_DIR)/*.c

install:
	@mkdir -p $(BUILD_DIR)
	cd $(BUILD_DIR); qmake ../src; make
	@mkdir -p $(INSTALL_DIR)
	cp shaders.txt $(INSTALL_DIR)/shaders.txt
	@mkdir -p $(INSTALL_DIR)/models
	cp ../misc/obj/*.obj $(INSTALL_DIR)/models
ifeq ($(OS), Linux)
	cp -r $(BUILD_DIR)/3D_Viewer_v1 $(INSTALL_DIR)
else
	cp -r $(BUILD_DIR)/3D_Viewer_v1.app $(INSTALL_DIR)
endif
	
run: install
# open $(INSTALL_DIR)
	/Users/pintoved/3D_Viewer_1.0/3D_Viewer_v1.app/Contents/MacOS/3D_Viewer_v1
	
uninstall:
	rm -rf $(BUILD_DIR)
	rm -rf $(INSTALL_DIR)

test:
	@mkdir -p $(BUILD_DIR)
	cp ../misc/obj/cat.obj $(BUILD_DIR)
	$(CC) $(CFLAGS) -g $(TEST_DIR)/tests.c $(LIB_DIR)/*.c file_parcer.c $(CHECK_LIBS) -o $(BUILD_DIR)/test_runner
	cd $(BUILD_DIR); ./test_runner

gcov_report: clean
	@mkdir -p $(GCOV_DIR)
	cp ../misc/obj/cat.obj $(GCOV_DIR)
	@cd $(GCOV_DIR); $(CC) $(CFLAGS) -g --coverage ../$(TEST_DIR)/tests.c ../../src/$(LIB_DIR)/*.c ../../src/file_parcer.c $(CHECK_LIBS) -lgcov -o test_runner
	@cd $(GCOV_DIR); ./test_runner
	gcov *.gcda
	lcov -t "gcov_report" -o $(GCOV_DIR)/gcov_report.info -c -d $(GCOV_DIR)
	genhtml -o $(GCOV_DIR) $(GCOV_DIR)/gcov_report.info
	open $(GCOV_DIR)/index.html

pintoved:
	g++ test.cpp -o main
	./main
	rm -rf main