#ifndef MAIN_H
#define MAIN_H

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "SDL/SDL.h"

using namespace std;

typedef struct {
	double x;
	double y;
} Point;

typedef struct {
	int height;
	Uint32 color;
} Wall;

int projPlaneWidth = 480;
int projPlaneHeight = 320;
Point playerPos = {9, 8}; //player position vector
Point playerDir = {-1, 0}; //direction vector
Point cameraPlane = {0, 0.66}; //camera plane vector

const int mapWidth = 15;
const int mapHeight = 15;

int worldMap[mapWidth][mapHeight]=
{
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 3, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 3, 0, 3, 0, 0, 0, 0, 0, 2, 2, 0, 0, 1},
    {1, 0, 3, 0, 3, 0, 0, 0, 0, 0, 2, 2, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 2, 2, 0, 0, 0, 0, 0, 0, 0, 3, 3, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 3, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
};

SDL_Surface *screen;
SDL_Event event;

void setPixel(int x, int y, Uint32 pixel); //SDL helper function
Uint32 getWallColor(int x, int y);
int getHeightForWallDist(double dist);
Wall getWallForRay(Point rayPos, Point rayDir); //currently depends on room being fully enclosed by 4 walls
int playerInWall();

#endif