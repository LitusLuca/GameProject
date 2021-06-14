#pragma once
#include "Window.h"
#include "EventManager.h"
#include "TextureManager.h"


struct SharedContext
{
	Window* m_window;
	EventManager* m_eventManager;
	TextureManager* m_textureManager;

	SharedContext() :
		m_window(nullptr), m_eventManager(nullptr), m_textureManager(nullptr) {}
};