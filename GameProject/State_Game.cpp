#include "pch.h"

#include "State_Game.h"
#include "StateManager.h"

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
	m_gameMap = new Map(m_stateManager->getContext(), this);
	m_gameMap->loadMap("media/Maps/map1.map");
}

void State_Game::onDestroy()
{
	EventManager* evMgr = m_stateManager->getContext()->m_eventManager;
	evMgr->removeCallback(StateType::Game,"Key_Escape");
	evMgr->removeCallback(StateType::Game, "Key_P");
	delete m_pepega;
	m_pepega = nullptr;
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
	
	

	SharedContext* sharedContext = getStateManager()->getContext();
	EntityBase* player = sharedContext->m_entityManager->find("pepega");
	if (!player)
	{
		std::cout << "Spawning: pepega\n";
		sharedContext->m_entityManager->add(EntityType::Player, "pepega");
		player = sharedContext->m_entityManager->find("pepega");
		player->setPosition(m_gameMap->getPlayerStart());
	}
	else
	{
		m_view.setCenter(player->getPosition());
		sharedContext->m_window->getRenderWindow()->setView(m_view);
	}
	//TODO: setting viewspace
	sf::FloatRect viewSpace = sharedContext->m_window->getViewSpace();
	if (viewSpace.left < 0)
	{
		m_view.setCenter(viewSpace.width / 2, m_view.getCenter().y);
		sharedContext->m_window->getRenderWindow()->setView(m_view);
	}
	else if (viewSpace.left + viewSpace.width > (m_gameMap->getMapSize().x + 1) * Sheet::Tile_Size)
	{
		m_view.setCenter((m_gameMap->getMapSize().x + 1) * Sheet::Tile_Size - viewSpace.width / 2, m_view.getCenter().y);
		sharedContext->m_window->getRenderWindow()->setView(m_view);
	}
	m_gameMap->update(l_time.asSeconds());
	sharedContext->m_entityManager->update(l_time.asSeconds());
	//std::cout << player->getPosition().x << "  " << player->getPosition().y << "\n";
}

void State_Game::draw()
{
	m_gameMap->draw();
	m_pepega->draw(m_stateManager->getContext()->m_window->getRenderWindow());
	m_stateManager->getContext()->m_entityManager->draw();
}

void State_Game::mainMenu(EventDetails* l_details)
{
	m_stateManager->switchTo(StateType::MainMenu);
}
void State_Game::pause(EventDetails* l_details)
{
	m_stateManager->switchTo(StateType::Paused);
}