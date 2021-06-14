#pragma once

#include "Window.h"
#include "EventManager.h"
#include "StateManager.h"
#include "SharedContext.h"
#include "TextBox.h"

class Game
{
//public functions
public:

	//constructor / destructor
	Game();
	virtual ~Game();

	//run game
	void run();

	//update / render
	//void processEvents();
	void update();

	sf::Time getElapsed();
	void restartClock();

	void render();

	void lateUpdate();
//private functions
private:
	void movePlayer(EventDetails* l_details);
//private data
private:
	Window m_window;
	sf::CircleShape m_player;
	sf::Vector2i m_increment;

	sf::Clock m_clock;
	sf::Time m_elapsed;

	TextBox m_textBox;

	StateManager m_stateManager;
	TextureManager m_textureManager;
	SharedContext m_context;
};

