.PHONY: all build rebuild run dvi tests clean cppcheck style

APP = SmartCalc
APP_DIR = ./$(APP)
BUILD_DIR = ./build
TEST_BUILD_DIR=$(BUILD_DIR)/tests
OS=$(shell uname)
ifeq ($(OS), Linux)
	CHECK_LEAKS=CK_FORK=no valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --log-file=valgrind.log
	OPEN=xdg-open
	DIR=
	RUN_APP=./$(APP_DIR)/$(APP)
else
	CHECK_LEAKS=CK_FORK=no leaks --atExit --
	OPEN=open
	FILTER=--gtest_filter=-*.Exception*
	DIR=/$(APP).app
	RUN_APP=open $(APP_DIR)/$(APP).app
endif

all: build

rebuild: clean build

build:
	@cmake -S . -B $(BUILD_DIR)
	@cmake --build $(BUILD_DIR) --target SmartCalc

install: build uninstall
	@mkdir -p $(APP_DIR)
	@cp -r $(BUILD_DIR)$(DIR) $(APP_DIR)

dist: install
	tar -czvf $(APP_DIR).tgz $(APP_DIR)
	mv $(APP_DIR).tgz $(APP_DIR)

uninstall:
	@rm -rf $(APP_DIR)
	
run: install
	$(RUN_APP)

dvi:
	@mkdir -p ./build/docs
	doxygen ./docs/Doxyfile
	$(OPEN) $(BUILD_DIR)/docs/html/index.html

tests:
	@cmake -S ./tests -B $(TEST_BUILD_DIR)
	@cmake --build $(TEST_BUILD_DIR)
	@$(TEST_BUILD_DIR)/Tests

style: 	
	@clang-format -style=google -n -verbose */*.cc */*.h *.cc

cppcheck: build
	cmake --build $(BUILD_DIR) --target cppcheck

leaks: tests
	@$(CHECK_LEAKS) $(TEST_BUILD_DIR)/TESTS $(FILTER)

check: style cppcheck leaks

clean:
	@rm -rf $(BUILD_DIR) *.log
