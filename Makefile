CXX := g++
CXXFLAGS := -std=c++17 -Wall -fPIC -Iinclude -Iinclude/qt

QT_CFLAGS := $(shell pkg-config --cflags Qt5Widgets)
QT_LIBS := $(shell pkg-config --libs Qt5Widgets)

SRC := $(wildcard src/*.cpp)
OBJ := $(SRC:.cpp=.o)

TARGET := bin/main

$(TARGET): $(OBJ)
	@mkdir -p bin
	$(CXX) $(CXXFLAGS) $(QT_CFLAGS) -o $@ $^ $(QT_LIBS)

src/%.o: src/%.cpp
	$(CXX) $(CXXFLAGS) $(QT_CFLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm -f src/*.o $(TARGET)
	rm -rf bin/