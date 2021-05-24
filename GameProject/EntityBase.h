#pragma once


enum class EntityType {
	Base, Player, Enemy
};
enum class EntityState {
	Idle, Walking, Jumping, Dying
};

struct TileInfo;
struct CollisionElement
{
	CollisionElement(float l_area, TileInfo* l_info, const sf::FloatRect& l_bounds) :
		m_area(l_area), m_tile(l_info), m_tileBounds(l_bounds) {}

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

	void move(float l_x, float l_y);
	void addVelocity(float l_x, float l_y);
	void accelerate(float l_x, float l_y);
	void setAccelerations(float l_x, float l_y);
	void applyFriction(float l_x, float l_y);
	virtual void update(float l_dT) = 0;
	virtual void draw(sf::RenderWindow* l_window) = 0;

	//getters and setters:
	void setPosition(const float& l_x, const float& l_y);
	void setPosition(const sf::Vector2f& l_pos);
	void setSize(const float& l_x, const float& l_y);
	void setState(const EntityState& l_state);

	std::string getName()const;
	EntityType getType()const;
	unsigned int getId()const;
	EntityState getState()const;
	const sf::Vector2f& getPosition()const;
	const sf::Vector2f& getSize()const;

protected:
	void updateAABB();
	void checkCollisions();
	void resolveCollisions();
	virtual void onEntityCollision(EntityBase* l_collider, bool attack) = 0;

	std::string m_name;
	EntityType m_type;
	unsigned int m_id;
	sf::Vector2f m_position;
	sf::Vector2f m_positionOld;
	sf::Vector2f m_velocity;
	sf::Vector2f m_maxVelocity;
	sf::Vector2f m_speed;
	sf::Vector2f m_accelaration;
	sf::Vector2f m_friction;
	TileInfo* m_refferenceTile;
	sf::Vector2f m_size;
	sf::FloatRect m_AABB;
	EntityState m_state;

	//collisons axis
	bool m_collisionOnX;
	bool m_collisionOnY;
	Collisions m_collisions;
	EntityManager* m_entityManager;
};

