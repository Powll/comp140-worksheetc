#pragma once

#include <SDL.h>

class Ball
{
public:
	Ball();
	~Ball();

	void BallInit(int width, int height, int posX, int posY, int side);
	
private:
	SDL_Rect ballPosition;
	int xVel, yVel;
};

