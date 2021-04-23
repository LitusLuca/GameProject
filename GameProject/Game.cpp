#include "game.h"

//private functions

/**
	initiate Game variables
*/
void Game::initVariables()
{
	this->window = nullptr;
}

/**
	initiate window with window.ini

	@return void
*/
void Game::initWindow()
{
	
	this->videoMode.height = 480;
	this->videoMode.width = 720;
	this->window = new sf::RenderWindow(this->videoMode, "GameProject", sf::Style::Titlebar | sf::Style::Close);
	this->window->setFramerateLimit(144);
	this->window->setVerticalSyncEnabled(false);
}

//constructors / destructors
Game::Game()
{
	this->initVariables();
	this->initWindow();
}

Game::~Game()
{
	delete this->window;
}

//accessors

/**
	return whether game is or is not running

	@return bool
*/
const bool Game::running() const
{
	return this->window->isOpen();
}

//functions

/**
	
	update the dt variable with the time it takes to update the frame

	@return void
*/
void Game::updateDt()
{
	this->dt = this->dtClock.restart().asSeconds();
}

/**
	polls for sfml events and performs the according actions

	@return void
*/
void Game::pollEvents()
{
	//Event Polling
	while (this->window->pollEvent(this->ev))
	{
		switch (this->ev.type)
		{
		case sf::Event::Closed:
			this->window->close();
			break;
		case sf::Event::KeyPressed:
			switch (this->ev.key.code)
			{
			case sf::Keyboard::Escape:
				this->window->close();
				break;
			}
			break;
		}
	}
}

/**
	excecute game logic

	@return void
*/
void Game::update()
{
	//poll events
	this->pollEvents();
}

/**
	render the frame to the window using the state

	@return void
*/
void Game::render()
{
	//clear old frame
	this->window->clear(sf::Color::Cyan);

	//render items

	//display frame in window
	this->window->display();
}

/**
	running the game
*/
void Game::run()
{
	//Game Loop
	while (this->running())
	{
		//update dt
		this->updateDt();
		//update
		this->update();

		//render
		this->render();

	}
}
