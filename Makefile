CXX := g++
CXXFLAGS := -std=c++17 -Wall -Iinclude

SRC := $(wildcard src/*.cpp)
OBJ := $(SRC:.cpp=.o)

TARGET := bin/test

$(TARGET): $(OBJ)
	@mkdir -p bin
	$(CXX) $(CXXFLAGS) -o $@ $^
	@rm -f $(OBJ)

src/%.o: src/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm -f src/*.o $(TARGET)
