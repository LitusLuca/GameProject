#include "pch.h"

#include "State_Intro.h"
#include "StateManager.h"

void State_Intro::onCreate()
{
	m_timePassed = 0.0f;
	sf::Vector2u windowSize = m_stateManager->getContext()->m_window->getRenderWindow()->getSize();
	m_introTexture.loadFromFile("recources/intro.png");
	m_introSprite.setTexture(m_introTexture);

	m_introSprite.setOrigin(m_introTexture.getSize().x / 2.0f, m_introTexture.getSize().y / 2.0f);
	m_introSprite.setPosition(windowSize.x / 2.f, -1*  m_introSprite.getLocalBounds().height / 2.0f);

	m_font.loadFromFile("recources/arial.ttf");
	m_text.setFont(m_font);
	m_text.setString("press SPACE to continue!");
	m_text.setCharacterSize(15);
	sf::FloatRect textRect = m_text.getLocalBounds();
	m_text.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
	m_text.setPosition(windowSize.x / 2.0f, windowSize.y / 2.0f);

	EventManager* evMgr = m_stateManager->getContext()->m_eventManager;
	evMgr->addCallback(StateType::Intro, "Intro_Continue", &State_Intro::continueIntro, this);

}

void State_Intro::onDestroy()
{
	EventManager* evMgr = m_stateManager->getContext()->m_eventManager;
	evMgr->removeCallback(StateType::Intro, "Intro_Continue");
}

void State_Intro::update(const sf::Time& l_time)
{
	if (m_timePassed < 2.5f)
	{
		m_timePassed += l_time.asSeconds();
		m_introSprite.setPosition(m_introSprite.getPosition().x, m_introSprite.getPosition().y + (100 * l_time.asSeconds()));
	}
}

void State_Intro::draw()
{
	sf::RenderWindow* window = m_stateManager->getContext()->m_window->getRenderWindow();
	window->draw(m_introSprite);
	if (m_timePassed > 2.5f)
	{
		window->draw(m_text);
	}
}

void State_Intro::continueIntro(EventDetails* l_details)
{
	if (m_timePassed >= 2.5f)
	{
		m_stateManager->switchTo(StateType::MainMenu);
		m_stateManager->remove(StateType::Intro);
	}
}

void State_Intro::activate() {}
void State_Intro::deactivate() {}