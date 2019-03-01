#pragma once

#include <SDL.h>

class Ball
{
public:
	Ball();
	~Ball();

	// Initialise ball properties
	void BallInit(int width, int height, int posX, int posY, int side);

	// Invert velocities when hitting something
	void invertXVel();
	void invertYVel();

	int getXVel();
	int getYVel();

	// Movement functions
	void moveX(float);
	void moveY(float);

	// Reset ball to given position
	void resetBallPosition(int, int);

	// Pause/Unpause ball
	void setBallPaused(bool);

	SDL_Rect getRect();
	
private:
	SDL_Rect ballPosition;

	// Velocities for x and y movement
	int xVel = 1, yVel = 1;

	// Storage variables for when the ball is paused (velocities = 0)
	int pausedXVel, pausedYVel;
	
};

