#include "stdafx.h"
#include "Game.h"
#include <iostream>

using std::cout;

/*
* Constructor - not used in this case
*
*/
Game::Game()
{
	
}

/*
	Decontructor - also not used in this case
*/
Game::~Game()
{
}

bool Game::initSerialConnection()
{
	serial = new SerialInterface();

	return true;
}

int Game::checkCollision(SDL_Rect A, SDL_Rect Ball)
{
	// Rectangle sides
	int leftA, leftB;
	int rightA, rightB;
	int topA, topB;
	int bottomA, bottomB;

	// Calculate sides of rect A
	leftA = A.x;
	rightA = A.x + A.w;
	topA = A.y;
	bottomA = A.y + A.h;

	// Calculate sides of rect A
	leftB = Ball.x;
	rightB = Ball.x + Ball.w;
	topB = Ball.y;
	bottomB = Ball.y + Ball.h;

	if ((leftB >= leftA && leftB <= rightA) || (rightB >= leftA && rightB <= rightA))
	{
		if ((topB >= topA && topB <= bottomA) || (bottomB >= topA && bottomB <= bottomA))
		{
			// Return 1 for collision
			return 1;
		}
	}

	// Return 0 if no collision is detected
	return 0;
}

/*
* init - used to initialise and setup SDL
* Return true if everything is successful
*/
bool Game::init(const char * title, int xpos, int ypos, int width, int height, int flags)
{
	// initialise SDL
	if (SDL_Init(SDL_INIT_EVERYTHING) == 0) {
		cout << "SDL init success \n";

		// Create a window
		mainWindow = SDL_CreateWindow(title, xpos, ypos, width, height, flags);

		// if window succesful..
		if (mainWindow != 0) {
			cout << "Window creation success \n";

			// create renderer
			mainRenderer = SDL_CreateRenderer(mainWindow, -1, 0);

			// if renderer successful...
			if (mainRenderer != 0) {
				cout << "Renderer creation success \n";
				SDL_SetRenderDrawColor(mainRenderer, 255, 255, 255, 255);
			}
			else {
				cout << "renderer failed \n";
				return false;
			}
		}
		else {
			cout << "window failed \n";
			return false;
		}

	}
	else {
		cout << "SDL fail \n";
		return false;
	}

	isRunning = true;
	cout << "SDL init success \n";

	windowHeight = height;
	windowWidth = width;

	initSerialConnection();

	// Setup players and ball
	playerPosition.x = 100;
	playerPosition.y = mapToScreen(serial->getPot1(), 0, 1023, 0, windowHeight - int(playerPosition.h));
	playerPosition.w = 25;
	playerPosition.h = 100;

	player2Position.x = width - 100;
	player2Position.y = mapToScreen(serial->getPot2(), 0, 1023, 0, windowHeight - int(playerPosition.h));
	player2Position.w = 25;
	player2Position.h = 100;

	ball.BallInit(25, 25, width / 2, height / 2, -1);

	return true;
}

void Game::render()
{
	// set background color
	SDL_SetRenderDrawColor(mainRenderer, 0, 0, 0, 255);
	
	// clear previous frame
	SDL_RenderClear(mainRenderer);

	// draw to the screen here!
	SDL_SetRenderDrawColor(mainRenderer, 255, 255, 255, 255);

	SDL_RenderFillRect(mainRenderer,&playerPosition);
	SDL_RenderFillRect(mainRenderer,&player2Position);
	SDL_RenderFillRect(mainRenderer, &ball.getRect());
	
	// render new frame
	SDL_RenderPresent(mainRenderer);
}

// By default, Arduino potensiometers return values between 0 and 1023 (inclusive) and therefore require mapping to the custom sized screen
int Game::mapToScreen(int rawPosition, int minValue, int maxValue, int minScreen, int maxScreen)
{
	return minScreen + (int)(((float)(maxScreen - minScreen) / (float)(maxValue - minValue)) * (rawPosition - minValue));
}

void Game::resetGame()
{
	// Pause ball while controller displays 1 second worth of LED's flashing to mark the end of the game
	ball.setBallPaused(true);
	SDL_Delay(1000);
	ball.setBallPaused(false);
	ball.invertXVel();
	ball.resetBallPosition(windowWidth / 2, windowHeight / 2);

	// Reset score
	p1Score = 0;
	p2Score = 0;

	serial->resetGame();
}

/*
* update - Process all variables !in charge of anything to do with rendering
*
*/
void Game::update()
{
	serial->getPositions();

	// Map given coordinates to screen
	playerPosition.y = mapToScreen(serial->getPot1(), 0, 1023, 0, windowHeight - int(playerPosition.h));
	player2Position.y = mapToScreen(serial->getPot2(), 0, 1023, 0, windowHeight - int(playerPosition.h));

	// If ball hits either the top or the bottom of the screen ivert its y Velocity
	if (ball.getRect().y <= 0 || ball.getRect().y >= windowHeight - ball.getRect().h)
	{
		ball.invertYVel();
	}
	// If ball reaches "behind" the platforms it is considered a score for the other player
	if (ball.getRect().x <= playerPosition.x + playerPosition.w - 1 || ball.getRect().x >= player2Position.x + 1)
	{
		if (ball.getXVel() > 0)
		{
			p1Score++;
			serial->score(-1);
		}
		else
		{
			p2Score++;
			serial->score(1);
		}
		// Reset scores and ball position if game is won
		if (p1Score == 3 || p2Score == 3)
		{
			p1Score = 0;
			p2Score = 0;
		}
		ball.resetBallPosition(windowWidth / 2, windowHeight / 2);
	}

	// check for collisions between platforms and ball
	if (checkCollision(playerPosition, ball.getRect()) > 0 || checkCollision(player2Position, ball.getRect()) > 0)
	{
		ball.invertXVel();
		ballSpeedMultiplier += 0.075;
	}

	ball.moveX(ballSpeedMultiplier);
	ball.moveY(ballSpeedMultiplier);
}

/*
* handleEvents - Poll Events and uses switch case to process specific events
*
*/
void Game::handleEvents()
{
	SDL_Event event;

	if (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_QUIT:
		{
			resetGame();
			isRunning = false;
			break; 
		}
		case SDL_MOUSEBUTTONDOWN:
			break;
		case SDL_KEYDOWN:
			if (event.key.keysym.sym == SDLK_w)
			{
				playerPosition.y -= 5.0f;
			}
			else if (event.key.keysym.sym == SDLK_s)
			{
				playerPosition.y += 5.0f;
			}
			break;
		default:
			break;
		}

	}
}

/*
* clean - Clean up SDL and close the port
*
*/
void Game::clean()
{	
	cout << "Cleaning SDL \n";
	SDL_DestroyWindow(mainWindow);
	SDL_DestroyRenderer(mainRenderer);
	SDL_Quit();
}


