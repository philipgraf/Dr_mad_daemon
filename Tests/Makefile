include ../source.mk
include ../flags.mk

TSOURCES = $(addprefix ../,$(SOURCES)) TileTest.cpp testrunner.cpp

COVFILE = app.info
COVDIR = coverage

LDFLAGS += -lcppunit
EXECUTABLE = testrunner

test: $(EXECUTABLE)
	$(CC) $(TSOURCES) -o $(EXECUTABLE) $(LDFLAGS) $(CFLAGS)
	
clean:
	$(RM) $(EXECUTABLE) *.gcno *.gcda data $(COVFILE) $(COVDIR) cppcheck.xml valgrind.xml

run: $(EXECUTABLE)
	./$(EXECUTABLE)
