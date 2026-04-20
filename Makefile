CXX := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -pedantic -Iinclude

SRC_DIR := src
BUILD_DIR := build
TARGET := $(BUILD_DIR)/student_management_system

SOURCES := $(SRC_DIR)/main.cpp $(SRC_DIR)/Student.cpp $(SRC_DIR)/Database.cpp $(SRC_DIR)/Utils.cpp

.PHONY: all clean run

all: $(TARGET)

$(TARGET): $(SOURCES)
	$(CXX) $(CXXFLAGS) $(SOURCES) -o $(TARGET)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)
