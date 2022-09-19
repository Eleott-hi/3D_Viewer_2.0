CC=gcc
CFLAGS=-std=c11 -Wall -Werror -Wextra
PROJECT_NAME=s21_matrix
LIB_NAME=$(PROJECT_NAME).a
CLEAN=rm -rf
OPEN=

# ADDITIONAL
DEFINE=
TEST_FLAG=-fsanitize=address -g
ADDITIONAL=#$(TEST_FLAG)
SED_SH=#sed_sh

ifeq ($(shell uname), Linux)
	CHECK_LIBS=-lcheck -lm -lpthread -lsubunit -lrt
	LEAKS=valgrind $(BUILD_DIR)/test
else
	CHECK_LIBS=-lcheck -lm -lpthread
	LEAKS=CK_FORK=no leaks --atExit -- $(BUILD_DIR)/test
endif

# FOLDERS
SRC_DIR=.
TEST_DIR=$(SRC_DIR)/../tests
BUILD_DIR=$(SRC_DIR)/../build
MINIVERTER_DIR=$(SRC_DIR)/../materials/build

# SOURCES
SOURCES=*.c
HEADERS=*.h
OBJ=*.o
SH_SOURCES=*.sh
TXT_SOURCES=*.txt*


# GOALS_START
all: clean $(PROJECT_NAME) test gcov_report

rebuild: clean all

build: $(PROJECT_NAME)

$(PROJECT_NAME): $(LIB_NAME)

$(LIB_NAME): $(SRC_DIR)/$(SOURCES) $(SRC_DIR)/$(HEADERS)
	$(CC) $(ADDITIONAL) $(CFLAGS) -g -c $(SRC_DIR)/$(SOURCES)
	ar rc $(LIB_NAME) $(OBJ)
	$(CLEAN) $(OBJ) 

# TESTS
test: $(LIB_NAME)
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) $(ADDITIONAL) -g $(TEST_DIR)/tests.c -L. $(LIB_NAME) $(CHECK_LIBS) -o $(BUILD_DIR)/$@
	$(BUILD_DIR)/$@	

# COVERAGE
gcov_report: clean
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) $(ADDITIONAL) --coverage -c $(SRC_DIR)/$(SOURCES)
	ar rc $(LIB_NAME) $(OBJ)
	$(CC) $(CFLAGS) $(TEST_DIR)/tests.c -L. $(LIB_NAME) $(CHECK_LIBS) -lgcov -o $(BUILD_DIR)/test
	$(CLEAN) $(OBJ)
	$(BUILD_DIR)/test
	gcov *.gcda
	lcov -t "gcov_report" -o gcov_report.info -c -d .
	mkdir -p $(BUILD_DIR)/report
	genhtml -o $(BUILD_DIR)/report gcov_report.info

# CHECK
check_all: clean mini_build check

check: clean leaks cppcheck lint

lint:
	python3 $(SRC_DIR)/../materials/linters/cpplint.py --recursive --extensions=c,h $(SRC_DIR)/../src $(TEST_DIR)

cppcheck:
	cppcheck --std=c11 --enable=all --suppress=missingIncludeSystem  --suppress=unusedFunction ./

leaks: clean test
	$(LEAKS)

mini_build: $(SED_SH)
	bash $(SRC_DIR)/mini_build.sh $(MINIVERTER_DIR)

clean:
	$(CLEAN) $(OBJ) *.a *.gcno *.gcda *.gcov *.info
	$(CLEAN) $(BUILD_DIR)/$(OBJ) $(BUILD_DIR)/*.a $(BUILD_DIR)/test
	$(CLEAN) $(BUILD_DIR)/report/*.*
	$(CLEAN) $(BUILD_DIR)/report/src/*.*

sed_sh:
	sed -i 's/\r//g' $(SRC_DIR)/$(SH_SOURCES)
	sed -i 's/\r//g' $(MINIVERTER_DIR)/$(SH_SOURCES)
	sed -i 's/\r//g' $(MINIVERTER_DIR)/scripts/$(SH_SOURCES)
#	sed -i 's/\r//g' $(TEST_DIR)/$(SH_SOURCES)

Victor: clean 
	$(CC) $(ADDITIONAL) $(SRC_DIR)/$(SOURCES) -o main -lm
	./main && rm -rf main
