#include "pch.h"

#include "State_Game.h"
#include "StateManager.h"

void State_Game::onCreate()
{
	m_texture.loadFromFile("texture.png");
	m_sprite.setTexture(m_texture);

	m_increment = sf::Vector2f(360.f, 360.f);

	EventManager* evMgr = m_stateManager->getContext()->m_eventManager;
	evMgr->addCallback(StateType::Game, "Key_Escape", &State_Game::mainMenu, this);
	evMgr->addCallback(StateType::Game, "Key_P", &State_Game::pause, this);
}

void State_Game::onDestroy()
{
	EventManager* evMgr = m_stateManager->getContext()->m_eventManager;
	evMgr->removeCallback(StateType::Game,"Key_Escape");
	evMgr->removeCallback(StateType::Game, "Key_P");
}

void State_Game::activate()
{
}

void State_Game::deactivate()
{
}

void State_Game::update(const sf::Time& l_time)
{
	sf::Vector2u winSize = m_stateManager->getContext()->m_window->getWindowSize();
	sf::Vector2u textureSize = m_texture.getSize();

	if (m_sprite.getPosition().x < 0 && m_increment.x < 0 || (m_sprite.getPosition().x + textureSize.x > winSize.x && m_increment.x > 0))
	{
		m_increment.x = -m_increment.x;
	}
	if (m_sprite.getPosition().y < 0 && m_increment.y < 0 || (m_sprite.getPosition().y + textureSize.y > winSize.y && m_increment.y > 0))
	{
		m_increment.y = -m_increment.y;
	}

	m_sprite.setPosition(m_sprite.getPosition().x + m_increment.x * l_time.asSeconds(), m_sprite.getPosition().y + m_increment.y * l_time.asSeconds());
}

void State_Game::draw()
{
	m_stateManager->getContext()->m_window->draw(m_sprite);
}

void State_Game::mainMenu(EventDetails* l_details)
{
	m_stateManager->switchTo(StateType::MainMenu);
}
void State_Game::pause(EventDetails* l_details)
{
	m_stateManager->switchTo(StateType::Paused);
}