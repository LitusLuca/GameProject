#pragma once
#include "BaseState.h"
#include "EventManager.h"
#include "Map.h"

class State_Game :
    public BaseState
{
public:
    State_Game(StateManager* l_stateManager) :
        BaseState(l_stateManager) {}

	void onCreate();
	void onDestroy();

	void activate();
	void deactivate();

	void update(const sf::Time& l_time);
	void draw();

	void mainMenu(EventDetails* l_details);
	void pause(EventDetails* l_details);

private:
	sf::Vector2f m_increment;
	SpriteSheet* m_pepega;
	Map* m_gameMap;
};

