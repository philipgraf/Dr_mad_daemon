
DEBUG = 1
CC=g++

debug: CPPFLAGS = -Wall -g -O0 -DDEBUG=$(DEBUG)

LDFLAGS = -lSDL -lSDL_image -lSDL_ttf -lSDL_gfx -lBox2D -lcwiid -lyaml-cpp -lSDL_mixer

RM = rm -rf
