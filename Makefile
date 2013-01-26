include source.mk

include flags.mk

HEADERS = $(SOURCES:.cpp=.h) $(addprefix $(SRCDIR), define.h)
OBJECTS = $(SOURCES:.cpp=.o)
EXECUTABLE = DrInSane

CFLAGS += -c

all: game

game:$(HEADERS) $(SOURCES) $(EXECUTABLE) 

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) $(SRCDIR)main.cpp -o $@ $(LDFLAGS) 

$(SOURCES):
	$(CC) -o $@ $(CFLAGS) 

clean:
	$(RM) $(OBJECTS) $(EXECUTABLE) *.gcno *.gcda

run: $(EXECUTABLE)
	./$(EXECUTABLE)
	


