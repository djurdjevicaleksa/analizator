CXX := g++
CXXFLAGS := -std=c++17 -Wall -fPIC -Iinclude -Iinclude/qt

LIB_DIR := -Llib/qt
QT_LIBS := -lQt5Widgets -lQt5Gui -lQt5Core

SRC := $(wildcard src/*.cpp)
OBJ := $(SRC:.cpp=.o)

TARGET := bin/main

$(TARGET): $(OBJ)
	@mkdir -p bin
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LIB_DIR) $(QT_LIBS) -Wl,-rpath=lib/qt

src/%.o: src/%.cpp
	$(CXX) $(CXXFLAGS) $(QT_CFLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm -f src/*.o $(TARGET)
	rm -rf bin/
