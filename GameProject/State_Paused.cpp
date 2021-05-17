#include "pch.h"

#include "State_Paused.h"
#include "StateManager.h"

void State_Paused::onCreate()
{
	m_font.loadFromFile("recources/arial.ttf");
	m_text.setFont(m_font);
	m_text.setString("PAUSED");
	m_text.setCharacterSize(28);
	m_text.setStyle(sf::Text::Bold);

	sf::Vector2u winSize = m_stateManager->getContext()->m_window->getWindowSize();
	
	sf::FloatRect textRect = m_text.getLocalBounds();
	m_text.setOrigin(textRect.left + textRect.width / 2.f, textRect.top + textRect.height / 2.f);
	m_text.setPosition(m_stateManager->getContext()->m_window->getWindowSize().x / 2.f, 500);

	m_rect.setSize(sf::Vector2f(winSize));
	m_rect.setPosition(0, 0);
	m_rect.setFillColor(sf::Color(0, 0, 0, 150));

	m_stateManager->getContext()->m_eventManager->addCallback(StateType::Paused, "Key_P", &State_Paused::unpause, this);
	setTransparent(true);
}

void State_Paused::onDestroy()
{
	m_stateManager->getContext()->m_eventManager->removeCallback(StateType::Paused, "Key_P");
}

void State_Paused::draw()
{
	sf::RenderWindow* window = m_stateManager->getContext()->m_window->getRenderWindow();
	window->draw(m_rect);
	window->draw(m_text);
}

void State_Paused::activate()
{
}

void State_Paused::deactivate()
{
}

void State_Paused::unpause(EventDetails* l_details)
{
	m_stateManager->switchTo(StateType::Game);
}

void State_Paused::update(const sf::Time& l_time)
{

}