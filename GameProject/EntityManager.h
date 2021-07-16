#pragma once
#include "EntityBase.h"
#include "Player.h"

using EntityContainer = std::unordered_map<unsigned int, EntityBase*>;
using EntityFactory = std::unordered_map<EntityType, std::function<EntityBase* (void)>>;
using EnemyTypes = std::unordered_map<std::string, std::string>;

struct SharedContext;

class EntityManager
{
public:
	EntityManager(SharedContext* l_context, unsigned int l_maxEntities);
	~EntityManager();

	int add(const EntityType& l_type, const std::string& l_name = "");
	EntityBase* find(unsigned int l_id);
	EntityBase* find(const std::string& l_name);
	void remove(unsigned int l_id);

	void update(float l_dT);
	void draw();

	void purge();

	SharedContext* getContext();

private:
	template<class T>
	void registerEntity(const EntityType& l_type)
	{
		m_entityFactory[l_type] = [this]()->EntityBase*
		{
			return new T(this);
		};
	}

	void processRemovals();
	void loadEnemyTypes(const std::string& l_name);
	void EntityCollisionCheck();

	EntityContainer m_entities;
	EnemyTypes m_enemyTypes;
	EntityFactory m_entityFactory;
	SharedContext* m_context;
	unsigned int m_idCounter;
	unsigned int m_maxEntities;

	std::vector<unsigned int> m_entitiesToRemove;
};

