#include "pch.h"

#include "State_Game.h"
#include "StateManager.h"

void State_Game::onCreate()
{
	m_texture.loadFromFile("recources/texture.png");
	m_sprite.setTexture(m_texture);

	m_increment = sf::Vector2f(360.f, 360.f);

	EventManager* evMgr = m_stateManager->getContext()->m_eventManager;
	evMgr->addCallback(StateType::Game, "Key_Escape", &State_Game::mainMenu, this);
	evMgr->addCallback(StateType::Game, "Key_P", &State_Game::pause, this);

	sf::Vector2u size = m_stateManager->getContext()->m_window->getWindowSize();
	m_view.setSize(size.x, size.y);
	m_view.setCenter(size.x / 2, size.y / 2);
	m_view.zoom(0.6f);
	m_stateManager->getContext()->m_window->getRenderWindow()->setView(m_view);

	m_gameMap = new Map(m_stateManager->getContext(), this);
	m_gameMap->loadMap("media/Maps/map1.map");
}

void State_Game::onDestroy()
{
	EventManager* evMgr = m_stateManager->getContext()->m_eventManager;
	evMgr->removeCallback(StateType::Game,"Key_Escape");
	evMgr->removeCallback(StateType::Game, "Key_P");
	delete m_gameMap;
	m_gameMap = nullptr;
	delete m_gameMap;
	m_gameMap = nullptr;
}

void State_Game::activate()
{
}

void State_Game::deactivate()
{
}

void State_Game::update(const sf::Time& l_time)
{
	SharedContext* context = m_stateManager->getContext();
	EntityBase* player = context->m_entityManager->find("Player");
	if (!player)
	{
		std::cout << "Respawning Player\n";
		context->m_entityManager->add(EntityType::Player, "Player");
		player = context->m_entityManager->find("Player");
		player->setPosition(m_gameMap->getPlayerStart());
	}
	else
	{
		m_view.setCenter(player->getPosition());
		context->m_window->getRenderWindow()->setView(m_view);
	}
	sf::FloatRect viewSpace = context->m_window->getViewSpace();
	if (viewSpace.left < 0)
	{
		m_view.setCenter(viewSpace.width / 2, m_view.getCenter().y);
		context->m_window->getRenderWindow()->setView(m_view);
	}
	else if (viewSpace.left + viewSpace.width > (m_gameMap->getMapSize().x + 1) * Sheet::Tile_Size)
	{
		m_view.setCenter((m_gameMap->getMapSize().x + 1) * Sheet::Tile_Size - (viewSpace.width / 2), m_view.getCenter().y);
		context->m_window->getRenderWindow()->setView(m_view);
	}
	m_gameMap->update(l_time.asSeconds());
	m_stateManager->getContext()->m_entityManager->update(l_time.asSeconds());
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