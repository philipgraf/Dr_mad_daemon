CC=g++
CFLAGS = -c -Wall
LDFLAGS = -lSDL -lSDL_image
SOURCES = Entity.cpp Event.cpp Game.cpp main.cpp Tools.cpp
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


