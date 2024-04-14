# Makefile for compiling main.cpp with SFML, including headers from inc directory
# Source files are in the src directory

# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -std=c++11 -Iinclude

# SFML libraries
LIBS = -lsfml-graphics -lsfml-window -lsfml-system

# Executable name
TARGET = sfml.o

# Source files directory
SRCDIR = src

# Object files directory
OBJDIR = obj

# Source files
SRCS = $(wildcard $(SRCDIR)/*.cpp)

# Object files
OBJS = $(patsubst $(SRCDIR)/%.cpp,$(OBJDIR)/%.o,$(SRCS))

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LIBS)

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	$(CXX) $(CXXFLAGS) -o $@ -c $<

clean:
	rm -f $(OBJS) $(TARGET)
