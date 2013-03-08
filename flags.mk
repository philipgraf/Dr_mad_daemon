
DEBUG = 3
CC=g++

debug: CPPFLAGS = -Wall -g -O0 -DDEBUG=$(DEBUG)
all: CPPFLAGS = -g

LDFLAGS = -lSDL -lSDL_image -lSDL_ttf -lSDL_gfx -lBox2D -lcwiid -lyaml-cpp -lSDL_mixer -lboost_filesystem -lboost_system

RM = rm -rf
