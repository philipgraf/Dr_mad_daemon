CC=g++
CFLAGS = -c -Wall -O0 -g
LDFLAGS = -lSDL -lSDL_image -lSDL_ttf
SOURCES = Entity.cpp Event.cpp Game.cpp main.cpp Tools.cpp Level.cpp Tile.cpp
OBJECTS = $(SOURCES:.cpp=.o)
EXECUTABLE = DrInSane


all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@ $(LDFLAGS) 

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm $(OBJECTS) $(EXECUTABLE)

run: DrInSane
	./$(EXECUTABLE)


