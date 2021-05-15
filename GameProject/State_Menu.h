#pragma once
#include "BaseState.h"
#include "EventManager.h"

class State_Menu :
    public BaseState
{
public:
	State_Menu(StateManager* l_stateManager) : BaseState(l_stateManager) {}

	void onCreate();
	void onDestroy();

	void activate();
	void deactivate();

	void update(const sf::Time& l_time);
	void draw();

	void mouseClick(EventDetails* l_details);

private:
	sf::Text m_text;
	sf::Font m_font;

	sf::Vector2f m_buttonSize;
	sf::Vector2f m_buttonPos;
	unsigned int m_buttonPadding;

	sf::RectangleShape m_rects[3];
	sf::Text m_labels[3];
	
};

