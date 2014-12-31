CXX = g++
CXXFLAGS = -g -Wall -std=c++1y -O0
LDFLAGS = -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer
SOURCES := $(wildcard src/*.cpp)
OBJECTS := $(patsubst %.cpp,%.o,${SOURCES})

td : ${OBJECTS}
	$(CXX) $(CXXFLAGS) -o td $(OBJECTS) $(LDFLAGS)
%.o : %.cpp
	$(CXX) -c $(CXXFLAGS) $< -o $@
