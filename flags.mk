CC=g++
CFLAGS = -Wall -O0 -g -fprofile-arcs -ftest-coverage -DDEBUG
LDFLAGS = -lSDL -lSDL_image -lSDL_ttf -lgcov -coverage

RM = rm -rf
