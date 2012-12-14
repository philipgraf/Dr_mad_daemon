CC=g++
CFLAGS = -c -Wall -O0 -g
LDFLAGS = -lSDL -lSDL_image -lSDL_ttf
SOURCES = Entity.cpp Event.cpp Game.cpp Tools.cpp Level.cpp Tile.cpp
HEADERS = $(SOURCES:.cpp=.h) define.h
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

game:$(HEADERS) $(SOURCES) $(EXECUTABLE) 

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) main.cpp -o $@ $(LDFLAGS) 

$(SOURCES):
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


$(TEST_EXECUTABLE): $(TEST_OBJECTS) $(OBJECTS)
	$(CC) $(TEST_OBJECTS) $(OBJECTS) -o $@ $(TEST_LDFLAGS)


