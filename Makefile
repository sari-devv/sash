CXX = g++
CXXFLAGS = -std=c++20 -Wall

TARGET = sash
SOURCES = main.cpp Parser.cpp
OBJECTS = $(SOURCES:.cpp=.o)

TEST_TARGET = test_parser
TEST_SOURCES = tests/TestParser.cpp Parser.cpp
TEST_OBJECTS = $(TEST_SOURCES:.cpp=.o)

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

clean:
	rm -f $(OBJECTS) $(TARGET) $(TEST_OBJECTS) $(TEST_TARGET)