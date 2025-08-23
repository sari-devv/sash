CXX = g++
CXXFLAGS = -g -std=c++20 -Wall -Werror

TARGET = sash
SOURCES = main.cpp Parser.cpp Shell.cpp Command.cpp cd.cpp pwd.cpp
OBJECTS = $(SOURCES:.cpp=.o)

TEST_TARGET = test_parser
TEST_SOURCES = tests/TestParser.cpp Parser.cpp Command.cpp cd.cpp pwd.cpp
TEST_OBJECTS = $(TEST_SOURCES:.cpp=.o)

ASAN_TARGET = sash_asan
ASAN_CXXFLAGS = $(CXXFLAGS) -fsanitize=address

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJECTS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

tests/TestParser.o: tests/TestParser.cpp
	$(CXX) $(CXXFLAGS) -I. -c $< -o $@

test: $(TEST_TARGET)
	./$(TEST_TARGET)

$(TEST_TARGET): $(TEST_OBJECTS)
	$(CXX) $(CXXFLAGS) -I. -o $(TEST_TARGET) $(TEST_OBJECTS)

asan: $(ASAN_TARGET)

$(ASAN_TARGET): $(OBJECTS)
	$(CXX) $(ASAN_CXXFLAGS) -o $(ASAN_TARGET) $(OBJECTS)

clean:
	rm -f $(OBJECTS) $(TARGET) $(TEST_OBJECTS) $(TEST_TARGET) $(ASAN_TARGET)
