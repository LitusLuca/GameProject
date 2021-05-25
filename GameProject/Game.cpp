#include "pch.h"

#include "game.h"


Game::Game():
	m_window("Game Project", sf::Vector2u(1920, 1080)), m_stateManager(&m_context), m_entityManager(&m_context, 100)
{
	m_context.m_window = &m_window;
	m_context.m_eventManager = m_window.getEventManager();
	m_context.m_entityManager = &m_entityManager;
	m_context.m_textureManager = &m_textureManager;
	m_stateManager.switchTo(StateType::Intro);
}
Game::~Game()
{
}

void Game::run()
{
	while (!m_window.isDone())
	{
		
		this->update();
		this->render();
		this->lateUpdate();
	}
}

void Game::update()
{
	m_window.update();
	m_stateManager.update(m_elapsed);
}

void Game::movePlayer(EventDetails* l_details)
{
	sf::Vector2i mousePos = m_window.getEventManager()->getMousePos(m_window.getRenderWindow());

	m_player.setPosition(mousePos.x, mousePos.y);


}

void Game::render()
{
	m_window.beginDraw();

	m_stateManager.draw();

	m_window.endDraw();
}

void Game::lateUpdate()
{
	m_stateManager.processRequests();
	this->restartClock();
}

sf::Time Game::getElapsed()
{
	return m_elapsed;
}
void Game::restartClock()
{
	m_elapsed = m_clock.restart();
}