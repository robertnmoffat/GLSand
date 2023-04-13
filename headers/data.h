#ifndef DATA_H
#define DATA_H
#include <GL/glut.h>

enum sType{
	none,
	fire,
	oil,
	water,
	sand,
	solid
};

struct Colour{
	GLubyte r;
	GLubyte g;
	GLubyte b;
};

struct Sand{
	sType type;
	Colour colour;
	bool left = rand()%2;
	bool moving = true;
	bool phase=0;
	bool burning=0;
};

const Colour SAND_COLOUR = {212, 183, 106};
const Colour WATER_COLOUR = {30, 100, 117};
const Colour STONE_COLOUR = {50,50,50};
const Colour OIL_COLOUR = {10,10,10};
const Colour FIRE_COLOUR = {222, 11, 11};

const int width = 800;
const int height = 400;
#endif