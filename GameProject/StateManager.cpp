#include "StateManager.h"

StateManager::StateManager(SharedContext* l_shared)
{
	this->registerState<State_Intro>(StateType::Intro);
	this->registerState<State_MainMenu>(StateType::MainMenu);
	this->registerState<State_Game>(StateType::Game);
	this->registerState<State_Paused>(StateType::Paused);
}

StateManager::~StateManager()
{
	for (auto& itr : m_states)
	{
		itr.second->onDestroy();
		delete itr.second;
	}
}

void StateManager::update(const sf::Time& l_time)
{
	if (m_states.empty()) return;
	if (m_states.back().second->isTranscendent() && m_states.size() >1)
	{
		auto itr = m_states.end();
		while (itr != m_states.begin())
		{
			if (itr != m_states.end())
			{
				if (!itr->second->isTranscendent()) break;
			}
			--itr;
		}
		for (; itr != m_states.end(); itr++)
		{
			itr->second->update(l_time);
		}
	}
	else
	{
		m_states.end()->second->update(l_time);
	}
}

void StateManager::draw()
{
	if (m_states.empty())
	{
		return;
	}
	if (m_states.back().second->isTransparent() && m_states.size() > 1)
	{
		auto itr = m_states.end();
		while (itr != m_states.begin())
		{
			if (itr != m_states.end())
			{
				if (!itr->second->isTransparent()) break;
			}
			--itr;
		}
		for (; itr != m_states.end(); itr++)
		{
			itr->second->draw();
		}
	}
	else
	{
		m_states.end()->second->draw();
	}
}

void StateManager::processRequests()
{
	while (m_toRemove.begin() != m_toRemove.end())
	{
		this->removeState(*m_toRemove.begin());
		m_toRemove.erase(m_toRemove.begin());
	}
}

SharedContext* StateManager::getContext()
{
	return m_shared;
}

bool StateManager::hasState(const StateType& l_type)
{
	for (auto itr = m_states.begin(); itr != m_states.end(); itr++)
	{
		if (itr->first == l_type)
		{
			auto removed = std::find(m_toRemove.begin(), m_toRemove.end(), l_type);
			if (removed == m_toRemove.end())
			{
				return true;
			}
			return false;
		}
	}
	return false;
}

void StateManager::switchTo(const StateType& l_type)
{
	m_shared->m_eventManager->setCurrentState(l_type);
	for (auto itr = m_states.begin(); itr != m_states.end(); itr++)
	{
		if (itr->first == l_type)
		{
			m_states.back().second->deactivate();
			StateType tmp_type = itr->first;
			BaseState* tmp_state = itr->second;
			m_states.erase(itr);
			m_states.emplace_back(tmp_type, tmp_state);
			tmp_state->activate();
			return;
		}
	}
	if (!m_states.empty())
	{
		m_states.back().second->deactivate();
	}
	this->createState(l_type);
	m_states.back().second->activate();
}

void StateManager::remove(const StateType& l_type)
{
	m_toRemove.push_back(l_type);
}

void StateManager::createState(const StateType& l_type)
{
	auto newState = m_stateFactory.find(l_type);
	if (newState == m_stateFactory.end()) return;
	BaseState* state = newState->second();
	m_states.emplace_back(l_type, state);
	state->onCreate();
}

void StateManager::removeState(const StateType& l_type)
{
	for (auto itr = m_states.begin(); itr != m_states.end(); itr++)
	{
		if (itr->first == l_type)
		{
			itr->second->onDestroy();
			delete itr->second();
			m_states.erase(itr);
			return;
		}
	}
}
