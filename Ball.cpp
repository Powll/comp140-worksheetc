#include "Ball.h"



Ball::Ball()
{
}


Ball::~Ball()
{
}
// side: 1 for right, -1 for left
void Ball::BallInit(int width, int height, int posX, int posY, int side = 1)
{
	ballPosition.h = height;
	ballPosition.w = width;
	ballPosition.x = posX;
	ballPosition.y = posY;

	xVel = side;
}

void Ball::invertXVel()
{
	xVel = -xVel;
}

void Ball::invertYVel()
{
	yVel = -yVel;
}

int Ball::getXVel()
{
	return xVel;
}

int Ball::getYVel()
{
	return yVel;
}

void Ball::moveX(float multiplier)
{
	ballPosition.x += (int)(xVel * multiplier);
}

void Ball::moveY(float multiplier)
{
	ballPosition.y += (int)(yVel * multiplier);
}

void Ball::resetBallPosition(int x, int y)
{
	ballPosition.x = x;
	ballPosition.y = y;
}

void Ball::setBallPaused(bool isPaused)
{
	// Switch out velocities when pausing/unpausing
	if (isPaused == true)
	{
		pausedXVel = xVel;
		pausedYVel = yVel;
		xVel = 0;
		yVel = 0;
	}
	else
	{
		xVel = pausedXVel;
		yVel = pausedYVel;
	}
}

SDL_Rect Ball::getRect()
{
	return ballPosition;
}
