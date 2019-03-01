#pragma once
#include "SDL.h"
#include <serial\serial.h>
#include "SerialInterface.h"
#include "Ball.h"
#include <dos.h>

class Game
{
public:
	Game();
	~Game();

	bool init(const char* title, int xpos, int ypos, int width, int height, int flags);
	void render();
	void update();
	void handleEvents();
	void clean();

	bool initSerialConnection();
	bool running() { return isRunning; };

private:
	SDL_Window* mainWindow;
	SDL_Renderer* mainRenderer;
	bool isRunning;

	int checkCollision(SDL_Rect, SDL_Rect);

	int windowHeight; // used for clamping player movement
	int windowWidth;

	int p1Score, p2Score;

	// Clamps position values from Arduino to the screen
	int mapToScreen(int, int, int, int, int);

	void resetGame();

	SerialInterface* serial;

	// I do not believe a separate class would have been necessarily needed for the players, given all we need is to send in movement commands
	SDL_Rect playerPosition;
	SDL_Rect player2Position;

	// Given the nature of the ball, a separate class is used
	Ball ball;
	
	// Multiplier used for increasing difficulty after each hit
	float ballSpeedMultiplier = 1.0;
};

