#include "State_Menu.h"
#include "StateManager.h"

void State_Menu::onCreate()
{
	m_font.loadFromFile("recources/arial.ttf");
	m_text.setFont(m_font);
	m_text.setString("MAIN MENU:");
	m_text.setCharacterSize(28);
	sf::FloatRect textRect = m_text.getLocalBounds();
	m_text.setOrigin(textRect.left + textRect.width / 2.f, textRect.top + textRect.height / 2.f);
	m_text.setPosition(m_stateManager->getContext()->m_window->getWindowSize().x / 2.f, 300);

	m_buttonSize = sf::Vector2f(600.f, 64.f);
	m_buttonPos = sf::Vector2f(m_stateManager->getContext()->m_window->getWindowSize().x / 2.f, 500);
	m_buttonPadding = 4.f;
	std::string str[3];
	str[0] = "PLAY";
	str[1] = "CREDITS";
	str[2] = "QUIT";

	for (size_t i = 0; i < 3; i++)
	{
		sf::Vector2f buttonPos(m_buttonPos.x, m_buttonPos.y + (i * (m_buttonSize.y + m_buttonPadding)));
		m_rects[i].setFillColor(sf::Color::Red);
		m_rects[i].setSize(m_buttonSize);

		m_rects[i].setOrigin(m_buttonSize.x / 2.f, m_buttonSize.y / 2.f);
		m_rects[i].setPosition(buttonPos);

		m_labels[i].setFont(m_font);
		m_labels[i].setCharacterSize(24);
		m_labels[i].setString(str[i]);

		sf::FloatRect labelRect = m_labels[i].getLocalBounds();
		m_labels[i].setOrigin(labelRect.left + labelRect.width / 2.f, labelRect.top + labelRect.height / 2.f);

		m_labels[i].setPosition(buttonPos);
	}

	m_stateManager->getContext()->m_eventManager->addCallback(StateType::MainMenu, "Mouse_Left", &State_Menu::mouseClick, this);
}

void State_Menu::onDestroy()
{
	m_stateManager->getContext()->m_eventManager->removeCallback(StateType::MainMenu, "Mouse_Left");
}

void State_Menu::activate()
{
	if (m_stateManager->hasState(StateType::Game) && m_labels[0].getString() == "PLAY")
	{
		m_labels[0].setString("RESUME");
		sf::FloatRect rect = m_labels[0].getLocalBounds();
		m_labels[0].setOrigin(rect.left + rect.width / 2.f, rect.top + rect.height / 2.f);
	}
}

void State_Menu::mouseClick(EventDetails* l_details)
{
	
	sf::Vector2i mousePos = l_details->m_mouse;
	std::cout << mousePos.x << mousePos.y << std::endl;
	float halfX = m_buttonSize.x / 2.f;
	float halfY = m_buttonSize.y / 2.f;
	for (size_t i = 0; i < 3; i++)
	{
		if (mousePos.x >= m_rects[i].getPosition().x - halfX &&
			mousePos.x <= m_rects[i].getPosition().x + halfX &&
			mousePos.y >= m_rects[i].getPosition().y - halfY &&
			mousePos.y <= m_rects[i].getPosition().y + halfY)
		{
			if (i==0)
			{
				m_stateManager->switchTo(StateType::Game);
			}
			else if (i==1)
			{
				//credit state
			}
			else if (i==2)
			{
				m_stateManager->getContext()->m_window->close();
			}
		}
	}
}

void State_Menu::draw()
{
	sf::RenderWindow* window = m_stateManager->getContext()->m_window->getRenderWindow();
	window->draw(m_text);
	for (size_t i = 0; i < 3; i++)
	{
		window->draw(m_rects[i]);
		window->draw(m_labels[i]);
	}
}

void State_Menu::deactivate() {}

void State_Menu::update(const sf::Time& l_time) {}