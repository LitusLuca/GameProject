#pragma once
#include "Window.h"
#include "EventManager.h"


struct SharedContext
{
	Window* m_window;
	EventManager* m_eventManager;

	SharedContext() :
		m_window(nullptr), m_eventManager(nullptr) {}
};