#include "pch.h"

#include "State_Game.h"
#include "StateManager.h"
#include "Direction.h"

void State_Game::onCreate()
{
	m_increment = sf::Vector2f(360.f, 360.f);

	m_pepega = new SpriteSheet(m_stateManager->getContext()->m_textureManager);
	m_pepega->loadSheet("media/SpriteSheets/pepega.sheet");
	m_pepega->getCurrentAnimation()->setLooping(true);
	m_pepega->setSpritePosition(sf::Vector2f(360.f, 360.f));

	EventManager* evMgr = m_stateManager->getContext()->m_eventManager;
	evMgr->addCallback(StateType::Game, "Key_Escape", &State_Game::mainMenu, this);
	evMgr->addCallback(StateType::Game, "Key_P", &State_Game::pause, this);
}

void State_Game::onDestroy()
{
	EventManager* evMgr = m_stateManager->getContext()->m_eventManager;
	evMgr->removeCallback(StateType::Game,"Key_Escape");
	evMgr->removeCallback(StateType::Game, "Key_P");
	delete m_pepega;
	m_pepega = nullptr;
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

	m_pepega->update(l_time.asSeconds());
	sf::Vector2f pepePos = m_pepega->getSpritePosition();
	sf::Vector2i pepeSize = m_pepega->getSpriteSize();
	if (pepePos.x < 0 + pepeSize.x / 2 && m_increment.x < 0 || (pepePos.x + pepeSize.x / 2 > winSize.x && m_increment.x > 0))
	{
		m_increment.x = -m_increment.x;
		if (m_pepega->getDirection() == Direction::Right) m_pepega->setDirection(Direction::Left);
		else m_pepega->setDirection(Direction::Right);
	}
	if (pepePos.y < 0 + pepeSize.y / 2 && m_increment.y < 0 || (pepePos.y + pepeSize.y / 2 > winSize.y && m_increment.y > 0))
	{
		m_increment.y = -m_increment.y;
	}
	m_pepega->setSpritePosition(sf::Vector2f(pepePos.x + m_increment.x * l_time.asSeconds(), pepePos.y + m_increment.y * l_time.asSeconds()));
}

void State_Game::draw()
{
	m_pepega->draw(m_stateManager->getContext()->m_window->getRenderWindow());
}

void State_Game::mainMenu(EventDetails* l_details)
{
	m_stateManager->switchTo(StateType::MainMenu);
}
void State_Game::pause(EventDetails* l_details)
{
	m_stateManager->switchTo(StateType::Paused);
}