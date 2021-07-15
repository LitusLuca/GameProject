#pragma once
#include "Character.h"
class Player :
    public Character
{
    Player(EntityManager* l_entityMgr);
    ~Player();

    void react(EventDetails* l_details);
    void onEntityCollision(EntityBase* l_collider, bool l_attack);
};

