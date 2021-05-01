#include "game.h"


Game::Game():
	m_window("Game Project", sf::Vector2u(800, 600))
{
	m_player.setFillColor(sf::Color::Green);
	m_player.setRadius(40.f);
	m_player.setPosition(100.f, 100.f);
	m_increment = sf::Vector2i(400, 400);

	m_textBox.setup(5, 14, 350, sf::Vector2f(255, 0));
	m_textBox.add("Seeded random number generator with: " + std::to_string(time(NULL)));
}
Game::~Game()
{
}

void Game::run()
{
	while (!m_window.isDone())
	{
		//this->processEvents();
		this->update();
		this->render();
		this->restartClock();
	}
}

void Game::update()
{
	m_window.update();
	this->movePlayer();
}

void Game::movePlayer()
{
	sf::Vector2u l_winSize = m_window.getWindwoSize();
	float l_playerSize = 2* m_player.getRadius();
	if ((m_player.getPosition().x > l_winSize.x - l_playerSize && m_increment.x > 0) ||
		(m_player.getPosition().x < 0 && m_increment.x < 0))
	{
		m_increment.x = -m_increment.x;
		m_textBox.add("Bounce!!!");
	}
		

	if ((m_player.getPosition().y > l_winSize.y - l_playerSize && m_increment.y > 0) ||
		(m_player.getPosition().y < 0 && m_increment.y < 0))
	{
		m_increment.y = -m_increment.y;
		m_textBox.add("bonk!!!");
	}

	float fElapsed = m_elapsed.asSeconds();
	m_player.setPosition(
		m_player.getPosition().x + m_increment.x * fElapsed,
		m_player.getPosition().y + m_increment.y * fElapsed);
}

void Game::render()
{
	m_window.beginDraw();
	m_window.draw(m_player);

	m_textBox.render(*m_window.getRenderWindow());

	m_window.endDraw();
}

sf::Time Game::getElapsed()
{
	return m_elapsed;
}
void Game::restartClock()
{
	m_elapsed = m_clock.restart();
}