# Define the compiler
CXX = g++

# Define compiler flags (e.g., C++11 standard)
CXXFLAGS = -std=c++11 -Wall

# Define the target executable
TARGET = cpu

# List of source files
SRCS = main.cpp cpu_core.cpp address_logic.cpp

# List of object files (replacing .cpp with .o)
OBJS = $(SRCS:.cpp=.o)

# Default rule to build the program
all: $(TARGET)

# Rule to link the object files and create the executable with -Wl,--subsystem,console
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS) -Wl,--subsystem,console

# Rule to compile source files into object files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean rule to remove compiled files
clean:
	del /Q $(OBJS) $(TARGET)