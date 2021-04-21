#include "game.h"

//private functions
void Game::initVariables()
{
	this->window = nullptr;
}

void Game::initWindow()
{
	this->videoMode.height = 480;
	this->videoMode.width = 720;
	this->window = new sf::RenderWindow(this->videoMode, "GameProject", sf::Style::Titlebar | sf::Style::Close);
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
const bool Game::running() const
{
	return this->window->isOpen();
}



//functions
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

void Game::update()
{
	//poll events
	this->pollEvents();
}

void Game::render()
{
	//clear old frame
	this->window->clear(sf::Color::Cyan);

	//render items

	//display frame in window
	this->window->display();
}

void Game::run()
{
	//Game Loop
	while (this->running())
	{

		//update
		this->update();

		//render
		this->render();

	}
}
