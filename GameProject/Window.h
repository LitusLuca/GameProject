#pragma once
#include <string>

#include <SFML\Graphics.hpp>
#include <SFML\System.hpp>
#include <SFML\Window.hpp>
#include <SFML\Audio.hpp>
#include <SFML\Network.hpp>

#include "EventManager.h"

class Window
{
//public functions
public:
	//constructors / destructor
	Window();
	Window(const std::string& l_title, const sf::Vector2u& l_size);
	~Window();

	//update / render
	void beginDraw();
	void endDraw();

	void draw(sf::Drawable& l_drawable);

	void update();

	//getters / setters
	bool isDone();
	bool isFullScreen();
	sf::Vector2u getWindwoSize();

	void toggleFullScreen(EventDetails* l_details);

	sf::RenderWindow* getRenderWindow();

	bool isFocused();
	EventManager* getEventManager();

	void close(EventDetails* l_details = nullptr);

	

//private functions
private:
	void setup(const std::string& l_title, const sf::Vector2u& l_size);
	void destroy();
	void create();

//private data
private:
	sf::RenderWindow m_window;
	sf::Vector2u m_windowSize;
	std::string m_windowTitle;
	bool m_isDone;
	bool m_isFullScreen;
	EventManager m_eventManager;
	bool m_isFocused;
};

