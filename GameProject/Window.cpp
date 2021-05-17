#include "pch.h"

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

	m_isFocused = true;
	m_eventManager.addCallback(StateType(0), "Fullscreen_toggle", &Window::toggleFullScreen, this);
	m_eventManager.addCallback(StateType(0), "Window_close", &Window::close, this);
	
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
		if (ev.type == sf::Event::LostFocus)
		{
			m_isFocused = false;
			m_eventManager.setFocus(false);
		}
		else if (ev.type == sf::Event::GainedFocus)
		{
			m_isFocused = true;
			m_eventManager.setFocus(true);
		}
		m_eventManager.handleEvent(ev);
	}
	m_eventManager.update();
}

void Window::toggleFullScreen(EventDetails* l_details)
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
sf::Vector2u Window::getWindowSize()
{
	return m_windowSize;
}

void Window::close(EventDetails* l_details)
{
	m_isDone = true;
}

sf::FloatRect Window::getViewSpace()
{
	sf::Vector2f viewCenter = m_window.getView().getCenter();
	sf::Vector2f viewSize = m_window.getView().getSize();
	sf::Vector2f viewSizeHalf(viewSize.x / 2.f, viewSize.y / 2.f);
	sf::FloatRect viewSpace(viewCenter - viewSizeHalf, viewSize);

	return viewSpace;
}

EventManager* Window::getEventManager()
{
	return &m_eventManager;
}