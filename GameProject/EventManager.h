#pragma once
#include <vector>
#include <utility>
#include <unordered_map>
#include <functional>
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>


#include <SFML\Graphics.hpp>
#include <SFML\System.hpp>
#include <SFML\Window.hpp>
#include <SFML\Audio.hpp>
#include <SFML\Network.hpp>

enum class EventType {
	KeyDown = sf::Event::KeyPressed,
	KeyUp = sf::Event::KeyReleased,
	MButtonDown = sf::Event::MouseButtonPressed,
	MButtonUp = sf::Event::MouseButtonReleased,
	MouseWheel = sf::Event::MouseWheelMoved,
	WindowResized = sf::Event::Resized,
	GainedFocus = sf::Event::GainedFocus,
	LostFocus = sf::Event::LostFocus,
	MouseEntered = sf::Event::MouseEntered,
	MouseLeft = sf::Event::MouseLeft,
	Closed = sf::Event::Closed,
	TextEntered = sf::Event::TextEntered,
	Keyboard = sf::Event::Count +1,
	Mouse,
	Joystick
};

struct EventInfo
{
	EventInfo() { m_code = 0; }
	EventInfo(int l_event) { m_code = l_event; }
	union 
	{
		int m_code;
	};
};

using Events = std::vector<std::pair<EventType, EventInfo>>;
struct EventDetails
{
	EventDetails(const std::string& l_bindName)
		: m_name(l_bindName)
	{
		this->clear();
	}
	std::string m_name;

	sf::Vector2i m_size;
	sf::Uint32 m_textEntered;
	sf::Vector2i m_mouse;
	int m_mouseWheelDelta;
	int m_keyCode;

	void clear()
	{
		m_size = sf::Vector2i(0, 0);
		m_textEntered = 0;
		m_mouse = sf::Vector2i(0, 0);
		m_mouseWheelDelta = 0;
		m_keyCode = -1;
	}
};

struct Binding
{
	Binding(const std::string& l_name)
		: m_name(l_name), m_details(l_name), c(0) {}
	void BindEvent(EventType l_type,
		EventInfo l_info = EventInfo())
	{
		m_events.emplace_back(l_type, l_info);
	}
	Events m_events;
	std::string m_name;
	int c;
	EventDetails m_details;
};

using Bindings = std::unordered_map<std::string, Binding*>;
using CallbackContainer = std::unordered_map<std::string, std::function<void(EventDetails*)>>;
enum class StateType;
using Callbacks = std::unordered_map<StateType, CallbackContainer>;


class EventManager
{
//public functions
public:
	//constructor / destructor
	EventManager();
	~EventManager();

	bool addBinding(Binding* l_binding);
	bool removeBinding(std::string l_name);

	void setFocus(const bool& l_focus);

	template<class T>
	bool addCallback(StateType l_state,const std::string& l_name, void(T::* l_func)(EventDetails*), T* l_instance)
	{
		auto itr = m_callbacks.emplace(l_state, CallbackContainer()).first;
		auto temp = std::bind(l_func, l_instance, std::placeholders::_1);
		return itr->second.emplace(l_name, temp).second;
	}

	bool removeCallback(StateType l_state,const std::string& l_name)
	{
		auto itr = m_callbacks.find(l_state);
		if (itr == m_callbacks.end()) return false;
		auto itr2 = itr->second.find(l_name);
		if (itr2 == itr->second.end()) return false;
		itr->second.erase(l_name);
		return true;
	}

	void handleEvent(sf::Event& l_event);
	void update();

	sf::Vector2i getMousePos(sf::RenderWindow* l_wind = nullptr)
	{
		return (l_wind ? sf::Mouse::getPosition(*l_wind) : sf::Mouse::getPosition());
	}

	void setCurrentState(StateType l_state);


private:
	void loadBindings();

	Bindings m_bindings;
	Callbacks m_callbacks;
	bool m_hasFocus;

	StateType m_currentState;


};

