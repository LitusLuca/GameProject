#include "game.h"

//private functions

/**
	initiate Game variables
*/
void Game::initVariables()
{
	this->window = nullptr;

	this->videoMode.height = 480;
	this->videoMode.width = 720;
}

/**
	initiate window with window.ini

	@return void
*/
void Game::initWindow()
{
	std::ifstream ifs("config/window.ini");
	std::string window_title = "Game";
	unsigned int fps_lim = 144;
	bool v_sync = false;

	if (ifs.is_open()) {
		std::getline(ifs, window_title);
		ifs >> videoMode.width >> videoMode.height;
		ifs >> fps_lim;
		ifs >> v_sync;
	}

	ifs.close();

	this->window = new sf::RenderWindow(this->videoMode, window_title, sf::Style::Titlebar | sf::Style::Close);
	this->window->setFramerateLimit(fps_lim);
	this->window->setVerticalSyncEnabled(v_sync);
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
