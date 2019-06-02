TARGET   		:= stats-viewer
CXX      		:= g++
CXXFLAGS 		:= -Wall -std=c++17 -O2
STATIC_LINKS	:= -static-libgcc -static-libstdc++

$(TARGET): src/main.cpp
	$(CXX) $(CXXFLAGS) -o $(TARGET) src/main.cpp $(STATIC_LINKS)
