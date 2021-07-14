#pragma once
#include "Map.h"

enum class EntityType
{
	Base,
	Enemy,
	Player
};

enum class EntityState
{
	Idle,
	Walking,
	Jumping,
	Attacking
};

class EntityManager;
class EntityBase
{
	friend class EntityManager;
public:
	EntityBase(EntityManager* l_entityMgr);
	virtual ~EntityBase();
	//TODO: getters and setters

	void move(float l_x, float l_y);
	void addVelocity(float l_x, float l_y);
	void accelerate(float l_x, float l_y);
	void setAcceleration(float l_x, float l_y);
	void applyFriction(float l_x, float l_y);
	virtual void update(float l_dT);
	virtual void draw(sf::RenderWindow* l_renderWnd) = 0;

protected:
	void UpdateAABB();
	void checkCollision();
	void resolveCollision();
	virtual void onEntityCollision(EntityBase* l_collider, bool l_attack) = 0;
	std::string m_name;
	EntityType m_type;
	unsigned int m_id;
	sf::Vector2f m_position;
	sf::Vector2f m_positionOld;
	sf::Vector2f m_Velocity;
	sf::Vector2f m_maxVelocity;
	sf::Vector2f m_velocity;
	sf::Vector2f m_speed;
	sf::Vector2f m_acceleration;
	sf::Vector2f m_friction;
	TileInfo* m_referenceTile;
	sf::Vector2f m_size;
	sf::FloatRect m_AABB;
	EntityState m_state;

	bool collidingOnX;
	bool collidingOnY;
	Collisions m_collisions;
	EntityManager* m_entityManager;
};

