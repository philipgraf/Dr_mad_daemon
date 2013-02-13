include source.mk

include flags.mk

HEADERS = $(SOURCES:.cpp=.h) $(addprefix $(SRCDIR), define.h)
OBJECTS = $(SOURCES:.cpp=.o)
EXECUTABLE = DrInSane


all: game

game:$(HEADERS) $(SOURCES) $(EXECUTABLE) 

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) $(SRCDIR)main.cpp -o $@ $(LDFLAGS) 

$(OBJECTS): $(HEADERS)

clean:
	$(RM) $(OBJECTS) $(EXECUTABLE) *.gcno *.gcda

run: $(EXECUTABLE)
	./$(EXECUTABLE)

debug: game