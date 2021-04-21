#pragma once

#include "State.h"

class Game
{
private:
	//variables
	//window
	sf::RenderWindow* window;
	sf::Event ev;
	sf::VideoMode videoMode;

	sf::Clock dtClock;
	float dt;

	//private functions
	void initVariables();
	void initWindow();

public:
	//constructors / destructors
	Game();
	virtual ~Game();

	//accessors
	const bool running() const;

	//functions
	void updateDt();
	void pollEvents();
	void update();
	void render();
	void run();
};

