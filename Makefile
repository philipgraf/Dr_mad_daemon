CC=g++
CFLAGS = -c -Wall -O0 -g
LDFLAGS = -lSDL -lSDL_image -lSDL_ttf
SOURCES = Entity.cpp Event.cpp Game.cpp main.cpp Tools.cpp Level.cpp Tile.cpp
HEADERS = Entity.h Event.h Game.h Tools.h Level.h Tile.h define.h
OBJECTS = $(SOURCES:.cpp=.o)
EXECUTABLE = DrInSane

RM := rm -rf

TEST_DIR = Tests
TEST_SOURCES = TileTest.cpp testrunner.cpp

TEST_DS = $(addprefix $(TEST_DIR)/,$(TEST_SOURCES))
TEST_OBJECTS = $(TEST_DS:.cpp=.o)
TEST_LDFLAGS = $(LDFLAGS) -lcppunit
TEST_EXECUTABLE = $(TEST_DIR)/testrunner


all: game test

game: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@ $(LDFLAGS) 

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	$(RM) $(OBJECTS) $(EXECUTABLE)

cleanTest:
	$(RM) $(TEST_OBJECTS) $(TEST_EXECUTABLE)

run: $(EXECUTABLE)
	./$(EXECUTABLE)
	
runTest: $(TEST_EXECUTABLE)
	./$(TEST_EXECUTABLE)

test: $(TEST_DS) $(TEST_EXECUTABLE)


$(TEST_EXECUTABLE): $(TEST_OBJECTS)
	$(CC) $(TEST_OBJECTS) -o $@ $(TEST_LDFLAGS)


