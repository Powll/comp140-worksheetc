#include "Ball.h"



Ball::Ball()
{
}


Ball::~Ball()
{
}
// side: 1 for right, -1 for left
void Ball::BallInit(int width, int height, int posX, int posY, int side)
{
	ballPosition.h = height;
	ballPosition.w = width;
	ballPosition.x = posX;
	ballPosition.y = posY;

	yVel = side;
}
