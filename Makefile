CXX      := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -O2
TARGET   := smart-task-manager
SRCS     := main.cpp Task.cpp TaskManager.cpp FileManager.cpp
OBJS     := $(SRCS:.cpp=.o)

# Default build
all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

# Ensure the data directory exists
data/tasks.txt:
	mkdir -p data

# Remove build artifacts
clean:
	rm -f $(OBJS) $(TARGET)

# Rebuild from scratch
rebuild: clean all

.PHONY: all clean rebuild
