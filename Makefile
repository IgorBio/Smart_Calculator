.PHONY: all build rebuild run dvi tests clean cppcheck style

BUILD_DIR=./build
TEST_BUILD_DIR=$(BUILD_DIR)/tests
OS=$(shell uname)
ifeq ($(OS), Linux)
	OPEN=xdg-open
else
	OPEN=open
endif


all: build

build:
	@cmake -S . -B $(BUILD_DIR)
	@cmake --build $(BUILD_DIR) --target SmartCalc

run: build
	./$(BUILD_DIR)/SmartCalc

dvi:
	@mkdir -p ./build/docs
	@doxygen ./docs/Doxyfile
	@$(OPEN) $(BUILD_DIR)/docs/html/index.html

tests:
	@cmake -S ./tests -B $(TEST_BUILD_DIR)
	@cmake --build $(TEST_BUILD_DIR)
	@$(TEST_BUILD_DIR)/Tests

style: 	
	@clang-format -style=google -n -verbose */*.cc  */*.h

cppcheck:
	@cmake -S . -B $(BUILD_DIR)
	@cmake --build $(BUILD_DIR) --target CPPCHECK

clean:
	@rm -rf $(BUILD_DIR) *.log
