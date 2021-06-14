#pragma once
#include "BaseState.h"
#include "EventManager.h"

class State_Intro :
    public BaseState
{
public:
	State_Intro(StateManager* l_stateManager) : BaseState(l_stateManager) {}

	void onCreate();
	void onDestroy();

	void activate();
	void deactivate();

	void update(const sf::Time& l_time);
	void draw();

    void continueIntro(EventDetails* l_details);

private:
	std::string m_introTexture;
	sf::Sprite m_introSprite;
	sf::Text m_text;
	sf::Font m_font;
	float m_timePassed;
};

