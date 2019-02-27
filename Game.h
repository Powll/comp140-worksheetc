#pragma once
#include "SDL.h"
#include <serial\serial.h>
#include "SerialInterface.h"
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

	void setBallPaused(bool);

	int checkCollision(SDL_Rect, SDL_Rect);

private:
	SDL_Window* mainWindow;
	SDL_Renderer* mainRenderer;
	bool isRunning;

	int windowHeight; // used for clamping player movement
	int windowWidth;
	void resetBallPosition();

	int p1Score, p2Score;

	// Clamps position values from Arduino to the screen
	int mapToScreen(int, int, int, int, int);

	void resetGame();

	SerialInterface* serial;

	SDL_Rect playerPosition;
	SDL_Rect player2Position;

	SDL_Rect ballPosition;
	int xVel = 3, yVel = 3;
	int pausedXVel = 3, pausedYVel = 3;
	float ballSpeedMultiplier = 1.0;
};

