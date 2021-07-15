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
	Attacking,
	Dying
};

struct CollisionElement
{
	CollisionElement(float l_area, TileInfo* l_info, const sf::FloatRect& l_bounds)
		: m_area(l_area), m_tile(l_info), m_tileBounds(l_bounds)
	{}
	float m_area;
	TileInfo* m_tile;
	sf::FloatRect m_tileBounds;
};

using Collisions = std::vector<CollisionElement>;

bool sortCollisions(const CollisionElement& l_1, const CollisionElement& l_2);

class EntityManager;
class EntityBase
{
	friend class EntityManager;
public:
	EntityBase(EntityManager* l_entityMgr);
	virtual ~EntityBase();
	void setPosition(const float& l_x, const float& l_y);
	void setPosition(const sf::Vector2f& l_pos);
	void setSize(const float& l_x, const float& l_y);
	void setState(const EntityState& l_state);

	const sf::Vector2f& getPosition() const;
	const sf::Vector2f& getSize() const;
	EntityState getState() const;
	std::string getName() const;
	unsigned int getId() const;
	EntityType getType() const;

	void move(float l_x, float l_y);
	void addVelocity(float l_x, float l_y);
	void accelerate(float l_x, float l_y);
	void setAcceleration(float l_x, float l_y);
	void applyFriction(float l_x, float l_y);
	virtual void update(float l_dT);
	virtual void draw(sf::RenderWindow* l_renderWnd) = 0;

protected:
	void updateAABB();
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

	bool m_collidingOnX;
	bool m_collidingOnY;
	Collisions m_collisions;
	EntityManager* m_entityManager;
};

