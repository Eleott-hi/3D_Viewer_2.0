# @authors   pintoved (Pinto Veda)
#            barnards (Barnards Gibbering)

#------------------------------------------------#
#				   INGREDIENTS                   #
#------------------------------------------------#
# NAME           			project name
# VERSION        			build version
# OS             			operating system
# FILES          			all .cc .h files
# INSTALL_DIR    			installation path
# FILES          			executable file name
# CMAKE          			build system executable

NAME         	:= Viewer3D
VERSION      	:= 2.0

OS           	:= $(shell uname -s)
FILES        	:= $(shell find . \( -name "*.cc" -o -name "*.h" \) -type f)

BUILD_DIR     	:= ../build
INSTALL_DIR   	:= ../install

TEST_DIR 	  	:= ./tests
TEST_BUILD_DIR	:= ../build-test

ifeq ($(OS), Darwin)
APP		     	:= $(NAME).app
OPEN 	     	:= open 
else
APP		     	:= $(NAME)
OPEN	     	:= ./
endif

#------------------------------------------------#
#                    RECIPES                     #
#------------------------------------------------#
# all      			default goal
# build    			build program
# run				build and run program 
# install 			copy program to install dir
# uninstall			delete program from install dir
# clean    			remove binary
# dvi  				build documentation
# dist  			create .tar file of project
# linter   			run code style check
# fix-linter		run code style fix
# cppcheck 			run static code analys

all: run

run: build
	cd $(BUILD_DIR); $(OPEN)$(APP)

install: build
	mkdir -p $(INSTALL_DIR)
	cp -r $(BUILD_DIR)/$(APP) $(INSTALL_DIR)

uninstall:
	rm -rfv $(INSTALL_DIR)

dvi:
	texi2pdf docs.tex
	rm -rfv *.aux *.log *.dvi *.out 

dist: build dvi
	cp -r $(BUILD_DIR)/$(APP) ./$(APP)
	-tar -czvf $(NAME)-$(VERSION).tar.gz $(APP) */ *.cc *.h *.txt *.pdf Makefile					
	rm -rf $(APP)

build:
	cmake -S . -B $(BUILD_DIR)
	cmake --build $(BUILD_DIR)

tests:
	cmake -S $(TEST_DIR) -B $(TEST_BUILD_DIR)
	cmake --build $(TEST_BUILD_DIR)
	cd $(TEST_BUILD_DIR); ctest && make coverage

clean:
	rm -rf $(BUILD_DIR)* $(INSTALL_DIR) $(TEST_BUILD_DIR) logs/ \
	*.tar.gz *.aux *.log *.dvi *.pdf *.out *.toc  *.user *.info

linter: fix-linter
	clang-format -n -style=Google $(FILES)
	
fix-linter:
	clang-format -i -style=Google $(FILES)

cppcheck:
	cppcheck --language=c++ --std=c++17 \
	--enable=all 						\
	--suppress=missingInclude 			\
	--suppress=unusedFunction 			\
	--suppress=noExplicitConstructor 	\
	--suppress=selfAssignment		 	\
	$(FILES)

#------------------------------------------------#
#					  SPEC                       #
#------------------------------------------------#

.PHONY: all run install uninstall dvi dist build tests clean linter linter-fix cppcheck
.SILENT:
