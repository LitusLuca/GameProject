#pragma once
#include "Character.h"
#include "EventManager.h"
class Player :
    public Character
{
public:
    Player(EntityManager* l_entityMgr);
    ~Player();

    void react(EventDetails* l_details);
    void onEntityCollision(EntityBase* l_collider, bool l_attack);
};

