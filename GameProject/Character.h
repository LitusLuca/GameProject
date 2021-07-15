#pragma once
#include "EntityBase.h"
#include "Direction.h"
#include "SpriteSheet.h"

class Character :
    public EntityBase
{
    friend class EntityManager;
public:
    Character(EntityManager* l_entityMgr);
    virtual ~Character();

    void move(const Direction& l_dir);
    void jump();
    void attack();
    void getHurt();
    void load(const std::string& l_path);
    virtual void onEntityCollision(EntityBase* l_collider, bool l_attack) = 0;
    virtual void update(float l_dT);
    void draw(sf::RenderWindow* l_renderWnd);

protected:
    void updateAttackAABB();
    void animate();
    SpriteSheet m_spriteSheet;
    float m_jumpVelocity;
    int m_hitpoints;
    sf::FloatRect m_attackAABB;
    sf::Vector2f m_attackAABBOffset;
};

