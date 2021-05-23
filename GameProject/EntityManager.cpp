#include "pch.h"
#include "EntityManager.h"

// TODO: add entity collision
// TODO: add enemy loading

EntityManager::EntityManager(SharedContext* l_context, unsigned int l_maxEntities) :
	m_context(l_context), m_maxEntities(l_maxEntities), m_idCounter(0)
{
	//loadEnemyTypes("enemyList.list");
	registerEntity<Player>(EntityType::Player);
	//registerEntity<Enemy>(EntityType::Enemy);
}

EntityManager::~EntityManager()
{
	purge();
}

int EntityManager::add(const EntityType& l_type, const std::string& l_name)
{
	auto itr = m_entityFactory.find(l_type);
	if (itr = m_entityFactory.end()) return -1;
	EntityBase* entity = itr->second();
	entity->m_id = m_idCounter;

	if (l_name != "") entity->m_name = l_name;
	m_entities.emplace(m_idCounter, entity);
	if (l_type == EntityType::Enemy)
	{
		auto itr = m_enemyTypes.find(l_name);
		if (itr != m_enemyTypes.end())
		{
			Enemy* enemy = (Enemy*)entity;
			enemy->load(itr->second);
		}
	}
	++m_idCounter;
	return m_idCounter - 1;
}

EntityBase* EntityManager::find(const std::string& l_name)
{
	for (auto& itr : m_entities)
	{
		if (itr.second->getName() == l_name) return itr.second;
	}
	return nullptr;
}

EntityBase* EntityManager::find(unsigned int l_id)
{
	auto itr = m_entities.find(l_id);
	if (itr == m_entities.end()) return nullptr;
	return itr->second;
}

void EntityManager::remove(unsigned int l_id)
{
	m_entitiesToRemove.emplace_back(l_id);
}

void EntityManager::update(float l_dT)
{
	for (auto& itr : m_entities)
	{
		itr.second->update(l_dT);
	}
	entityCollisionCheck();
	processRemovals();
}

void EntityManager::draw()
{
	sf::RenderWindow* window = m_context->m_window->getRenderWindow();
	sf::FloatRect viewSpace = m_context->m_window->getViewSpace();

	for (auto& itr : m_entities)
	{
		if (!viewSpace.intersects(itr.second->m_AABB)) continue;
		itr.second->draw(window);
	}
}

void EntityManager::purge()
{
	for (auto& itr : m_entities)
	{
		delete itr.second;
	}
	m_entities.clear();
	m_idCounter = 0;
}

SharedContext* EntityManager::getContext()
{
	return m_context;
}

void EntityManager::processRemovals()
{
	while (m_entitiesToRemove.begin() != m_entitiesToRemove.end())
	{
		unsigned int id = m_entitiesToRemove.back();
		auto itr = m_entities.find(id);
		if (itr != m_entities.end())
		{
			delete itr->second;
			m_entities.erase(itr);
		}
		m_entitiesToRemove.pop_back();
	}
}