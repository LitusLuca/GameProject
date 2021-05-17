#pragma once


class StateManager;

class BaseState
{
	friend class StateManager;
public:
	//constructor / destructor
	BaseState(StateManager* l_stateManager) :
		m_stateManager(l_stateManager), m_transparent(false), m_transcendent(false) {}
	virtual ~BaseState() {}

	virtual void onCreate() = 0;
	virtual void onDestroy() = 0;
	
	virtual void activate() = 0;
	virtual void deactivate() = 0;

	virtual void update(const sf::Time& l_time) = 0;
	virtual void draw() = 0;
	
	void setTransparent(const bool& l_transparent)
	{
		m_transparent = l_transparent;
	}
	bool isTransparent()
	{
		return m_transparent;
	}
	
	void setTranscendent(const bool& l_transcendent)
	{
		m_transcendent = l_transcendent;
	}
	bool isTranscendent()
	{
		return m_transcendent;
	}

	StateManager* getStateManager()
	{
		return m_stateManager;
	}
	sf::View& getView()
	{
		return m_view;
	}

protected:
	StateManager* m_stateManager;
	bool m_transparent;
	bool m_transcendent;
	sf::View m_view;
};

