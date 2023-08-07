## Makefile by phyliscr [2023]

DEFAULT=\033[0m   # Сброс цвета

WHITE_FG=\033[37m 
RED_FG=\033[31m 
GREEN_FG=\033[32m 
BLUE_FG=\033[34m 
PURPLE_FG=\033[35m 
BLACK_FG=\033[30m 

WHITE_BG=\033[47m 
RED_BG=\033[41m 
GREEN_BG=\033[42m 
BLUE_BG=\033[44m 
PURPLE_BG=\033[45m 
BLACK_BG=\033[40m 

## Variables Block
CC = gcc
CCFLAGS = -Werror -Wall -Wextra -std=c11
COVERAGE = 
LIBS = -lpthread -lcheck -lm
LEAK = -fsanitize=leak -fsanitize=address -fsanitize=undefined -fsanitize=unreachable -fno-sanitize-recover -fstack-protector -fanalyzer

## Structure Block
QT_PROJECT = 3d_viewer_v1
QT_PROJECT_FOLDER = interface

SOURCE_FOLDER = base
SOURCES = $(wildcard $(SOURCE_FOLDER)/*.c)
OBJECTS = $(patsubst %.c,%.o,$(notdir $(SOURCES)))

TARGET = base.a
LIBS_FOLDER = libs

TESTS_FILENAME = test
TESTS_FOLDER = tests
TESTS_SOURCES = $(wildcard $(TESTS_FOLDER)/*.c)
TESTS_OBJECTS = $(patsubst %.c,%.o,$(notdir $(TESTS_SOURCES)))

ifeq ($(shell uname -s), Linux)
	LIBS = -lpthread -lcheck -lsubunit -lm
endif

## Application Block
.PHONY: all clean run install uninstall dist dvi test gcov_report debug add_debug add_coverage 

all: clean check install dvi

clean: 
	@rm -rfv libs/*.a valgrind.log tests/test report/* 
	@rm -rfv *.gcda *.gcno *.o 
	@rm -rfv temp_root build-* makefile_generated.mk moc_* ui_* $(QT_PROJECT)
	@rm -rfv doxygen
	@rm -rfv $(QT_PROJECT).tar.gz qrc_resources.cpp .qmake.stash
	
makefile_generated.mk: $(QT_PROJECT_FOLDER)/$(QT_PROJECT).pro
	@echo "\n$(BLACK_FG)$(WHITE_BG)--------------- INSTALL -------------- $(DEFAULT)\n"
	qmake -o makefile_generated.mk $(QT_PROJECT_FOLDER)/$(QT_PROJECT).pro

install: makefile_generated.mk
	make -f makefile_generated.mk install INSTALL_ROOT=temp_root

uninstall: makefile_generated.mk
	make -f makefile_generated.mk uninstall INSTALL_ROOT=temp_root

dvi:
	@rm -rfv html latex
	@doxygen Doxyfile
	@-open doxygen/html/index.html

dist: clean
	tar -cvf $(QT_PROJECT).tar *
	gzip $(QT_PROJECT).tar

run_test: test
	@echo "\n$(WHITE_FG)$(RED_BG)--------------- RUN TESTS -------------- $(DEFAULT)\n"
	$(TESTS_FOLDER)/$(TESTS_FILENAME)
	@echo "\n$(WHITE_FG)$(RED_BG)---------------------------------------- $(DEFAULT)\n"

$(TARGET): 
	$(CC) $(CCFLAGS) $(DEBUG) $(COVERAGE) -c $(SOURCES) 
	ar rc $(LIBS_FOLDER)/$@ $(OBJECTS)
	ranlib $(LIBS_FOLDER)/$@
	@rm -rfv *.o


test: $(TARGET)
	$(CC) $(CCFLAGS) -c $(TESTS_SOURCES)
	$(CC) $(CCFLAGS) $(DEBUG) $(COVERAGE) $(TESTS_OBJECTS) $(LIBS) $(LIBS_FOLDER)/$(TARGET) -o $(TESTS_FOLDER)/$@
	@rm -rfv *.o

debug: add_debug test

gcov_report: add_coverage test run_test
	mkdir -p report
	lcov -t "test" -o report/test.info -c -d .
	genhtml -o report report/test.info
	@rm -rfv *.gcda *.gcno

add_coverage:
	$(eval COVERAGE += --coverage)

add_debug:
	$(eval DEBUG += -g -fdiagnostics-color=always)

## Check Block
.PHONY: check clang cppcheck cpplint leaks leaks_debug gcc_leaks
check: clean gcov_report clang cppcheck leaks 

clang:
	@echo "\n$(BLACK_FG)$(PURPLE_BG)------------- CLANG-FORMAT ------------- $(DEFAULT)\n"
	@echo "clang-format: Error."
	@clang-format -style=Google -n $(SOURCE_FOLDER)/*.c $(TESTS_FOLDER)/*.c $(QT_PROJECT_FOLDER)/*.cpp
	@echo "clang-format: Correction."
	@clang-format -style=Google -i $(SOURCE_FOLDER)/*.c $(TESTS_FOLDER)/*.c $(QT_PROJECT_FOLDER)/*.cpp
	@echo "clang-format: Check."
	@clang-format -style=Google -n $(SOURCE_FOLDER)/*.c $(TESTS_FOLDER)/*.c $(QT_PROJECT_FOLDER)/*.cpp

cppcheck:
	@echo "\n$(BLACK_FG)$(GREEN_BG)-------------- CPPCHECK ---------------- $(DEFAULT)\n"
	cppcheck --enable=all --suppress=missingIncludeSystem  $(SOURCE_FOLDER) $(TESTS_FOLDER)

leaks:
	@echo "\n$(BLACK_FG)$(BLUE_BG)-------------- VALGRIND ---------------- $(DEFAULT)\n"
	valgrind --leak-check=full --log-file=valgrind.log $(TESTS_FOLDER)/test
	@echo "\n$(BLACK_FG)$(BLUE_BG)-------- VALGRIND RESULT FILTER -------- $(DEFAULT)\n"
	grep "ERROR SUMMARY:" valgrind.log

leaks_debug: ## If a leak is detected !!!
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes \
			 --verbose $(TESTS_FOLDER)/test 

## https://github.com/google/sanitizers/wiki/AddressSanitizerComparisonOfMemoryTools
gcc_leaks: clean
	$(CC) $(CCFLAGS) -c $(C_FILE)
	-ggdb -g -fsanitize=leak -fsanitize=address -fsanitize=undefined \
	-fsanitize=unreachable -fno-sanitize-recover -fstack-protector -fanalyzer \
