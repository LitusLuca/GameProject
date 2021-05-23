#pragma once
#include "Window.h"
#include "EventManager.h"
#include "TextureManager.h"

class Map;
struct SharedContext
{
	Window* m_window;
	EventManager* m_eventManager;
	TextureManager* m_textureManager;

	Map* m_gameMap;
	// TODO: adjust shared context
	SharedContext() :
		m_window(nullptr), m_eventManager(nullptr), m_textureManager(nullptr), m_gameMap(nullptr) {}
};