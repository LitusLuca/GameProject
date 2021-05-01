#include "Window.h"

Window::Window()
{
	this->setup("SFML Window", sf::Vector2u(640, 480));
}

Window::Window(const std::string& l_title, const sf::Vector2u& l_size) 
{
	this->setup(l_title, l_size);
}

Window::~Window()
{
	this->destroy();
}

void Window::setup(const std::string& l_title, const sf::Vector2u& l_size)
{
	m_windowTitle = l_title;
	m_windowSize = l_size;
	m_isFullScreen = false;
	m_isDone = false;
	
	this->create();
}

void Window::create()
{
	auto style = m_isFullScreen ? sf::Style::Fullscreen :
		(sf::Style::Titlebar | sf::Style::Close);
	m_window.create(sf::VideoMode(m_windowSize.x, m_windowSize.y), m_windowTitle, style);
	m_window.setFramerateLimit(144);
}

void Window::destroy()
{
	m_window.close();
}

void Window::update()
{
	sf::Event ev;
	while (m_window.pollEvent(ev))
	{
		switch (ev.type)
		{
		case sf::Event::Closed:
			m_isDone = true;
		case sf::Event::KeyPressed:
			switch (ev.key.code)
			{
			case sf::Keyboard::F5:
				this->toggleFullScreen();
			}
		}
	}
}

void Window::toggleFullScreen()
{
	m_isFullScreen = !m_isFullScreen;
	this->destroy();
	this->create();
}

sf::RenderWindow* Window::getRenderWindow()
{
	return &m_window;
}

void Window::beginDraw() 
{
	m_window.clear(sf::Color::Black);
}

void Window::endDraw() 
{
	m_window.display();
}

void Window::draw(sf::Drawable& l_drawable)
{
	m_window.draw(l_drawable);
}

bool Window::isDone() 
{
	return m_isDone;
}
bool Window::isFullScreen() 
{
	return m_isFullScreen;
}
sf::Vector2u Window::getWindwoSize()
{
	return m_windowSize;
}