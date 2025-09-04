CXX = g++
CXXFLAGS = -g -std=c++20 -Wall -Werror

BUILD_DIR = build

TARGET = sash
SOURCES = main.cpp Parser.cpp Shell.cpp Command.cpp cd.cpp pwd.cpp History.cpp
OBJECTS = $(patsubst %.cpp, $(BUILD_DIR)/%.o, $(SOURCES))

TEST_TARGET = test_parser
TEST_SOURCES = tests/TestParser.cpp Parser.cpp Command.cpp cd.cpp pwd.cpp
TEST_OBJECTS = $(patsubst %.cpp, $(BUILD_DIR)/%.o, $(TEST_SOURCES))

ASAN_TARGET = sash_asan
ASAN_CXXFLAGS = $(CXXFLAGS) -fsanitize=address

all: $(BUILD_DIR)/$(TARGET)

$(BUILD_DIR)/$(TARGET): $(OBJECTS)
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(BUILD_DIR)/%.o: %.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/tests/TestParser.o: tests/TestParser.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -I. -c $< -o $@

test: $(BUILD_DIR)/$(TEST_TARGET)
	$(BUILD_DIR)/$(TEST_TARGET)

$(BUILD_DIR)/$(TEST_TARGET): $(TEST_OBJECTS)
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -I. -o $@ $^

asan: $(BUILD_DIR)/$(ASAN_TARGET)

$(BUILD_DIR)/$(ASAN_TARGET): $(OBJECTS)
	@mkdir -p $(@D)
	$(CXX) $(ASAN_CXXFLAGS) -o $@ $^

clean:
	rm -rf $(BUILD_DIR)
