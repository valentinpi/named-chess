# General

CXX      		:= g++
TARGET   		:= named-chess

# Build

# Debug
CXXFLAGS 		:= -g -O0 -std=c++17 -Wall
#CXXFLAGS 		:= -g -O2 -std=c++17 -Wall
# Release
#CXXFLAGS 		:= -mwindows -O2 -std=c++17 -Wall
# Remember to set the DEBUG macro in Utils.hpp according to the build

# Dependencies

LINKS    		:= -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_net -lSDL2_ttf
STATIC_LINKS	:= -static-libgcc -static-libstdc++

all: $(TARGET) clean

$(TARGET): $(patsubst src/%.cpp, obj/%.o, $(wildcard src/*.cpp))
	windres -o obj/named-chess.res -O coff res/named-chess.rc
	$(CXX) $(CXXFLAGS) -o bin/$(TARGET) $(wildcard obj/*.o) obj/named-chess.res $(LINKS) $(STATIC_LINKS)

obj/%.o: src/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

clean:
	del /Q obj\*.o
	del /Q obj\*.res
