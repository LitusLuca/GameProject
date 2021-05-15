#pragma once
#include "BaseState.h"
#include "EventManager.h"

class State_Paused :
    public BaseState
{
public:
    State_Paused(StateManager* l_stateManager) :
        BaseState(l_stateManager) {}

    void onCreate();
    void onDestroy();
    
    void update(const sf::Time& l_time);
    void draw();

    void activate();
    void deactivate();

    void unpause(EventDetails* l_details);

private:
    sf::Text m_text;
    sf::Font m_font;

    sf::RectangleShape m_rect;
};

