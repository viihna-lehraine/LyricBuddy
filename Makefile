# Compiler and Flags
CXX = g++
CXXFLAGS = -Wall -std=c++20 -D_GNU_SOURCE -D_POSIX_C_SOURCE_200809L -Wunused-include
LDFLAGS = -lcurl -ljsoncpp -lstdc++fs

# Target and Sources
TARGET = build/LyricBuddy
SRC = src/Main.cpp src/modules/EnvParser.cpp src/modules/NetworkUtils.cpp src/modules/Utilities.cpp
INCLUDE = src/headers/

# Build Rules
all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -I$(INCLUDE) -o $@ $^ $(LDFLAGS)

clean:
	rm -f $(TARGET)
