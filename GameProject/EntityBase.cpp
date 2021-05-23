#include "pch.h"
#include "EntityBase.h"
#include "Map.h"
#include "SharedContext.h"


EntityBase::EntityBase(EntityManager* l_entityMgr) :
	m_entityManager(l_entityMgr), m_name("BaseEntity"), m_type(EntityType::Base),
	m_refferenceTile(nullptr), m_state(EntityState::Idle), m_id(0),
	m_collisionOnX(false), m_collisionOnY(false) {}

EntityBase::~EntityBase() {}

void EntityBase::setPosition(const float& l_x, const float& l_y)
{
	m_position = sf::Vector2f(l_x, l_y);
	updateAABB();
}

void EntityBase::setPosition(const sf::Vector2f& l_pos)
{
	m_position = l_pos;
	updateAABB();
}

void EntityBase::setSize(const float& l_x, const float& l_y)
{
	m_size = sf::Vector2f(l_x, l_y);
	updateAABB();
}

void EntityBase::setState(const EntityState& l_state)
{
	if (m_state == EntityState::Dying) return;
	m_state = l_state;
}

void EntityBase::move(float l_x, float l_y)
{
	m_positionOld = m_position;
	m_position += sf::Vector2f(l_x, l_y);
	sf::Vector2u mapSize = m_entityManager->GetContext()->m_gameMap->getMapSize();

	if (m_position.x < 0) m_position.x = 0;
	else if (m_position.x > (mapSize.x + 1) * Sheet::Tile_Size) m_position.x = (mapSize.x + 1) * Sheet::Tile_Size;

	if (m_position.y < 0) m_position.y = 0;
	else if (m_position.y > (mapSize.y + 1) * Sheet::Tile_Size)
	{
		m_position.y = (mapSize.y + 1) * Sheet::Tile_Size;
		setState(EntityState::Dying);
	}
	updateAABB();
}

void EntityBase::addVelocity(float l_x, float l_y)
{
	m_velocity += sf::Vector2f(l_x, l_y);
	if (abs(m_velocity.x) > m_maxVelocity.x)
	{
		if (m_velocity.x > 0) m_velocity.x = m_maxVelocity.x;
		else m_velocity.x = -m_maxVelocity.x;
	}
	if (abs(m_velocity.y) > m_maxVelocity.y)
	{
		if (m_velocity.y > 0) m_velocity.y = m_maxVelocity.y;
		else m_velocity.y = -m_maxVelocity.y;
	}
}

void EntityBase::accelerate(float l_x, float l_y)
{
	m_accelaration += sf::Vector2f(l_x, l_y);
}
void EntityBase::setAccelerations(float l_x, float l_y)
{
	m_accelaration = sf::Vector2f(l_x, l_y);
}

void EntityBase::applyFriction(float l_x, float l_y)
{
	if (m_velocity.x != 0)
	{
		if (abs(m_velocity.x) - abs(l_x) < 0) m_velocity.x = 0;
		else
		{
			if (m_velocity.x < 0) m_velocity.x += l_x;
			else m_velocity.x -= l_x;
		}
	}
}

void EntityBase::update(float l_dT)
{
	Map* map = m_entityManager->getContext()->m_gameMap;
	float gravity = map->getGravity();
	accelerate(0, gravity);
	addVelocity(m_accelaration.x * l_dT, m_accelaration.y * l_dT);
	setAccelerations(0.f, 0.f);
	sf::Vector2f frictionValue;
	if (m_refferenceTile)
	{
		frictionValue = m_refferenceTile->m_friction;
		if (m_refferenceTile->m_deadly) setState(EntityState::Dying);
	}
	else if (map->getDefaultTile()) frictionValue = map->getDefaultTile()->m_friction;
	else frictionValue = m_friction;
	float friction_x = (m_speed.x * frictionValue.x) * l_dT;
	float friction_y = (m_speed.y * frictionValue.y) * l_dT;
	applyFriction(friction_x, friction_y);
	sf::Vector2f deltaPos = m_velocity * l_dT;
	move(deltaPos.x, deltaPos.y);
	m_collisionOnX = false;
	m_collisionOnY = false;
	checkCollisions();
	resolveCollisions();
}

void EntityBase::updateAABB()
{
	m_AABB = sf::FloatRect(m_position.x - (m_size.x / 2), m_position.y - m_size.y, m_size.x, m_size.y);
}

void EntityBase::checkCollisions()
{
	Map* gameMap = m_entityManager->getContext()->m_gameMap;
	unsigned int tileSize = gameMap->getTileSize();
	int fromX = floor(m_AABB.left / tileSize);
	int toX = floor((m_AABB.left + m_AABB.width) / tileSize);
	int fromY = floor(m_AABB.top / tileSize);
	int toY = floor((m_AABB.top + m_AABB.height) / tileSize);

	for (int x = fromX; x <= toX; ++x)
	{
		for (int y = fromY; y <= toY; ++y)
		{
			Tile* tile = gameMap->getTile(x, y);
			if (!tile) continue;
			sf::FloatRect tileBounds(x * tileSize, y * tileSize, tileSize, tileSize);
			sf::FloatRect intersection;
			m_AABB.intersects(tileBounds, intersection);
			float area = intersection.width * intersection.height;

			CollisionElement e(area, tile->m_props, tileBounds);
			m_collisions.emplace_back(e);
			if (tile->m_warp && m_type == EntityType::Player)
				gameMap->loadNext();
		}
	}
}

void EntityBase::resolveCollisions()
{
	if (!m_collisions.empty())
	{
		std::sort(m_collisions.begin(), m_collisions.end(), sortCollisions);
		Map* gameMap = m_entityManager->getContext()->m_gameMap;
		unsigned int tileSize = gameMap->getTileSize();
		for (auto& itr : m_collisions)
		{
			if (!m_AABB.intersects(itr.m_tileBounds)) continue;
			float xDiff = (m_AABB.left + (m_AABB.width / 2)) - (itr.m_tileBounds.left + (itr.m_tileBounds.width / 2));
			float yDiff = (m_AABB.top + m_AABB.height /2) - (itr.m_tileBounds.top + itr.m_tileBounds.height / 2);
			float resolve = 0;
			if (abs(xDiff) > abs(yDiff))
			{
				if (xDiff > 0)
					resolve = (m_AABB.left + m_AABB.width) - itr.m_tileBounds.left;
				else
					resolve = -((m_AABB.left + m_AABB.width) - itr.m_tileBounds.left);
				move(resolve, 0);
				m_velocity.x = 0;
				m_collisionOnX = true;
			}
			else
			{
				if (yDiff > 0)
					resolve = (m_AABB.top + m_AABB.height) - itr.m_tileBounds.top;
				else
					resolve = -((m_AABB.top + m_AABB.height) - itr.m_tileBounds.top);
				move(0, resolve);
				m_velocity.y = 0;
				if (m_collisionOnY) continue;
				m_refferenceTile = itr.m_tile;
				m_collisionOnY = true;
			}
		}
		m_collisions.clear();
	}
	if (!m_collisionOnY)
		m_refferenceTile = nullptr;
}

bool sortCollisions(const CollisionElement& l_1, const CollisionElement& l_2)
{
	return l_1.m_area > l_2.m_area;
}
