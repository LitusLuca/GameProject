#pragma once
#include "Window.h"
#include "EventManager.h"
#include "TextureManager.h"
#include "EntityManager.h"

class Map;
struct SharedContext
{
	Window* m_window;
	EventManager* m_eventManager;
	TextureManager* m_textureManager;
	EntityManager* m_entityManager;
	Map* m_gameMap;


	SharedContext() :
		m_window(nullptr), m_eventManager(nullptr), m_textureManager(nullptr),m_entityManager(nullptr), m_gameMap(nullptr) {}
};